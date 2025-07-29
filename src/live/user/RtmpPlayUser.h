/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-28 22:18:58
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 12:53:49
 * @FilePath: /liveStream-study/src/live/RtmpPlayUser.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "PlayerUser.h"

namespace liveStream
{
    namespace live
    {
        class RtmpPlayerUser : public PlayerUser
        {
        public:
            // using PlayerUser::User;   // 使用c++11的委托构造
            explicit RtmpPlayerUser(const ConnectionPtr &ptr, const StreamPtr &stream, const SessionPtr &s);
        
        public:
            bool PostFrames();
            UserType GetUserType() const;
        
        private:
            using User::SetUserType; // 同样使用父类的
            // 发送头和meta
            bool PushFrame(PacketPtr &packet, bool is_header);
            // 发送数据帧
            bool PushFrame(std::vector<PacketPtr> &list);

            
        };
        
    
        
    } // namespace live
    
} // namespace liveStream