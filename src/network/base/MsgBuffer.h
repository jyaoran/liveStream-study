/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-28 18:23:32
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 17:33:32
 * @FilePath: /liveStream-study/src/network/base/MsgBuffer.h
 * @Description: 该文件定义了一个用于发送和接收数据的内存缓冲区类 MsgBuffer
 * @
 * @版权所有 (c) 2025 jiangshan yaoranyaoran2015@outlook.com，保留所有权利。 
 *********************************************************************/
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <cstdint>

namespace liveStream
{
    namespace network
    {
        
        static constexpr size_t kBufferDefaultLength{2048}; // 默认缓冲区大小
        static constexpr char CRLF[]{"\r\n"}; // 表示换行符的常量字符串

        /**
         * @brief 这个类表示一个用于发送和接收数据的内存缓冲区。
         *
         */
        class MsgBuffer
        {
        public:
            /**
             * @brief 构造一个新的消息缓冲区实例。
             *
             * @param len 缓冲区的初始大小。
             */
            MsgBuffer(size_t len = kBufferDefaultLength);

            /**
             * @brief 获取缓冲区的开始位置，即可读数据的起始位置。
             *
             * @return const char* 可读数据的起始位置。
             */
            const char *Peek() const
            {
                return begin() + head_;
            }

            /**
             * @brief 获取缓冲区的结束位置，即新数据可以写入的位置。
             *
             * @return const char* 新数据可以写入的位置。
             */
            const char *BeginWrite() const
            {
                return begin() + tail_;
            }
            char *BeginWrite()
            {
                return begin() + tail_;
            }

            /**
             * @brief 从缓冲区中获取一个字节的值，但不移除该值。
             *
             * @return uint8_t 获取的字节值。
             */
            uint8_t PeekInt8() const
            {
                assert(ReadableBytes() >= 1);
                return *(static_cast<const uint8_t *>((void *)Peek()));
            }

            /**
             * @brief 从缓冲区中获取一个无符号短整型的值，但不移除该值。
             *
             * @return uint16_t 获取的无符号短整型值。
             */
            uint16_t PeekInt16() const;

            /**
             * @brief 从缓冲区中获取一个无符号整型的值，但不移除该值。
             *
             * @return uint32_t 获取的无符号整型值。
             */
            uint32_t PeekInt32() const;

            /**
             * @brief 从缓冲区中获取一个无符号长整型的值，但不移除该值。
             *
             * @return uint64_t 获取的无符号长整型值。
             */
            uint64_t PeekInt64() const;

            /**
             * @brief 从缓冲区中获取并移除指定长度的数据。
             *
             * @param len 要获取并移除的数据长度。
             * @return std::string 获取并移除的数据。
             */
            std::string Read(size_t len);

            /**
             * @brief 从缓冲区中获取并移除一个字节的值。
             *
             * @return uint8_t 获取并移除的字节值。
             */
            uint8_t ReadInt8();

            /**
             * @brief 从缓冲区中获取并移除一个无符号短整型的值。
             *
             * @return uint16_t 获取并移除的无符号短整型值。
             */
            uint16_t ReadInt16();

            /**
             * @brief 从缓冲区中获取并移除一个无符号整型的值。
             *
             * @return uint32_t 获取并移除的无符号整型值。
             */
            uint32_t ReadInt32();

            /**
             * @brief 从缓冲区中获取并移除一个无符号长整型的值。
             *
             * @return uint64_t 获取并移除的无符号长整型值。
             */
            uint64_t ReadInt64();

            /**
             * @brief 交换当前缓冲区与另一个缓冲区的内容。
             *
             * @param buf 要交换的另一个缓冲区。
             */
            void Swap(MsgBuffer &buf) noexcept;

            /**
             * @brief 返回缓冲区中可读数据的大小。
             *
             * @return size_t 可读数据的大小。
             */
            size_t ReadableBytes() const
            {
                return tail_ - head_;
            }

