/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-29 12:46:20
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 12:49:28
 * @FilePath: /liveStream-study/src/live/PlayerUser.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "PlayerUser.h"

using namespace liveStream::live;

PlayerUser::PlayerUser(const ConnectionPtr &ptr, const StreamPtr &stream, const SessionPtr &s)
    : User(ptr, stream, s)
{

}

PacketPtr PlayerUser::Meta() const
{
    return meta_;
}

PacketPtr PlayerUser::VideoHeader() const
{
    return video_header_;
}

PacketPtr PlayerUser::AudioHeader() const
{
    return audio_header_;
}

void PlayerUser::ClearMeta() // 发送之后清除meta，就知道是已经发送过了
{
    meta_.reset();
}

void PlayerUser::ClearAudioHeader()
{
    audio_header_.reset();
}

void PlayerUser::ClearVideoHeader()
{
    video_header_.reset();
}

// 这个函数留给具体的协议用户类发送实现
// rtmp , flv, hls等具 es() = 0;
TimeCorrector &PlayerUser::GetTimeCorrector() // 调整正确的时间戳
{
    return time_corrector_;
}