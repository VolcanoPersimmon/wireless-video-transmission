/*! \file frame_builder.cpp
 *  \brief C++ for the Frame Builder class.
 *
*  该类接收输入数据（MPDU），通过首先在数据前加上 PHY 头
*  并在数据后附加 IEEE CRC-32 校验和来构建 PHY 层帧。
*  然后通过加扰和卷积编码按照指定的编码率对数据进行编码
*  （这可能需要或不需要打孔），并对数据进行交织处理。
*  随后按照指定方式对数据进行调制。调制后的数据
*  被映射为符号，数据被映射到相应的数据子载波，
*  而导频和空载波则被映射到它们各自的子载波。
*  每个符号随后通过 IFFT 转换到时域，并为每个符号附加循环前缀。
*  最后，符号被串联在一起，并在帧的开头加上前导码以完成帧的构建。
*  该帧随后返回，以便传递给 USRP 进行传输。

 */

#include <arpa/inet.h>
#include <boost/crc.hpp>

#include "frame_builder.h"
#include "interleaver.h"
#include "qam.h"
#include "ppdu.h"
#include "symbol_mapper.h"
#include "fft.h"
#include "preamble.h"
#include "rates.h"
#include "viterbi.h"
#include "parity.h"
#include "modulator.h"
#include "puncturer.h"


#define preamble_length 256 //!< Not cyclic prefixed at this point yet

namespace fun
{
    /*!
    * -Initializations
    *  + #m_ifft -> 64 point IFFT object
    */
    frame_builder::frame_builder() :
        m_ifft(64)
    {
    }

    /*!
    *  build_frame 函数是主要函数，它将输入数据转换为原始输出样本。
*  ppdu 类用于附加 PHY 头、加扰、卷积编码、交织并调制输入数据。
*  符号映射器将调制后的数据转换为符号，并将数据、导频和空载波映射到各自的子载波。
*  IFFT 执行逆快速傅里叶变换（IFFT）。
*  循环前缀被添加，最后前导码被附加在帧的开头以完成帧的构建，
*  然后返回该帧以传递给 USRP 块。

     */
    std::vector<std::complex<double> > frame_builder::build_frame(std::vector<unsigned char> payload, Rate rate)
    {
        //Append header, scramble, code, interleave, & modulate
        ppdu ppdu_frame(payload, rate);        
        std::vector<std::complex<double> > samples = ppdu_frame.encode();

        // Map the subcarriers and insert pilots
        symbol_mapper mapper = symbol_mapper();
        std::vector<std::complex<double> > mapped = mapper.map(samples);

        // Perform the IFFT
        m_ifft.inverse(mapped);

        // Add the cyclic prefixes
        std::vector<std::complex<double> > prefixed(mapped.size() * 80 / 64);
        for(int x = 0; x < mapped.size() / 64; x++)
        {
            memcpy(&prefixed[x*80], &mapped[x*64+48], 16*sizeof(std::complex<double>));
            memcpy(&prefixed[x*80+16], &mapped[x*64], 64*sizeof(std::complex<double>));
        }

        // Prepend the preamble
        std::vector<std::complex<double> > frame(prefixed.size() + 320);

        memcpy(&frame[0], &PREAMBLE_SAMPLES[0], 320 * sizeof(std::complex<double>));
        memcpy(&frame[320], &prefixed[0], prefixed.size() * sizeof(std::complex<double>));

        // Return the samples
        return frame;
    }
}

