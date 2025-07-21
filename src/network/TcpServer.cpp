/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-30 16:44:53
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 16:39:37
 * @FilePath: /liveStream-study/src/network/TcpServer.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "TcpServer.h"
#include "base/easylogging++.h"


using namespace liveStream::network;

liveStream::network::TcpServer::TcpServer(EventLoop *loop, const InetAddress &listenAddr)
: m_loop(loop), m_listenAddr(listenAddr)
{
    m_acceptor = std::make_shared<Acceptor>(loop, listenAddr);
}

liveStream::network::TcpServer::~TcpServer()
{

}

void liveStream::network::TcpServer::setNewConnectionCallback(const NewConnectionCallback &cb)
{
    m_newConnectionCallback = cb;
}

void liveStream::network::TcpServer::setNewConnectionCallback(NewConnectionCallback &&cb)
{
    m_newConnectionCallback = std::move(cb);
}

void liveStream::network::TcpServer::setDestroyConnectionCallback(const DestroyConnectionCallback &cb)
{
    m_destroyConnectionCallback = cb;
}

void liveStream::network::TcpServer::setDestroyConnectionCallback(DestroyConnectionCallback &&cb)
{
    m_destroyConnectionCallback = std::move(cb);
}

void liveStream::network::TcpServer::setActiveCallback(const ActiveCallback &cb)
{ 
    m_activeCallback = cb;
}

void liveStream::network::TcpServer::setActiveCallback(ActiveCallback &&cb)
{
    m_activeCallback = std::move(cb);
}

void liveStream::network::TcpServer::setWriteCompleteCallback(const wirteCompleteCallback &cb)
{
    m_writeCompleteCallback = cb;
}

void liveStream::network::TcpServer::setWriteCompleteCallback(wirteCompleteCallback &&cb)
{
    m_writeCompleteCallback = std::move(cb);
}

void liveStream::network::TcpServer::setMessageCallback(const OnMessageCallback &cb)
{
    m_messageCallback = cb;
}
void liveStream::network::TcpServer::setMessageCallback(OnMessageCallback &&cb)
{
    m_messageCallback = std::move(cb);
}

void liveStream::network::TcpServer::onAccet(int sockfd, const InetAddress &addr)
{
    TcpConnectionPtr conn = std::make_shared<TcpConnection>(m_loop, sockfd, m_listenAddr, addr);
    conn->setCloseCallback(std::bind(&TcpServer::onConnectionClose, this, std::placeholders::_1));

    if (m_writeCompleteCallback)
    {
        conn->setWriteCompleteCallback(m_writeCompleteCallback);
    }
    
    if (m_activeCallback)
    {
        conn->setActiveCallback(m_activeCallback);
    }

    conn->setOnMessageCallback(m_messageCallback);
    m_connections.insert(conn);
    m_loop->addEvent(conn);
    conn->enableCheckIdleTimeout(30);
    if (m_newConnectionCallback)
    {
        m_newConnectionCallback(conn);
    }
}

void liveStream::network::TcpServer::onConnectionClose(const TcpConnectionPtr &conn)
{
    // LOG(INFO) << "TcpServer::onConnectionClose [" << m_listenAddr.toIpPort() << "]";
    m_loop->assertInLoopThread();
    m_connections.erase(conn);
    m_loop->removeEvent(conn);
    if (m_destroyConnectionCallback)
    {
        m_destroyConnectionCallback(conn);
    }
    
}

void liveStream::network::TcpServer::Start()
{
    m_acceptor->setAcceptCallback(std::bind(&TcpServer::onAccet, this, 
        std::placeholders::_1, std::placeholders::_2));
    m_acceptor->Start();
}

void liveStream::network::TcpServer::Stop()
{
    m_acceptor->Stop();
}