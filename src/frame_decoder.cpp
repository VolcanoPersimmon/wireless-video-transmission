/*! \file frame_decoder.cpp
 *  \brief C++ file for the Frame Decoder block.
 *
 *  The Frame Decoder block is in charge of decoding the frame once the data
 *  subcarriers have been recovered from the received frame.
 */

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <boost/crc.hpp>

#include "frame_decoder.h"
#include "qam.h"
#include "interleaver.h"
#include "viterbi.h"
#include "parity.h"
#include "rates.h"
#include "modulator.h"
#include "puncturer.h"
#include "interleaver.h"
#include "ppdu.h"

namespace fun
{
    /*!
     * - Initializations:
     *   + #m_current_frame -> Reset to a frame of 0 length with RATE_1_2_BPSK
     */
    frame_decoder::frame_decoder() :
        block("frame_decoder"),
        m_current_frame(FrameData(RateParams(RATE_3_4_QAM64)))
    {
        m_current_frame.Reset(RateParams(RATE_3_4_QAM64), 0, 0);
    }

    /*!
        *  当检测到帧开始时，该块首先尝试解码 ppdu 头。
        *  如果通过对头部比特的简单奇偶校验检查确定成功，
        *  则它会尝试使用从头部收集的参数解码帧的有效负载。
        *  如果通过 IEEE CRC-32 检查确定成功，
        *  解码后的有效负载将被传递到 output_buffer，以便返回给接收链，
        *  以便可以传递到 MAC 层。

     */
    void frame_decoder::work()
    {
        if(input_buffer.size() == 0) return;
        output_buffer.resize(0);

        // Step through each 48 sample symbol
        for(int x = 0; x < input_buffer.size(); x++)
        {
            // Copy over available symbols
            if(m_current_frame.samples_copied < m_current_frame.sample_count)
            {
                memcpy(&m_current_frame.samples[m_current_frame.samples_copied], &input_buffer[x].samples[0], 48 * sizeof(std::complex<double>));
                m_current_frame.samples_copied += 48;
            }

            // Decode the frame if possible
            if(m_current_frame.samples_copied >= m_current_frame.sample_count && m_current_frame.sample_count != 0)
            {
                ppdu frame = ppdu(m_current_frame.rate_params.rate, m_current_frame.length);
                if(frame.decode_data(m_current_frame.samples))
                {
                    output_buffer.push_back(frame.get_payload());
                }
                m_current_frame.sample_count = 0;
            }

            // Look for a start of frame
            if(input_buffer[x].tag == START_OF_FRAME)
            {
                // Attempt to decode the header
                ppdu h = ppdu();
                std::vector<std::complex<double> > header_samples(48);
                memcpy(header_samples.data(), input_buffer[x].samples, 48 * sizeof(std::complex<double>));
                if(!h.decode_header(header_samples)) continue;

                // Calculate the frame sample count
                int length = h.get_length();
                RateParams rate_params = RateParams(h.get_rate());
                int frame_sample_count = h.get_num_symbols() * 48;

                // Start a new frame
                m_current_frame.Reset(rate_params, frame_sample_count, length);
                m_current_frame.samples.resize(h.get_num_symbols() * 48);
                continue;
            }
        }
    }
}


