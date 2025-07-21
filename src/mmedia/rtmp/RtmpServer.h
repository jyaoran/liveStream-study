/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:26:40
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 16:30:24
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpServer.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include "mmedia/rtmp/RtmpHandler.h"
#include "network/net/TcpConnection.h"
#include "network/TcpServer.h"

namespace liveStream
{
    namespace mm
    {
        using namespace liveStream::network;

        class RtmpServer : public TcpServer
        {
        public:
            RtmpServer(EventLoop *loop, const InetAddress& local, RtmpHandler* handler = nullptr);
            ~RtmpServer();
        
        public:
            void Start() override;
            void Stop() override;

        private:
            void OnNewConnection(const TcpConnectionPtr &conn); 
            void OnConnectionDestroy(const TcpConnectionPtr &conn);
            void OnMessage(const TcpConnectionPtr &conn, MsgBuffer &buf);
            void OnWriteComplete(const ConnectionPtr &conn);
            void OnActive(const ConnectionPtr &conn);

            RtmpHandler *rtmp_handler_{nullptr};
        };
        
        
        
    } // namespace mm
    
} // namespace liveStream