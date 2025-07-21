/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-03 16:18:28
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:26:52
 * @FilePath: /liveStream-study/src/network/UdpClient.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "UdpClient.h"
#include "network/base/SocketOpt.h"
#include "base/easylogging++.h"

using namespace liveStream::network;

liveStream::network::UdpClient::UdpClient(EventLoop *loop, const InetAddress &server)
: UdpSocket(loop, -1, InetAddress(), server), m_serverAddr(server)
{
}

liveStream::network::UdpClient::~UdpClient()
{
}

void liveStream::network::UdpClient::connect()
{
    m_loop->runInLoop([this](){
        connectInLoop();
    });
}

void liveStream::network::UdpClient::setConnectionCallback(const connectionCallback &cb)
{
    m_connCallback = cb;
}

void liveStream::network::UdpClient::setConnectionCallback(connectionCallback &&cb)
{
    m_connCallback = std::move(cb);
}

void liveStream::network::UdpClient::connectInLoop()
{   
    // LOG(INFO) << "UdpClient::connectInLoop [" << m_serverAddr.toIpPort() << "]";
    m_loop->assertInLoopThread();

    m_fd = SocketOpt::createNonBlockUdpsocket(AF_INET);
    if (m_fd < 0)
    {
        onClose();
        return;
    }
    
    m_connecting = true;

    m_loop->addEvent(std::dynamic_pointer_cast<UdpClient>(shared_from_this()));

    SocketOpt opt(m_fd);
    opt.connect(m_serverAddr);
    m_serverAddr.getSockAddr((struct sockaddr *)&m_serveraddr);
    
    if (m_connCallback)
    {
        m_connCallback(std::dynamic_pointer_cast<UdpSocket>(shared_from_this()), true);
    }
    
}

void liveStream::network::UdpClient::send(std::list<UdpBufferNodePtr> &msgList)
{
    UdpSocket::udpSend(msgList);
}

void liveStream::network::UdpClient::send(const char *msg, size_t len)
{
    // LOG(INFO) << "UdpClient::send [" << m_serverAddr.toIpPort() << "] " << std::string(msg, len);
    UdpSocket::udpSend(msg, len, (sockaddr*)&m_serveraddr, m_addrlen);
}

void liveStream::network::UdpClient::onClose()
{
    if (m_connecting)
    {
        m_loop->removeEvent(std::dynamic_pointer_cast<UdpClient>(shared_from_this()));
        m_connecting = false;
        UdpSocket::onClose();
    }
    
}