            /**
             * @brief 返回缓冲区中可写数据的空间大小。
             *
             * @return size_t 可写数据的空间大小。
             */
            size_t WritableBytes() const
            {
                return buffer_.size() - tail_;
            }

            /**
             * @brief 将新数据追加到缓冲区的末尾。
             *
             * @param buf 要追加的数据。
             */
            void Append(const MsgBuffer &buf);
            template <int N>
            void Append(const char (&buf)[N])
            {
                assert(strnlen(buf, N) == N - 1); // 确保字符串长度正确
                Append(buf, N - 1);
            }
            /**
             * @brief 将指定长度的数据追加到缓冲区的末尾。
             *
             * @param buf 要追加的数据。
             * @param len 数据的长度。
             */
            void Append(const char *buf, size_t len);
            /**
             * @brief 将字符串追加到缓冲区的末尾。
             *
             * @param buf 要追加的字符串。
             */
            void Append(const std::string &buf)
            {
                Append(buf.c_str(), buf.length());
            }

            /**
             * @brief 将一个字节的值追加到缓冲区的末尾。
             *
             * @param b 要追加的字节值。
             */
            void AppendInt8(const uint8_t b)
            {
                Append(static_cast<const char *>((void *)&b), 1);
            }

            /**
             * @brief 将一个无符号短整型的值追加到缓冲区的末尾。
             *
             * @param s 要追加的无符号短整型值。
             */
            void AppendInt16(const uint16_t s);

            /**
             * @brief 将一个无符号整型的值追加到缓冲区的末尾。
             *
             * @param i 要追加的无符号整型值。
             */
            void AppendInt32(const uint32_t i);

            /**
             * @brief 将一个无符号长整型的值追加到缓冲区的末尾。
             *
             * @param l 要追加的无符号长整型值。
             */
            void AppendInt64(const uint64_t l);

            /**
             * @brief 将新数据添加到缓冲区的开头。
             *
             * @param buf 要添加的数据。
             * @param len 数据的长度。
             */
            void AddInFront(const char *buf, size_t len);

            /**
             * @brief 将一个字节的值添加到缓冲区的开头。
             *
             * @param b 要添加的字节值。
             */
            void AddInFrontInt8(const uint8_t b)
            {
                AddInFront(static_cast<const char *>((void *)&b), 1);
            }

            /**
             * @brief 将一个无符号短整型的值添加到缓冲区的开头。
             *
             * @param s 要添加的无符号短整型值。
             */
            void AddInFrontInt16(const uint16_t s);

            /**
             * @brief 将一个无符号整型的值添加到缓冲区的开头。
             *
             * @param i 要添加的无符号整型值。
             */
            void AddInFrontInt32(const uint32_t i);

            /**
             * @brief 将一个无符号长整型的值添加到缓冲区的开头。
             *
             * @param l 要添加的无符号长整型值。
             */
            void AddInFrontInt64(const uint64_t l);

            /**
             * @brief 清空缓冲区中的所有数据。
             *
             */
            void RetrieveAll();

            /**
             * @brief 从缓冲区中移除指定长度的数据。
             *
             * @param len 要移除的数据长度。
             */
            void Retrieve(size_t len);

            /**
             * @brief 从文件描述符读取数据并将其放入缓冲区。
             *
             * @param fd 文件描述符，通常是套接字。
             * @param retErrno 读取时的错误代码。
             * @return ssize_t 从文件描述符读取的字节数。发生错误时返回-1。
             */
            ssize_t ReadFd(int fd, int *retErrno);

            /**
             * @brief 移除缓冲区中指定位置之前的所有数据。
             *
             * @param end 指定的位置。
             */
            void RetrieveUntil(const char *end)
            {
                assert(Peek() <= end); // 确保end在可读范围内
                assert(end <= BeginWrite()); // 确保end在可写范围之前
                Retrieve(end - Peek()); // 移除到指定位置的数据
            }

            /**
             * @brief 查找缓冲区中CRLF（回车换行符）的位置。
             *
             * @return const char* CRLF的位置，如果没有找到则返回NULL。
             */
            const char *FindCRLF() const
            {
                const char *crlf = std::search(Peek(), BeginWrite(), CRLF, CRLF + 2); // 查找CRLF
                return crlf == BeginWrite() ? NULL : crlf; // 如果找到末尾则返回NULL
            }

