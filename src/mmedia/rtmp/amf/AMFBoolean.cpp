/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:15:27
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 14:21:24
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFBoolean.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "AMFBoolean.h"

#include "mmedia/base/MMediaLog.h"
#include "mmedia/base/BytesWriter.h"
#include "mmedia/base/BytesReader.h"


using namespace liveStream::mm;

AMFBoolean::AMFBoolean(const std::string &name)
:AMFAny(name)
{
}

AMFBoolean::AMFBoolean()
{
}

AMFBoolean::~AMFBoolean()
{
}

int AMFBoolean::Decode(const char *data, int size, bool has)
{
    if(size >= 1)
    {
        b_ = *data != 0 ? true : false;
        return 1;
    }
    return -1;
}

bool AMFBoolean::IsBoolean()
{
    return true;  // 是Boolean类，实现父类的判断函数，动态绑定就能执指向
}

bool AMFBoolean::Boolean()
{
    return b_;
}

void AMFBoolean::Dump() const
{
    RTMP_TRACE << "Boolean: " << b_;
}