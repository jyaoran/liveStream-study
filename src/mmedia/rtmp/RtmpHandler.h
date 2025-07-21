/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:16:44
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:57:40
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpHandler.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include "mmedia/base/MMediaHandler.h"
#include <string>

namespace liveStream
{
    namespace mm
    {
        /// @brief  作为扩展功能的实现头
        /// @note  继承自MMediaHandler，添加了Rtmp服务特有的功能
        /// @note  后面liveserver有菱形继承，暂时不做修改
        /// @note  此类为接口类，不应该被实例化
        class RtmpHandler : virtual public MMediaHandler // 后面liveserver有菱形继承
        {
        public:
            // 播放
            virtual bool OnPlay(const TcpConnectionPtr &conn, const std::string &session_name, const std::string &param) { return false; }
            // 推流
            virtual bool OnPublish(const TcpConnectionPtr &conn, const std::string &session_name, const std::string &param) { return false; }
            // 暂停
            virtual bool OnPause(const TcpConnectionPtr &conn, bool pause) { return false; }
            // 定位(快进等)≈
            virtual void OnSeek(const TcpConnectionPtr &conn, double time) {}
            // 告知可以开始publish
            virtual void OnPublishPrepare(const TcpConnectionPtr &conn) {}
        };
    } // namespace mm

} // namespace liveStream