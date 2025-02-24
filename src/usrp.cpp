/*! \file usrp.cpp
 *  \brief C++ file for the usrp class.
 *
usrp 类是 UHD API 的一个包装类。它提供了一个简单的接口，用于从 USRP 发送和接收样本。

usrp_params 类是一个容器，用于保存 USRP 所需的参数，例如中心频率、采样率、发送/接收增益等。
 */

#include "usrp.h"

namespace fun
{
    /*!
    **初始化**
  * `#m_params` -> 先前初始化的 `usrp_params` 对象，包含 USRP 所需的参数。
     */
    usrp::usrp(usrp_params params) :
        m_params(params)
    {
        // 实例化 multi_usrp
        // m_usrp = uhd::usrp::multi_usrp::make("uhd::device_addr_t(m_params.device_addr)");
        m_usrp = uhd::usrp::multi_usrp::make("");
        //m_usrp = uhd::usrp::multi_usrp::make("");
        m_device = m_usrp->get_device();

        // 设置中心频率
        m_usrp->set_tx_freq(uhd::tune_request_t(m_params.freq));
        m_usrp->set_rx_freq(uhd::tune_request_t(m_params.freq));

        // 设置采样率
        m_usrp->set_tx_rate(m_params.rate);
        m_usrp->set_rx_rate(m_params.rate);

        // 设置天线增益
        m_usrp->set_tx_gain(m_params.tx_gain);
        m_usrp->set_rx_gain(m_params.rx_gain);

        // 设置接收天线
        //m_usrp->set_rx_antenna("RX2");

        // 获取 TX 和 RX 流句柄
        m_tx_streamer = m_usrp->get_tx_stream(uhd::stream_args_t("fc64"));
        m_rx_streamer = m_usrp->get_rx_stream(uhd::stream_args_t("fc64"));

        // 启动 RX 流
        uhd::stream_cmd_t stream_cmd(uhd::stream_cmd_t::STREAM_MODE_START_CONTINUOUS);
        stream_cmd.stream_now = true;
        m_usrp->issue_stream_cmd(stream_cmd);

        sem_init(&m_tx_sem, 0, 0);
        sem_post(&m_tx_sem);
    }

    /*!
     * 发送一批样本到 USRP，这些样本表示要由 USRP 上变频并传输的数字基带信号。

    此函数在调用后不会阻塞。由于 UHD API 的多线程特性，此函数可能会在 USRP 完成所有样本的传输之前返回。这通常是可以的，因为后续对该方法的调用会在 USRP 中缓冲更多样本。然而，如果调用速度不够快，可能会发生下溢。有关更多详细信息，请参见 [Ettus 网站的链接](http://files.ettus.com/manual/page_general.html#general_ounotes)。
    
     */
    void usrp::send_burst(std::vector<std::complex<double> > samples)
    {
        sem_wait(&m_tx_sem);

        uhd::tx_metadata_t tx_metadata;
        tx_metadata.start_of_burst = true;
        tx_metadata.end_of_burst = true;
        tx_metadata.has_time_spec = false;
        m_tx_streamer->send(&samples[0], samples.size(), tx_metadata);

        sem_post(&m_tx_sem);
    }


    /*!
     * 发送一批样本到 USRP，这些样本表示要由 USRP 上变频并传输的数字基带信号。

        * 此函数使用信号量来阻塞，直到 USRP 响应确认所有样本已通过无线传输。这可以防止用户一次发送过多数据，从而使用户在传输完成时有一定的感觉。如果用户调用此函数的速度不够快，可能会发生下溢。有关更多详细信息，请参见 [Ettus 网站的链接](http://files.ettus.com/manual/page_general.html#general_ounotes)。
     */
    void usrp::send_burst_sync(std::vector<std::complex<double> > samples)
    {
        // 将样本按 `m_amp` 进行缩放。
        if(m_params.tx_amp != 1.0)
            for(int x = 0; x < samples.size(); x++)
                samples[x] *= m_params.tx_amp;

        // 发送样本。
        uhd::tx_metadata_t tx_metadata;
        tx_metadata.start_of_burst = true;
        tx_metadata.end_of_burst = true;
        tx_metadata.has_time_spec = false;
        m_tx_streamer->send(&samples[0], samples.size(), tx_metadata);

        // 等待突发结束确认（ACK）随后发生的下溢。
        bool got_ack = false;
        bool got_underflow = false;
        uhd::async_metadata_t async_metadata;
        while(!got_ack && !got_underflow && m_device->recv_async_msg(async_metadata, 1))
        {
            got_ack = (async_metadata.event_code == uhd::async_metadata_t::EVENT_CODE_BURST_ACK);
            got_underflow = (got_ack && async_metadata.event_code == uhd::async_metadata_t::EVENT_CODE_UNDERFLOW);
        }
    }

    /*!
     从 USRP 获取 num_samples 并将其放入缓冲区参数中。如果此函数调用得“不够快”，USRP 会感到不满，因为计算机没有足够快地消费样本，无法跟上 USRP 的接收采样率。这将导致 USRP 指示溢出，从而无法保证检索数据的完整性。有关更多详细信息，请参见 Ettus 网站的链接。 *
     */
    void usrp::get_samples(int num_samples, std::vector<std::complex<double> > & buffer)
    {
        // Get some samples
        uhd::rx_metadata_t rx_meta;
        m_rx_streamer->recv(&buffer[0], num_samples, rx_meta);
    }

}

