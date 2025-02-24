/*! \file frame_builder.h
 *  \brief Header file for the Frame Builder class.
 *
* 此类接收输入数据（MPDUs）并构建 PHY 层帧，首先为数据添加 PHY 头部，并附加 IEEE CRC-32 校验和。然后对数据进行编码，包括扰码、以指定的编码率进行卷积编码（这可能需要或不需要打孔），并对数据进行交织。接着按照指定的方式对数据进行调制。调制后的数据被映射为符号，其中数据映射到相应的数据子载波，而导频和空载波映射到它们各自的子载波。每个符号经过 IFFT（逆快速傅里叶变换）处理以转换为时域，并为每个符号附加循环前缀。最后，将符号连接在一起并在前面添加前导码以完成帧构建。然后返回帧，以便将其传递给 USRP 进行传输。
 */


#ifndef FRAME_BUILDER_H
#define FRAME_BUILDER_H

#include <vector>
#include <complex>

#include "fft.h"
#include "rates.h"

namespace fun
{
    /*!
     * \brief The frame_builder class.
     *
     *  输入： vector of unsigned chars representing 
     *              the payload (MPDU) and 
     *              the transmission PHY
     *              transmission rate.
     *
     *  输出： vector of raw complex samples representing 
     *               the digital base-band time domain signal to be passed to the usrp class for up-conversion and transmission over the air.
     *
     *  This class takes input data (MPDUs) and builds PHY layer frames by first prepending a PHY header
     *  and appending an IEEE CRC-32 checksum to the data. It then encodes the data by scrambling混淆,
     *  convolutional coding卷积编码 the data at the coding rate specified (this may or may not require puncturing打孔),
     *  and interleaving交织 the data. It then modulates the data as specified. The modulated data
     *  is then mapped into symbols with the data being mapped to the respective data subcarriers with pilots
     *  and nulls being mapped to their respective subcarriers. Each symbol is then run through an IFFT to
     *  convert it to time domain an a cyclic prefix is attached to each symbol. Finally, the symbols are
     *  concatenated together and a preamble is prepended to complete the frame. The frame is then returned
     *  so that it can be passed to the USRP for transmission.
     * 此类接收输入数据（MPDU），并通过在数据前添加一个PHY头和在数据末尾附加一个IEEE CRC-32校验和来构建PHY层帧。然后，它通过对数据进行扰码、以指定的编码速率进行卷积编码（这可能需要或不需要打孔），并对数据进行交织。接下来，数据按照规定进行调制。调制后的数据被映射为符号，数据被映射到相应的数据子载波上，导频和空载波也被映射到各自的子载波上。每个符号都经过逆傅里叶变换（IFFT），以将其转换为时域，并在每个符号上附加循环前缀。最后，这些符号被连接在一起，前面添加一个前导码，以完成帧的构建。然后返回该帧，以便将其传递给USRP进行传输。
     */
    class frame_builder
    {
    public:

        /*!
         * \brief Constructor for frame_builder class
         */
        frame_builder();

        /*!
         * \brief Main function for building a PHY frame
         * \param payload (MPDU) the data that needs to be transmitted over the air.
         * \param rate the PHY transmission rate at which to transmit the respective data at.
         * \return A vector of complex doubles representing the digital base-band time domain signal
         *  to be passed to the usrp class for up-conversion and transmission over the air.
         */
        std::vector<std::complex<double> >  build_frame(std::vector<unsigned char> payload, Rate rate);

    private:

        fft m_ifft; //!< The fft instance used to perform the inverse FFT on the OFDM symbols

    };
}


#endif // FRAME_BUILDER_H
