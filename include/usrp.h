/*! \file usrp.h
 *  \brief Header file for the usrp class and the usrp_params struct.
 *
 *  The usrp class is a wrapper class for the UHD API. It provides a simple interface for transmitting
 *  and receiving samples to/from the USRP.
 *
 *  The usrp_params class is a container for holding the necessary parameters for the usrp
 *  such as center frequency, sample rate, tx/rx gain, etc..
 */

#ifndef USRP_H
#define USRP_H

#include <uhd/usrp/multi_usrp.hpp>
#include <uhd/convert.hpp>
#include <uhd/stream.hpp>
#include <semaphore.h>
#include <memory>

namespace fun
{
    /*!
     * \brief  usrp_params struct主要包括USRP的参数：
                    中心频率、采样率、收发功率增益等等
     */
    struct usrp_params
    {
        double freq;                //!< Center Frequency
        double rate;                //!< Sample Rate
        double tx_gain;             //!< Transmit Gain 
        double rx_gain;             //!< Receive Gain  
        double tx_amp;              //!< Transmit Amplitude  在发送到 USRP 之前对所有发送采样进行缩放
        std::string device_addr;    //!< IP Address of USRP as a string - i.e. "192.168.10.2" or "" to find automatically

        /*!
         * \brief usrp_params 的构造函数。只需初始化成员字段，以便稍后查找。
         * \param freq -> #freq
         * \param rate -> #rate
         * \param tx_gain -> #tx_gain
         * \param rx_gain -> #rx_gain
         * \param tx_amp -> #tx_amp
         * \param device_addr -> #device_addr
         */
        usrp_params(double freq = 5.72e9, double rate = 5e6, double tx_gain=20, double rx_gain=20, double tx_amp=1.0, std::string device_addr="") :
            freq(freq),
            rate(rate),
            tx_gain(tx_gain),
            rx_gain(rx_gain),
            tx_amp(tx_amp),
            device_addr(device_addr)
        {
        }
    };

    /*!
     * \brief 一个简单的类，用于连接 USRP。
     *
     *  usrp 类是 UHD 应用程序接口的封装类。它提供了一个向 USRP 发送和接收采样的简单接口。
     */
    class usrp
    {
    public:

        /*!
         * \brief usrp 类的构造函数。
         * \param params 这些参数将在 USRP 对象的生命周期内保持不变。
         */
        usrp(usrp_params params);


        // 发送采样脉冲串，并阻塞直到脉冲串结束
        /*!
         * \brief 发送采样脉冲串，并阻塞直到脉冲串结束
         * \param samples A vector of complex doubles，代表 USRP 上变频和传输的基带时域信号。
         */
        void send_burst_sync(std::vector<std::complex<double> > samples);

        /*!
         * \brief 发送采样脉冲串，但在脉冲串结束前不阻塞。
         * \param samples A vector of complex doubles，代表 USRP 上变频和传输的基带时域信号。
         */
        void send_burst(std::vector<std::complex<double> > samples);

        //  从 USRP 获取一些 sample
        /*!
         * \brief 获取 num_samples个样本，并将其放入缓冲区的前num_samples的位置。
         * \param num_samples 从 USRP 提取的样本数量。
         * \param buffer 放置检索到的样本的缓冲区。
         */
        void get_samples(int num_samples, std::vector<std::complex<double> > & buffer);


    private:


        usrp_params m_params; //!< Container for the parameters for this instance of the USRP class.

        std::shared_ptr<uhd::usrp::multi_usrp> m_usrp; //!< multi_usrp (main USRP handle)
        std::shared_ptr<uhd::device> m_device;         //!< device (receives async messages)
        uhd::rx_streamer::sptr m_rx_streamer;            //!< TX (output) streamer
        uhd::tx_streamer::sptr m_tx_streamer;            //!<  RX (input) streamer

        sem_t m_tx_sem;                                  //!< Sempahore used to block for #send_burst_sync
    };

}


#endif // USRP_H
