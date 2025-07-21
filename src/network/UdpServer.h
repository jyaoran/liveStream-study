/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:13:36
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-03 15:55:02
 * @FilePath: /liveStream-study/src/network/UdpServer.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "network/base/InetAddress.h"
#include "network/base/SocketOpt.h"
#include "network/net/EventLoop.h"
#include "network/net/UdpSocket.h"

#include <functional>

namespace liveStream 
{
    namespace network 
    {
        class UdpServer : public UdpSocket
        {
        public:
            UdpServer(EventLoop* loop, const InetAddress& server);
            virtual ~UdpServer();
        
            void Start();
            void Stop();

        private:
            void open();

            InetAddress m_serverAddr;
        };
    } // namespace network
} // namespace liveStream