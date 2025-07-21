/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-03 15:50:25
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:26:11
 * @FilePath: /liveStream-study/src/network/UdpServer.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/UdpServer.h"
#include "base/easylogging++.h"
#include "UdpServer.h"

liveStream::network::UdpServer::UdpServer(EventLoop *loop,  const InetAddress &server)
: UdpSocket(loop, -1, server, InetAddress()), m_serverAddr(server)
{
    
}

liveStream::network::UdpServer::~UdpServer()
{
    Stop();
}

void liveStream::network::UdpServer::Start()
{
    m_loop->runInLoop([this]() {
        open();
    });
}

void liveStream::network::UdpServer::Stop()
{
    m_loop->runInLoop([this]() {
        m_loop->removeEvent(std::dynamic_pointer_cast<UdpServer>(shared_from_this()));
        onClose();
    });
}

void liveStream::network::UdpServer::open()
{
    // LOG(INFO) << "UdpServer::open [" << m_serverAddr.toIpPort() << "]";
    m_loop->assertInLoopThread();

    m_fd = SocketOpt::createNonBlockUdpsocket(AF_INET);
    if (m_fd < 0)
    {
        onClose();
        return;
    }

    m_loop->addEvent(std::dynamic_pointer_cast<UdpServer>(shared_from_this()));
    SocketOpt opt(m_fd);
    opt.bindAddress(m_serverAddr);
    opt.setReuseAddr(true);
    opt.setReusePort(true);
    
}