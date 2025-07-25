/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:28:22
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 16:14:53
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpClient.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include "network/TcpClient.h"
#include "network/net/EventLoop.h"
#include "network/base/InetAddress.h"
#include "mmedia/rtmp/RtmpHandler.h"
#include <memory>
#include <functional>

namespace liveStream
{
    namespace mm
    {
        using namespace liveStream::network;
        class RtmpClient
        {
        public:
            RtmpClient(EventLoop *loop, RtmpHandler *handler);
            ~RtmpClient();

        public:
 
            void SetCloseCallback(const CloseConnectionCallback &cb);
            void SetCloseCallback(CloseConnectionCallback &&cb);

            void Play(const std::string &url); // 播放（拉流）
            void Publish(const std::string &url); // 推流
            void Send(PacketPtr &&data); // 推流发送


        private:
            void OnWriteComplete(const TcpConnectionPtr &conn); // 写完成回调
            void OnConnection(const TcpConnectionPtr &conn, bool connected); // 连接回调
            void OnMessage(const TcpConnectionPtr &conn, MsgBuffer &buf);
            
            bool ParseUrl(const std::string &url);
            void CreateTcpClient();

            EventLoop *loop_{nullptr};
            InetAddress addr_;  // 服务端地址
            RtmpHandler *handler_{nullptr};
            TcpClientPtr tcp_client_;   // 使用tcpclient进行数据的连接接受发送
            std::string url_;
            bool is_player_{false};
            CloseConnectionCallback close_cb_; 
        };
    } // namespace mm
    
} // namespace liveStream