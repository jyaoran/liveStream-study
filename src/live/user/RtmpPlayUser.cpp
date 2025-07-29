/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-29 12:51:14
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 13:30:10
 * @FilePath: /liveStream-study/src/live/RtmpPlayUser.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "RtmpPlayUser.h"
#include "live/Stream.h"
#include "mmedia/rtmp/RtmpContext.h"
#include "live/base/LiveLog.h"
#include "base/TTime.h"

using namespace liveStream::live;
using namespace liveStream::mm;

RtmpPlayerUser::RtmpPlayerUser(const ConnectionPtr &ptr, const StreamPtr &stream, const SessionPtr &s)
:PlayerUser(ptr, stream, s)
{

}

/// @brief 发送多个帧
/// @return
bool RtmpPlayerUser::PostFrames()
{
    // 流没有准备好或者元数据都没有发送
    if(!stream_->Ready() || ! stream_->HasMedia())
    {
        Deactive();
        return false;
    }

    stream_->GetFrames(std::dynamic_pointer_cast<PlayerUser>(shared_from_this()));
    // 发送元数据  
    if(meta_)
    {
        auto ret = PushFrame(meta_, true);
        if(ret)
        {
            LIVE_INFO << "rtmp sent meta now: " << liveStream::base::TTime::getCurrentTimeInMS()
                    << " host:" << user_id_;
            
            meta_.reset();
        }
    }
    else if(audio_header_)
    {
        auto ret = PushFrame(audio_header_, true);
        if(ret)
        {
            LIVE_INFO << "rtmp sent audio_header_ now: " << liveStream::base::TTime::getCurrentTimeInMS()
                    << " host:" << user_id_;
            
            audio_header_.reset();
        }
    }
    else if(video_header_)
    {
        auto ret = PushFrame(video_header_, true);
        if(ret)
        {
            LIVE_INFO << "rtmp sent video_header_ now: " << liveStream::base::TTime::getCurrentTimeInMS()
                    << " host:" << user_id_;
            
            video_header_.reset();
        }
    }
    else if(!out_frames_.empty()) // 发送音视频数据
    {
        auto ret = PushFrame(out_frames_);
        if(ret)
        {
            out_frames_.clear();
        }
    }
    else
    {
        // 没有数据了
        Deactive();
    }
    return true;
}

/// @brief 每个类型客户的类型是固定的
/// @return 
UserType RtmpPlayerUser::GetUserType() const
{
    return UserType::kUserTypePlayerRtmp;
}

/// @brief 发送单帧数据
/// @param packet 
/// @param is_header 
/// @return 
bool RtmpPlayerUser::PushFrame(PacketPtr &packet, bool is_header)
{
    auto cx = connection_->getContext<RtmpContext>(kRtmpContext);
    if(!cx || !cx->Ready())
    {
        return false;
    }

    int64_t ts = 0;
    // 不是头修正时间戳
    if(!is_header) 
    {
        ts = time_corrector_.CorrectTimestamp(packet);
    }

    // 如果是头，就是fmt0，是第一个包
    // 构建rtmp包使用rtmp协议发送
    cx->BuildChunk(packet, ts, is_header);
    cx->Send();
    return true;
}

bool RtmpPlayerUser::PushFrame(std::vector<PacketPtr> &list)
{
    auto cx = connection_->getContext<RtmpContext>(kRtmpContext);
    if(!cx || !cx->Ready())
    {
        return false;
    }

    int64_t ts = 0;
    for(int i = 0; i < list.size(); ++i)
    {
        PacketPtr &packet = list[i];
        ts = time_corrector_.CorrectTimestamp(packet);
        cx->BuildChunk(packet, ts);
    }
    // 发送整个缓冲区
    cx->Send();
    return true;
}