            /**
             * @brief 确保缓冲区有足够的空间来写入指定长度的数据。
             *
             * @param len 需要的可写空间长度。
             */
            void EnsureWritableBytes(size_t len);

            /**
             * @brief 当新数据写入缓冲区后，将写指针向前移动。
             *
             * @param len 写入的数据长度。
             */
            void HasWritten(size_t len)
            {
                assert(len <= WritableBytes()); // 确保有足够的可写空间
                tail_ += len; // 移动写指针
            }

            /**
             * @brief 将写指针向后移动以移除缓冲区末尾的数据。
             *
             * @param offset 要移除的数据长度。
             */
            void Unwrite(size_t offset)
            {
                assert(ReadableBytes() >= offset); // 确保有足够的可读数据
                tail_ -= offset; // 移动写指针
            }

            /**
             * @brief 访问缓冲区中的某个字节，不移除该字节。
             *
             * @param offset 要访问的字节位置。
             * @return const char& 访问的字节。
             */
            const char &operator[](size_t offset) const
            {
                assert(ReadableBytes() >= offset); // 确保offset在可读范围内
                return Peek()[offset]; // 返回指定位置的字节
            }
            /**
             * @brief 访问缓冲区中的某个字节，不移除该字节。
             *
             * @param offset 要访问的字节位置。
             * @return char& 访问的字节。
             */
            char &operator[](size_t offset)
            {
                assert(ReadableBytes() >= offset); // 确保offset在可读范围内
                return begin()[head_ + offset]; // 返回指定位置的字节
            }

        private:
            size_t head_; // 可读数据的起始位置
            size_t initCap_; // 初始容量
            std::vector<char> buffer_; // 内部缓冲区
            size_t tail_; // 可写数据的起始位置

            /**
             * @brief 获取缓冲区的开始地址。
             *
             * @return const char* 缓冲区的开始地址。
             */
            const char *begin() const
            {
                return &buffer_[0];
            }
            /**
             * @brief 获取缓冲区的开始地址。
             *
             * @return char* 缓冲区的开始地址。
             */
            char *begin()
            {
                return &buffer_[0];
            }
        };

        /**
         * @brief 交换两个MsgBuffer对象的内容。
         *
         * @param one 第一个MsgBuffer对象。
         * @param two 第二个MsgBuffer对象。
         */
        inline void swap(MsgBuffer &one, MsgBuffer &two) noexcept
        {
            one.Swap(two);
        }

        /**
         * @brief 将一个64位无符号整型的值转换为网络字节序（大端序）。
         *
         * @param n 要转换的64位无符号整型值。
         * @return uint64_t 转换后的网络字节序值。
         */
        inline uint64_t hton64(uint64_t n)
        {
            static const int one = 1;
            static const char sig = *(char *)&one; // 检查机器字节序
            if (sig == 0)
                return n;  // 如果是大端序，直接返回输入值
            char *ptr = reinterpret_cast<char *>(&n);
            std::reverse(ptr, ptr + sizeof(uint64_t)); // 反转字节序
            return n;
        }

        /**
         * @brief 将一个64位无符号整型的值从网络字节序（大端序）转换为主机字节序。
         *
         * @param n 要转换的64位无符号整型值。
         * @return uint64_t 转换后的主机字节序值。
         */
        inline uint64_t ntoh64(uint64_t n)
        {
            return hton64(n); // 使用hton64实现转换
        }        
    }  // namespace network


}
namespace std
{
    /**
     * @brief 重载std::swap以支持MsgBuffer对象的交换。
     *
     * @param one 第一个MsgBuffer对象。
     * @param two 第二个MsgBuffer对象。
     */
    template <>
    inline void swap(liveStream::network::MsgBuffer &one, liveStream::network::MsgBuffer &two) noexcept
    {
        one.Swap(two);
    } 
}
