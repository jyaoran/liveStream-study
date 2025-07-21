/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:21:00
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:58:25
 * @FilePath: /liveStream-study/src/mmedia/base/BytesWriter.h
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
        /// @brief 将根据主机字节序长度转换网络字节序字节长度，并写入buf中。
        class BytesWriter
        {
        public:
            BytesWriter() = default;
            ~BytesWriter() = default;

            static int WriteUint32T(char *buf, uint32_t val);
            static int WriteUint24T(char *buf, uint32_t val);
            static int WriteUint16T(char *buf, uint16_t val);
            static int WriteUint8T(char *buf, uint8_t val);
        };
    }
}