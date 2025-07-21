/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-01 16:44:10
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:27:12
 * @FilePath: /liveStream-study/src/network/net/UdpSocket.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "UdpSocket.h"
#include "base/easylogging++.h"

using namespace liveStream::network;

UdpSocket::UdpSocket(EventLoop *loop, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr)
    : Connection(loop, sockfd, localAddr, peerAddr),
      m_messageBuffer(m_messageBufferSize)
{
}

UdpSocket::~UdpSocket()
{
}

void UdpSocket::onTimeout()
{
    onClose();
}

void liveStream::network::UdpSocket::setCloseConnectionCallback(const UdpSocketCloseConnectionCallback &cb)
{
    m_closeConnectionCallback = cb;
}

void liveStream::network::UdpSocket::setCloseConnectionCallback(UdpSocketCloseConnectionCallback &&cb)
{
    m_closeConnectionCallback = std::move(cb);
}

void liveStream::network::UdpSocket::setOnMessageCallback(const UdpSocketMessageCallback &cb)
{
    m_messageCallback = cb;
}

void liveStream::network::UdpSocket::setOnMessageCallback(UdpSocketMessageCallback &&cb)
{
    m_messageCallback = std::move(cb);
}

void liveStream::network::UdpSocket::setWriteCompleteCallback(const UdpSocketWriteCompleteCallback &cb)
{
    m_writeCompleteCallback = cb;
}

void liveStream::network::UdpSocket::setWriteCompleteCallback(UdpSocketWriteCompleteCallback &&cb)
{
    m_writeCompleteCallback = std::move(cb);
}

void liveStream::network::UdpSocket::setTimeoutCallback(const UdpSocketTimeoutCallback &cb)
{
    m_timeoutCallback = cb;
}

void liveStream::network::UdpSocket::setTimeoutCallback(UdpSocketTimeoutCallback &&cb)
{
    m_timeoutCallback = std::move(cb);
}

void liveStream::network::UdpSocket::enableCheckIdleTimeout(int32_t maxTime)
{
    auto tp =
        std::make_shared<UdpTimeoutEntry>(std::dynamic_pointer_cast<UdpSocket>(shared_from_this()));
    m_maxIdleSeconds = maxTime;
    m_timeoutEntry = tp;

    m_loop->insertEntry(tp, m_maxIdleSeconds);
}

void liveStream::network::UdpSocket::udpSend(std::list<UdpBufferNodePtr> &msgList)
{
    m_loop->runInLoop([this, &msgList]()
                      { udpSendInLoop(msgList); });
}

void liveStream::network::UdpSocket::udpSend(const char *data, size_t len, sockaddr *addr, socklen_t addrlen)
{
    m_loop->runInLoop([this, data, len, addr, addrlen]()
                      { udpSendInLoop(data, len, addr, addrlen); });
}

void liveStream::network::UdpSocket::udpSendInLoop(std::list<UdpBufferNodePtr> &msgList)
{
    for (auto &msg : msgList)
    {
        m_bufferList.emplace_back(msg);
    }

    if (!m_bufferList.empty())
    {
        enableWrite(true);
    }
}

void UdpSocket::udpSendInLoop(const char *data, size_t len, sockaddr *addr, socklen_t addrlen)
{
    if (m_bufferList.empty())
    {
        auto ret = ::sendto(m_fd, data, len, 0, addr, addrlen);
        if (ret > 0)
        {
            return;
        }
    }

    auto node = std::make_shared<UdpBufferNode>(const_cast<char*>(data), len, addr, addrlen);
    m_bufferList.emplace_back(node);
    enableWrite(true);
}

void liveStream::network::UdpSocket::onRead()
{
    if (m_closed)
    {
        LOG(INFO) << "UdpSocket::onRead() m_closed";
        return;
    }

    extendLifetime();
    while (true)
    {
        struct sockaddr_in6 src_addr;
        socklen_t addrlen = sizeof(src_addr);
        
        int nread = ::recvfrom(m_fd, m_messageBuffer.BeginWrite(), 
                               m_messageBufferSize, 0, (struct sockaddr *)&src_addr, &addrlen);
        if (nread > 0)
        {
            InetAddress peerAddr;
            m_messageBuffer.HasWritten(nread);

            if (src_addr.sin6_family == AF_INET)
            {
                char ip[INET_ADDRSTRLEN] = {0};
                struct sockaddr_in *addr_v4 = (struct sockaddr_in *)&src_addr;
                ::inet_ntop(AF_INET, &(addr_v4->sin_addr), ip, INET_ADDRSTRLEN);

                peerAddr.setAddr(ip);
                peerAddr.setPort(ntohs(addr_v4->sin_port));
                peerAddr.setIsIpv6(false);
            }
            else if (src_addr.sin6_family == AF_INET6)
            {
                char ip[INET6_ADDRSTRLEN] = {0};
                ::inet_ntop(AF_INET6, &(src_addr.sin6_addr), ip, INET6_ADDRSTRLEN);

                peerAddr.setAddr(ip);
                peerAddr.setPort(ntohs(src_addr.sin6_port));
                peerAddr.setIsIpv6(true);
            }
            
            if (m_messageCallback)
            {
                m_messageCallback(std::dynamic_pointer_cast<UdpSocket>(shared_from_this()), 
                                    peerAddr, m_messageBuffer);
            }
            m_messageBuffer.RetrieveAll();
        }
        else if (nread < 0)
        {
            if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
            {
                onClose();
                return;
            }
            break;
        }
    }
}

void liveStream::network::UdpSocket::onWrite()
{
    if (m_closed)
    {
        return;
    }

    extendLifetime();

    while (true)
    {
        if (!m_bufferList.empty())
        {
            auto node = m_bufferList.front();
            auto ret = ::sendto(m_fd, node->m_addr, node->m_size, 0, node->m_sockaddr, node->m_sockaddrLen);
            if (ret > 0)
            {
                m_bufferList.pop_front();    
            }
            else if (ret < 0)
            {
                if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    onClose();
                    return;
                }
                break;
            }
        }

        if (m_bufferList.empty())
        {
            if (m_writeCompleteCallback)
                m_writeCompleteCallback(std::dynamic_pointer_cast<UdpSocket>(shared_from_this()));
        }
    }
}

void liveStream::network::UdpSocket::onClose()
{
    if (!m_closed)
    {
        m_closed = true;
        if (m_closeConnectionCallback)
        {
            m_closeConnectionCallback(std::dynamic_pointer_cast<UdpSocket>(shared_from_this()));
        }
        Event::close();
    }
    
}

void liveStream::network::UdpSocket::onError(const std::string &msg)
{
    LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << "; error : " << msg;
    onClose();
}

void liveStream::network::UdpSocket::forceClose()
{
    m_loop->runInLoop([this]() {
        onClose();
    });
}

void UdpSocket::extendLifetime()
{
    auto tp = m_timeoutEntry.lock();
    if (tp)
    {
        m_loop->insertEntry(tp, m_maxIdleSeconds);
    }
}