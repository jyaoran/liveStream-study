/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 15:14:01
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 15:16:35
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFNull.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "AMFNull.h"
#include "mmedia/base/MMediaLog.h"
#include "mmedia/base/BytesWriter.h"
#include "mmedia/base/BytesReader.h"


using namespace liveStream::mm;
AMFNull::AMFNull(const std::string &name)
:AMFAny(name)
{
}

AMFNull::AMFNull()
{
}

AMFNull::~AMFNull()
{
}

/// @brief  NULL不用解析，直接返回0表示解析正确
/// @param data 
/// @param size 
/// @param has 
/// @return 
int AMFNull::Decode(const char *data, int size, bool has)
{
    return 0;
}

bool AMFNull::IsNull()
{
    return true;  // 是Null类，实现父类的判断函数，动态绑定就能执指向
}

void AMFNull::Dump() const
{
    RTMP_TRACE << "Null: ";
}
