/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:22:27
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:43:12
 * @FilePath: /liveStream-study/src/mmedia/base/BytesWriter.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "BytesWriter.h"
#include <netinet/in.h>
#include <cstring>

using namespace liveStream::mm;

/// 将一个uint32_t类型的无符号整数写入字符数组，并转换为网络字节序
/// @param buf 指向目标字符数组的指针
/// @param val 要写入的uint32_t类型的无符号整数
/// @return 写入的字节数
int BytesWriter::WriteUint32T(char *buf, uint32_t val)
{
    // 将值转换为网络字节序（大端）
    val = htonl(val);
    // 将转换后的值复制到目标缓冲区
    memcpy(buf, &val, sizeof(uint32_t));
    // 返回写入的字节数
    return sizeof(uint32_t);
}

/// 将一个24位的无符号整数写入字符数组，并转换为网络字节序
/// @param buf 指向目标字符数组的指针
/// @param val 要写入的24位无符号整数，以uint32_t类型表示
/// @return 写入的字节数
int BytesWriter::WriteUint24T(char *buf, uint32_t val)
{
    // 将值转换为网络字节序（大端）
    val = htonl(val);
    // 获取转换后值的指针，并跳过最高8位以只写入24位
    char *ptr = (char *)&val + 1;
    // 将24位值复制到目标缓冲区
    memcpy(buf, ptr, 3);
    // 返回写入的字节数
    return 3;
}

/// 将一个uint16_t类型的无符号整数写入字符数组，并转换为网络字节序
/// @param buf 指向目标字符数组的指针
/// @param val 要写入的uint16_t类型的无符号整数
/// @return 写入的字节数
int BytesWriter::WriteUint16T(char *buf, uint16_t val)
{
    // 将值转换为网络字节序（大端）
    val = htons(val);
    // 将转换后的值复制到目标缓冲区
    memcpy(buf, &val, sizeof(uint16_t));
    // 返回写入的字节数
    return sizeof(uint16_t);
}

/// 将一个uint8_t类型的无符号整数写入字符数组
/// @param buf 指向目标字符数组的指针
/// @param val 要写入的uint8_t类型的无符号整数
/// @return 写入的字节数
int BytesWriter::WriteUint8T(char *buf, uint8_t val)
{
    // 直接将uint8_t类型的值写入目标缓冲区
    char *data = (char *)&val;
    buf[0] = data[0];
    // 返回写入的字节数

    return 1;
}
