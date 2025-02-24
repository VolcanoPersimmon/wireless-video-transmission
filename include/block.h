/*! \file block.h
 *  \brief Base class for receiver chain blocks.
 *
*  该类初始化输入和输出缓冲区
*  并包含纯虚函数 work。

 *
 */

#ifndef BLOCK_H
#define BLOCK_H

/*! \def BUFFER_MAX
 *  \brief 输入和输出缓冲区的最大缓冲区大小
 *
*  从技术上讲，每个块使用
* ~~~{.cpp}
* std::vector::reserve(size_type n)
* ~~~
* 函数来预留 BUFFER_MAX * sizeof(size_type) 字节的空间

 */

#define BUFFER_MAX 65536

#include <vector>
#include <string>

namespace fun
{
    /*!
     * \brief The block_base class.
     *
    *  该类允许接收链使用通用指针
    *     引用接收链中的每个块，即使它们是不同的模板。

     */
    class block_base
    {
    public:

        /*!
         * \brief block_base constructor
         * \param block_name the name of the block as a std::string
         */
        block_base(std::string block_name) :
            name(block_name)
        {
        }

        /*!
         * \brief The main work function.
         *
         * The work function is a pure virtual function.
         */
        virtual void work() = 0;

        /*!
         * \brief the public name of the block
         */
        std::string name;
    };

    /*!
     * \brief The block class template.
     *
     *  该类是接收链中块的模板类。
*  该类包含输入和输出缓冲区。

     */
    template<typename I, typename O>
    class block : public block_base
    {
    public:

        /*!
         * \brief constructor
         *
         * Reserves BUFFER_MAX * sizeof(size_type) for the input and output buffers.
         * \param block_name the name of the block as a std::string
         */
        block(std::string block_name) :
            block_base(block_name)
        {
            input_buffer.reserve(BUFFER_MAX);
            output_buffer.reserve(BUFFER_MAX);
        }

        /*!
         * \brief The main work function.
         *
         *  该函数是纯虚函数。
            *  该函数必须消耗 input_buffer 并填充 output_buffer。
            *  在此过程中，应确保相应地调整 output_buffer 的大小，
            *  并保留 input_buffer 中可能在下一次调用时需要的任何项目。

         */
        virtual void work() = 0;

        /*!
         * \brief input_buffer contains new input items to be consumed
         *
         *  包含类型为 I 的新输入项。对于每次调用 work，
        *  传递给 input_buffer 的项目数量没有保证，
        *  只要它必须小于 #BUFFER_MAX。

         */
        std::vector<I> input_buffer;

        /*!
         * \brief output_buffer is where the output items of the block should be placed
         *
         *  对于每次调用，块必须生成的输出项数量没有限制，
        *  只要它必须小于 #BUFFER_MAX。

         */
        std::vector<O> output_buffer;
    };

}

#endif // BLOCK_H
