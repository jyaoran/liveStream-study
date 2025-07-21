/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-01 15:30:18
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:24:24
 * @FilePath: /liveStream-study/src/network/net/UdpSocket.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "network/base/InetAddress.h"
#include "network/base/MsgBuffer.h"
#include "network/net/EventLoop.h"
#include "network/net/Connection.h"

#include <memory>
#include <functional>
#include <list>

namespace liveStream 
{
    namespace network 
    {
        class UdpSocket;
        using UdpSocketPtr = std::shared_ptr<UdpSocket>;

        using UdpSocketMessageCallback = std::function<void(const UdpSocketPtr &, const InetAddress &, MsgBuffer &)>;
        using UdpSocketWriteCompleteCallback = std::function<void(const UdpSocketPtr &)>;
        using UdpSocketCloseConnectionCallback = std::function<void(const UdpSocketPtr &)>;
        using UdpSocketTimeoutCallback = std::function<void(const UdpSocketPtr &)>;

        struct UdpTimeoutEntry;

        struct UdpBufferNode : public BufferNode
        {
            using ptr = std::shared_ptr<BufferNode>;
            
            UdpBufferNode(void *buf, size_t size, struct sockaddr *addr, socklen_t addrlen)
            : BufferNode(buf, size), m_sockaddr(addr), m_sockaddrLen(addrlen)
            {}
 
            struct sockaddr *m_sockaddr{nullptr};
            socklen_t m_sockaddrLen{0};
        };
        
        using UdpBufferNodePtr = std::shared_ptr<UdpBufferNode>;

        class UdpSocket : public Connection
        {
        public:
            using ptr = std::shared_ptr<UdpSocket>;

            UdpSocket(EventLoop *loop, int sockfd,
            const InetAddress &localAddr, const InetAddress &peerAddr);
            ~UdpSocket();

            void onTimeout();
            void setCloseConnectionCallback(const UdpSocketCloseConnectionCallback &cb);
            void setCloseConnectionCallback(UdpSocketCloseConnectionCallback &&cb);
            void setOnMessageCallback(const UdpSocketMessageCallback &cb);
            void setOnMessageCallback(UdpSocketMessageCallback &&cb);
            void setWriteCompleteCallback(const UdpSocketWriteCompleteCallback &cb);
            void setWriteCompleteCallback(UdpSocketWriteCompleteCallback &&cb);
            void setTimeoutCallback(const UdpSocketTimeoutCallback &cb);
            void setTimeoutCallback(UdpSocketTimeoutCallback &&cb);
            void enableCheckIdleTimeout(int32_t maxTime);

            void udpSend(std::list<UdpBufferNodePtr> &msgList);
            void udpSend(const char *data, size_t len, struct sockaddr *addr, socklen_t addrlen);
            void udpSendInLoop(std::list<UdpBufferNodePtr> &msgList);
            void udpSendInLoop(const char *data, size_t len, struct sockaddr *addr, socklen_t addrlen);

            void onRead() override;
            void onWrite() override;
            void onClose() override;
            void onError(const std::string &msg) override;
            void forceClose() override;

        private:
            void extendLifetime();

            std::list<UdpBufferNodePtr> m_bufferList;
            bool m_closed{false};
            int32_t m_maxIdleSeconds{30};
            std::weak_ptr<UdpTimeoutEntry> m_timeoutEntry;
            int32_t m_messageBufferSize{65535};
            MsgBuffer m_messageBuffer;
            UdpSocketTimeoutCallback m_timeoutCallback;
            UdpSocketMessageCallback m_messageCallback;
            UdpSocketWriteCompleteCallback m_writeCompleteCallback;
            UdpSocketCloseConnectionCallback m_closeConnectionCallback;

        };

        struct UdpTimeoutEntry
        {
        public:
            UdpTimeoutEntry(const UdpSocketPtr &conn) : m_conn(conn) { }
            ~UdpTimeoutEntry() 
            { 
                auto conn = m_conn.lock();
                if (conn)
                {
                    conn->onTimeout();
                }
            }
        private:
            std::weak_ptr<UdpSocket> m_conn;
        };
    } // namespace network
} // namespace liveStream