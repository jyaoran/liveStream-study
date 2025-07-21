/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:23:54
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:58:12
 * @FilePath: /liveStream-study/src/mmedia/base/BytesReader.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once
#include <stdint.h>

namespace liveStream
{
    namespace mm
    {
        // BytesReader 类提供了一组静态方法用于从字符数组中读取不同长度的无符号整数。
        class BytesReader
        {
        public:
            // 默认构造函数
            BytesReader() = default;

            // 默认析构函数
            ~BytesReader() = default;

            // 从字符数组中读取一个uint64_t类型的无符号整数
            // @param data 指向包含数据的字符数组的指针
            // @return 读取的uint64_t类型的无符号整数
            static uint64_t ReadUint64T(const char *data);

            // 从字符数组中读取一个uint32_t类型的无符号整数
            // @param data 指向包含数据的字符数组的指针
            // @return 读取的uint32_t类型的无符号整数
            static uint32_t ReadUint32T(const char *data);

            // 从字符数组中读取一个24位的无符号整数，并返回为uint32_t类型
            // @param data 指向包含数据的字符数组的指针
            // @return 读取的24位无符号整数，以uint32_t类型返回
            static uint32_t ReadUint24T(const char *data);

            // 从字符数组中读取一个uint16_t类型的无符号整数
            // @param data 指向包含数据的字符数组的指针
            // @return 读取的uint16_t类型的无符号整数
            static uint16_t ReadUint16T(const char *data);

            // 从字符数组中读取一个uint8_t类型的无符号整数
            // @param data 指向包含数据的字符数组的指针
            // @return 读取的uint8_t类型的无符号整数
            static uint8_t ReadUint8T(const char *data);
        };
    }
}
