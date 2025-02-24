/*! \file receiver_chain.cpp
 *  \brief C++ file for Receiver Chain class.
 *
接收链类是用于接收和解码物理层帧的模块的主要控制器。它包含每个模块的实例，并在处理过程中将数据传递到接收链，最终返回正确接收的负载（MPDUs），这些负载可以传递给上层。
 */

#include <iostream>
#include <functional>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "receiver_chain.h"

namespace fun
{
    /*!
     * -Initializes each receiver chain block:
     *  + frame_detector
     *  + timing_sync
     *  + fft_symbols
     *  + channel_est
     *  + phase_tracker
     *  + frame_decoder
     *
     *  Adds each block to the receiver chain.
     */
    receiver_chain::receiver_chain()
    {
        m_frame_detector = new frame_detector();
        m_timing_sync = new timing_sync();
        m_fft_symbols = new fft_symbols();
        m_channel_est = new channel_est();
        m_phase_tracker = new phase_tracker();
        m_frame_decoder = new frame_decoder();

        // We use semaphore references, so we don't
        // want them to move to a different memory location
        // if the vectors get resized
        m_wake_sems.reserve(100);
        m_done_sems.reserve(100);

        // Add the blocks to the receiver chain
        add_block(m_frame_detector);
        add_block(m_timing_sync);
        add_block(m_fft_symbols);
        add_block(m_channel_est);
        add_block(m_phase_tracker);
        add_block(m_frame_decoder);
    }

    /*!
     add_block 函数为每个模块创建一个唤醒和完成信号量。
然后，它为模块创建一个新的线程，并将该线程添加到线程向量中以供引用。
     */
    void receiver_chain::add_block(fun::block_base * block)
    {
        m_wake_sems.push_back(sem_t());
        m_done_sems.push_back(sem_t());
        int index = m_wake_sems.size() - 1;
        sem_init(&m_wake_sems[index], 0, 0);
        sem_init(&m_done_sems[index], 0, 0);
        m_threads.push_back(std::thread(&receiver_chain::run_block, this, index, block));
    }

    /*!
    #run_block 函数是控制对每个模块工作函数调用的主线程。该函数是一个无限循环，首先等待 wake_semaphore 被触发，以指示模块该“醒来”并处理刚刚放入其 input_buffer 中的数据，通过运行其 work() 函数。然后，一旦 work() 函数返回，run_block 就会向 done semaphore 发送信号，表示模块已完成对 input_buffer 中所有内容的处理。此时，它会循环回去，并在下一组输入数据准备好时等待模块再次被“唤醒”。
     */
    void receiver_chain::run_block(int index, fun::block_base * block)
    {
        while(1)
        {
            sem_wait(&m_wake_sems[index]);

            boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();
            block->work();
            boost::posix_time::time_duration elapsed = boost::posix_time::microsec_clock::local_time() - start;

            if(elapsed.total_microseconds() > (2000 / 5e6 * 1e6))
            {
                //std::cout << "! - " <<  block->name << std::endl;
            }

            sem_post(&m_done_sems[index]);
        }
    }

    /*!
     该函数是接收链的主要调度器。它从 USRP 模块接收原始复数样本，并首先将它们传递到帧检测器模块的输入缓冲区。然后，它通过向每个模块的“唤醒”信号量发送信号来解锁每个线程。接着，它等待每个线程发出信号，表示它已完成对其 work() 函数的调用。一旦所有线程完成，它会将每个模块的输出缓冲区内容移到链中下一个模块的输入缓冲区，并返回帧解码器的输出缓冲区内容。
     */
    std::vector<std::vector<unsigned char> > receiver_chain::process_samples(std::vector<std::complex<double> > samples)
    {
        // samples -> sync short in
        m_frame_detector->input_buffer.swap(samples);

        // Unlock the threads
        for(int x = 0; x < m_wake_sems.size(); x++) sem_post(&m_wake_sems[x]);

        // Wait for the threads to finish
        for(int x = 0; x < m_done_sems.size(); x++) sem_wait(&m_done_sems[x]);

        // Update the buffers
        m_timing_sync->input_buffer.swap(m_frame_detector->output_buffer);
        m_fft_symbols->input_buffer.swap(m_timing_sync->output_buffer);
        m_channel_est->input_buffer.swap(m_fft_symbols->output_buffer);
        m_phase_tracker->input_buffer.swap(m_channel_est->output_buffer);
        m_frame_decoder->input_buffer.swap(m_phase_tracker->output_buffer);

        // Return any completed packets
        return m_frame_decoder->output_buffer;
    }

}
