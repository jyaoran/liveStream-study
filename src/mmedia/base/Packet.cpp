/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:19:12
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:59:21
 * @FilePath: /liveStream-study/src/mmedia/base/Packet.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "Packet.h"

using namespace liveStream::mm;

/// @brief
/// @param size 需要的保存的数据块的大小
/// @return
Packet::ptr Packet::NewPacket(int32_t size)
{
    // 类本身也占空间
    auto block_size = size + sizeof(Packet);

    Packet *packet = (Packet *)new char[block_size];
    memset((void *)packet, 0x00, block_size);

    packet->index_ = -1;
    packet->type_ = kPacketTypeUnKnowed;
    // 这里的capacity_是指数据块的大小
    packet->capacity_ = size;
    // 将ext_指针置空
    packet->ext_.reset();

    // 返回带自定义删除器的智能指针
    return Packet::ptr(packet, [](Packet *p)
                       { delete[] (char *)p; });
}
