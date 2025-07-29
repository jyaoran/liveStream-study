/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-27 16:19:39
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-27 16:21:44
 * @FilePath: /liveStream-study/src/live/base/CodecUtils.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "mmedia/base/Packet.h"

namespace liveStream
{ 
    namespace live
    {
        using namespace liveStream::mm;

        class CodecUtils
        {
        public:
            static bool IsCodecHeader(const PacketPtr &packet);
            static bool IsKeyFrame(const PacketPtr &packet);
        };
    
        
    } // namespace live
    
} // namespace liveStream