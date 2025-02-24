/*! \file ppdu.h
 *  \brief Header file for the ppdu class and the plcp_header struct.
 *
*  ppdu 类是一个用于存放 ppdu 的容器，ppdu 由 PHY 头（也称为 PLCP 头）和一个字节负载组成。
*  该类还具有根据 PHY 传输速率对头部和负载进行编码和解码的函数。

 */

#ifndef PPDU_H
#define PPDU_H

#include <complex>
#include <vector>
#include "rates.h"

#define MAX_FRAME_SIZE 2000

namespace fun
{
    /*!
     * \brief *  plcp_header 结构体是 PLCP 头及其相应参数的容器。

     */
    struct plcp_header
    {
        Rate rate;                  //!< The PHY Rate for this frame.
        int length;                 //!< The length of the payload in bytes.
        int num_symbols;            //!< The number of ofdm symbols in the frame.
        unsigned short service;     //!< The service field.

        /*!
         * \brief Constructor for empty plcp_header.
         *
         * -Initializations:
         *  + rate -> RATE_1_2_BPSK i.e. BPSK modulation with 1/2 rate convolutional code
         *  + length -> 0
         *  + num_symbols -> 0
         *  + service -> 0
         */
        plcp_header()
        {
            rate = RATE_1_2_BPSK;
            length = 0;
            num_symbols = 0;
            service = 0;
        }

        /*!
         * \brief Constructor for plcp_header with rate and length known.
         * \param _rate PHY Rate for this frame.
         * \param _length Length of payload in bytes.
         */
        plcp_header(Rate _rate, int _length)
        {
            rate = _rate;
            length = _length;
            service = 0;
        }

        /*!
         * \brief plcp_header 的构造函数，已知速率、长度和符号数量。
        * \param _rate 该帧的 PHY 速率。
        * \param _length 有效负载的字节长度。
        * \param _num_symbols 帧中的 OFDM 符号数量。

         */
        plcp_header(Rate _rate, int _length, int _num_symbols)
        {
            rate = _rate;
            length = _length;
            num_symbols = _num_symbols;
            service = 0;
        }

    };

    /*!
     * \brief The ppdu class
     *
     *  ppdu 类是一个用于存放 ppdu 的容器，ppdu 由 PHY 头（也称为 PLCP 头）和一个字节负载组成。
*  该类还具有根据 PHY 传输速率对头部和负载进行编码和解码的函数。

     */
    class ppdu
    {
    public:

        /****************
         * Constructors *
         ****************/

        /*!
         * \brief Default constructor for empty ppdu
         */
        ppdu();

        /*!
         * \brief Constructor for ppdu with rate and length known.
         * \param rate PHY Rate for this frame.
         * \param length Length of payload in bytes.
         */
        ppdu(Rate rate, int length);

        /*!
         * \brief Constructor for ppdu with payload and Rate known.
         * \param payload The payload/data/MPDU to be transmitted.
         * \param rate The PHY rate for this frame.
         */
        ppdu(std::vector<unsigned char> payload, Rate rate);

        /******************
         * Public Members *
         ******************/

        /*!
         * \brief Public interface for encoding a ppdu
         * \return Modulated data as a vector of complex doubles
         */
        std::vector<std::complex<double> > encode();

        /*!
        * \brief plcp_header 解码的公共接口。
* \param samples 代表编码头符号的复数样本。
* \return 布尔值，指示解码头部是否成功，
*  通过检查/比较头部中的 1 位奇偶校验字段来判断。
*  如果成功，则对象的 #header 字段将适当地填充解码后的字段。

         */
        bool decode_header(std::vector<std::complex<double> > samples);

        /*!
         * \brief 将 PHY 负载解码为 PPDU 的公共接口。
* \param samples 代表编码负载符号的复数样本。
* \return 布尔值，指示解码负载是否成功，
*  通过计算并比较附加在负载末尾的 IEEE CRC-32 来判断。
*  如果成功，则对象的 #payload 字段将填充解码后的负载/MPDU。

         */
        bool decode_data(std::vector<std::complex<double> > samples);


        Rate get_rate(){return header.rate;}     //!< Get this PPDU's PHY tx rate
        int get_length(){return header.length;}  //!< Get this PPDU's payload length
        int get_num_symbols(){return header.num_symbols;} //!< Get the number of OFDM symbols in this PPDU
        std::vector<unsigned char> get_payload(){return payload;} //!< Get the payload of this PPDU.

    private:

        plcp_header header; //!< This PPDU's header parameters
        std::vector<unsigned char> payload; //!< This PPDU's payload

        /*!
         * \brief Encodes this PPDU's header. The header is always encoded with
         *  BPSK modulation and 1/2 rate convolutional code.
         * \return The modulated header symbol.
         */
        std::vector<std::complex<double> > encoder_header();

        /*!
         * \brief Encodes this PPDU's payload. The payload is encoded at the rate
         *  specified in the header.rate field.
         * \return The modulated data.
         */
        std::vector<std::complex<double> > encode_data();

    };

}


#endif // PPDU_H
