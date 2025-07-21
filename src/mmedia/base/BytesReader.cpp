/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:24:08
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:58:07
 * @FilePath: /liveStream-study/src/mmedia/base/BytesReader.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "BytesReader.h"
#include <netinet/in.h>
#include <cstring>
#include <cstdint>

using namespace liveStream::mm;

#include <cstring>
#include <cstdint>

using namespace liveStream::mm;

// 读取一个大端存储的uint64_t值，并将其转换为小端存储
uint64_t BytesReader::ReadUint64T(const char *data)
{
    uint64_t in = *((uint64_t *)data); // 将输入数据解释为uint64_t
    uint64_t res = __bswap_64(in);     // 使用__bswap_64将大端存储的uint64_t转换为小端
    return res;                        // 直接返回转换后的uint64_t值，而非double
}

// 读取一个大端存储的uint32_t值，并将其转换为小端存储
uint32_t BytesReader::ReadUint32T(const char *data)
{
    uint32_t *c = (uint32_t *)data; // 将输入数据解释为uint32_t
    return ntohl(*c);               // 使用ntohl将大端存储的uint32_t转换为小端
}

// 读取一个大端存储的24位（3字节）值，并将其转换为小端存储
uint32_t BytesReader::ReadUint24T(const char *data)
{
    unsigned char *c = (unsigned char *)data; // 将输入数据解释为unsigned char
    uint32_t val;
    val = (c[0] << 16) | (c[1] << 8) | c[2]; // 组装24位3字节
    // 注意：这里的输入数据是大端存储的，而输出结果val已经是小端存储
    return val;
}

// 读取缓存头部的两个字节值，作为一个大端存储的short int值，并将其转换为小端存储
uint16_t BytesReader::ReadUint16T(const char *data)
{
    uint16_t *c = (uint16_t *)data; // 将输入数据解释为uint16_t
    return ntohs(*c);               // 使用ntohs将大端存储的uint16_t转换为小端
}

// 读取缓存头部一个字节的值，返回该字节的值
uint8_t BytesReader::ReadUint8T(const char *data)
{
    return data[0]; // 直接返回输入数据的第一个字节
}
