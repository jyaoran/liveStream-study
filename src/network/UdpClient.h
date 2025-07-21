/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:13:19
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:00:33
 * @FilePath: /liveStream-study/src/network/UdpClient.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "network/base/InetAddress.h"
#include "network/net/EventLoop.h"
#include "network/net/UdpSocket.h"

namespace liveStream 
{
    namespace network 
    {
        using connectionCallback = std::function<void(const UdpSocketPtr&, bool)>;

        class UdpClient : public UdpSocket
        {
        public:
            UdpClient(EventLoop *loop, const InetAddress &server);
            virtual ~UdpClient();

            void connect();
            void setConnectionCallback(const connectionCallback &cb);
            void setConnectionCallback(connectionCallback &&cb);
            void connectInLoop();
            void send(std::list<UdpBufferNodePtr> &msgList);
            void send(const char *msg, size_t len);
            void onClose() override;

        private:
            bool m_connecting;
            InetAddress m_serverAddr;
            connectionCallback m_connCallback;
            struct sockaddr_in6 m_serveraddr;
            socklen_t m_addrlen{sizeof(sockaddr_in6)};
        };
    } // namespace network
} // namespace liveStream