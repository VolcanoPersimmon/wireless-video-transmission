/*! \file fft.cpp
 *  \brief C++ for the fft class.
 *
* 此类是对 `fftw3` 库的包装类，包含用于执行 64 点正向和逆向 FFT 的函数。
 */

#include <cstring>
#include <assert.h>

#include "fft.h"

namespace fun
{

    /*!
        该映射将子载波移位，使其不是按 0-63 的顺序排列，而是可以被视为正频率和负频率，这正是 FFTW3 库使用的顺序。
     */
    const int fft::fft_map[64] =
    {
      32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
    };

    /*!
     * -初始化:
     *  + #`m_fft_length` -> 64，因为我们始终处理 64 点 FFT，因为有 64 个 OFDM 子载波。
     */
    fft::fft(int fft_length) :
        m_fft_length(fft_length)
    {
        // Allocate the FFT buffers
        m_fftw_in_forward = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_fft_length);
        m_fftw_out_forward = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_fft_length);
        m_fftw_in_inverse = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_fft_length);
        m_fftw_out_inverse = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * m_fft_length);
        m_fftw_plan_forward = fftw_plan_dft_1d(m_fft_length, m_fftw_in_forward, m_fftw_out_forward, FFTW_FORWARD, FFTW_MEASURE);
        m_fftw_plan_inverse = fftw_plan_dft_1d(m_fft_length, m_fftw_in_inverse, m_fftw_out_inverse, FFTW_BACKWARD, FFTW_MEASURE);
    }


    /*!
     * 此函数对 64 个复数据样本执行单次就地 64 点 FFT。  
  用户必须遍历时域信号，并将每个 64 个样本的符号单独传递给此函数。  
  此函数处理从所有正（0-63）索引到正负频率索引的移位。
     */
    void fft::forward(std::complex<double> data[64])
    {
        memcpy(m_fftw_in_forward, &data[0], m_fft_length * sizeof(std::complex<double>));
        fftw_execute(m_fftw_plan_forward);

        for(int s = 0; s < 64; s++)
        {
            memcpy(&data[s], &m_fftw_out_forward[fft_map[s]], sizeof(std::complex<double>));
        }
    }

    /*!
    * 此函数遍历输入向量（其长度必须是 64 的整数倍），  
  并对输入向量的每个连续 64 个样本块执行就地 64 点 IFFT。  
  此函数处理从正负频率（-32 到 31）索引到全正（0 到 63）索引的移位。  
  此函数还将输出缩放为 1/64，以与 IFFT 函数保持一致。
     */
    void fft::inverse(std::vector<std::complex<double> > & data)
    {
        assert(data.size() % m_fft_length == 0);

        // 对每个 `m_fft_length` 样本运行 IFFT。
        for(int x = 0; x < data.size(); x += m_fft_length)
        {
            if(m_fft_length == 64)
            {
                for(int s = 0; s < 64; s++)
                {
                    memcpy(&m_fftw_in_inverse[s], &data[x + fft_map[s]], sizeof(std::complex<double>));
                }
            }
            else
            {
                memcpy(&m_fftw_in_inverse[0], &data[x], m_fft_length * sizeof(std::complex<double>));
            }

            fftw_execute(m_fftw_plan_inverse);
            memcpy(&data[x], m_fftw_out_inverse, m_fft_length * sizeof(std::complex<double>));
        }

        // 按 1/fft_length 进行缩放。
        for(int x = 0; x < data.size(); x++)
        {
            data[x] /= m_fft_length;
        }
    }
}


