/*! \file fft.h
 *  \brief Header file for the fft class.
 *
* 此类是对 `fftw3` 库的包装类，包含用于执行 64 点正向和逆向 FFT 的函数。
*/

#ifndef FFT_H
#define FFT_H

#include <complex>
#include <fftw3.h>
#include <vector>

namespace fun
{
    /*!
     * \brief The fft class
     *
    此类是 fftw3 库的包装类，包含在发送链和接收链中执行 IFFT 和 FFT 所需的函数和必要参数。
     */
    class fft
    {
    public:


              /*!
              * \brief `fft` 对象的构造函数  
                \param `fft_length` FFT 的长度，例如 64 点 FFT

      /*!*/
              fft(int fft_length);
              /*
              * \brief 就地执行 64 点正向 FFT  
              * \param `data` 包含 64 个时域复样本的数组，将其转换为频域样本。
              */
        void forward(std::complex<double> data[64]);

              /*!
              * \brief 对输入数据进行就地逆 FFT  
                \param `data` 频域中的复数双精度向量，将其转换为时域。数据向量的长度必须是 `#m_fft_length` 的整数倍。
              */
        void inverse(std::vector<std::complex<double> > & data);

    private:

        /*!
         * \brief Mapping to/from FFT order.
         */
        static const int fft_map[64];

        /*!
         * \brief  FFT 的长度。在 802.11a 中始终是 64 点 FFT，因为有 64 个子载波。
         */
        int m_fft_length;        

        /*!
         * \brief 用于 `fftw3` 库的前向输入缓冲区。
         */
        fftw_complex * m_fftw_in_forward;

        /*!
         * \brief 用于 `fftw3` 库的前向输出缓冲区。
         */
        fftw_complex * m_fftw_out_forward;

        /*!
         * \brief 用于 fftw3 库的逆向输入缓冲区。
         */
        fftw_complex * m_fftw_in_inverse;

        /*!
         * \brief 用于 fftw3 库的逆向输出缓冲区。
         */
        fftw_complex * m_fftw_out_inverse;

        /*!
         * \brief 用于 `fftw3` 库的正向 FFT 方案。
         */
        fftw_plan m_fftw_plan_forward;

        /*!
         * \brief 用于 `fftw3` 库的逆向 FFT 方案。
         */
        fftw_plan m_fftw_plan_inverse;
    };
}


#endif // FFT_H
