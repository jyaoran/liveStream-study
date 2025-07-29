/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-28 18:10:16
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-28 18:10:17
 * @FilePath: /liveStream-study/src/live/base/CodecHeader.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "mmedia/base/Packet.h"
#include <vector>
#include <memory> 
#include <cstdint>

namespace liveStream
{
    namespace live
    {
        using namespace liveStream::mm;

        class CodecHeader
        {
        public:
            CodecHeader();
            ~CodecHeader();
        
        public:
            PacketPtr Meta(int idx);  // 当前最新的包
            PacketPtr VideoHeader(int idx);
            PacketPtr AudioHeader(int idx);
            void SaveMeta(const PacketPtr &packet);
            void ParseMeta(const PacketPtr &packet);
            void SaveAudioHeader(const PacketPtr &packet);
            void SaveVideoHeader(const PacketPtr &packet);
            bool ParseCodecHeader(const PacketPtr &packet);
        
        private:
            PacketPtr video_header_;
            PacketPtr audio_header_;
            PacketPtr meta_;
            int meta_version_{0};
            int audio_version_{0};
            int video_version_{0};
            std::vector<PacketPtr> video_header_packets_; // 每个用户进度不同，包头也不一样
            std::vector<PacketPtr> audio_header_packets_;
            std::vector<PacketPtr> meta_packets_;
            int64_t start_timestamp_{0};
        };
    
        
    } // namespace live
    
} // namespace liveStream
