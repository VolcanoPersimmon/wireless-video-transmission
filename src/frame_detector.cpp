/*! \file frame_detector.cpp
 *  \brief C++ file for the Frame Detector block.
 *
 * This block is in charge of detecting the beginning of a frame using the
 * short training sequence in the preamble.
 */

#include <cstring>
#include <iostream>

#include "frame_detector.h"

namespace fun
{
    /*!
     * - Initializations:
     *   + #m_power_acc      -> #STS_LENGTH (16 samples)
     *   + #m_corr_acc       -> #STS_LENGTH (16 samples)
     *   + #m_carryover      -> #STS_LENGTH (16 samples)
     *   + #m_plateau_length -> 0
     *   + #m_plateau_flag   -> false
     */
    frame_detector::frame_detector() :
        block("frame_detector"),
        m_power_acc(STS_LENGTH),
        m_corr_acc(STS_LENGTH),
        m_carryover(STS_LENGTH, 0),
        m_plateau_length(0),
        m_plateau_flag(false)
    {
    }

    /*!
     *  该块使用自相关来检测短训练序列。
*  这种自相关是通过移动窗口平均值实现的，
*  使用循环累加器跟踪当前的自相关和输入样本的输入功率。
*  然后将归一化的自相关与阈值进行比较，以确定
*  当前样本是否属于短训练序列（STS）。

     */
    void frame_detector::work()
    {
        if(input_buffer.size() == 0) return;
        output_buffer.resize(input_buffer.size());

        // Step through the samples
        for(int x = 0; x < input_buffer.size(); x++)
        {
            output_buffer[x].tag = NONE;

            // Get the delayed samples
            std::complex<double> delayed;
            if(x < STS_LENGTH) delayed = m_carryover[x];
            else delayed = input_buffer[x-STS_LENGTH];

            // Update the correlation accumulators
            m_corr_acc.add(input_buffer[x] * std::conj(delayed));

            // Update the power accumulator
            m_power_acc.add(std::norm(input_buffer[x]));

            // Calculate the normalized correlations
            double corr = std::abs(m_corr_acc.sum) / m_power_acc.sum;

            if(corr > PLATEAU_THRESHOLD)
            {
                m_plateau_length++;
                if(m_plateau_length == STS_PLATEAU_LENGTH)
                {
                    output_buffer[x].tag = STS_START;
                    m_plateau_flag = true;
                }
            }
            else
            {
                if(m_plateau_flag)
                {
                    output_buffer[x].tag = STS_END;
                    m_plateau_flag = false;
                }
                m_plateau_length = 0;
            }

            // Pass through the sample
            output_buffer[x].sample = input_buffer[x];
        }

        // Carryover the last 16 output samples
        memcpy(&m_carryover[0],
               &input_buffer[input_buffer.size() - STS_LENGTH],
               STS_LENGTH * sizeof(std::complex<double>));
    }

}


