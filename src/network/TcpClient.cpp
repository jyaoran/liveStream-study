/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-30 17:20:39
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-03 15:52:28
 * @FilePath: /liveStream-study/src/network/TcpClient.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "TcpClient.h"
#include "network/base/SocketOpt.h"
#include "base/easylogging++.h"

liveStream::network::TcpClient::TcpClient(EventLoop *loop, const InetAddress &serverAddr)
: TcpConnection(loop, -1, InetAddress(), serverAddr), m_serverAddr(serverAddr)
{
}

liveStream::network::TcpClient::~TcpClient()
{
    onClose();
}

void liveStream::network::TcpClient::setConnectionCallback(const ConnectionCallback & cb)
{
    m_connectionCallback = cb;
}

void liveStream::network::TcpClient::setConnectionCallback(ConnectionCallback &&cb)
{
    m_connectionCallback = std::move(cb);
}

void liveStream::network::TcpClient::connect()
{
    m_loop->runInLoop(std::bind(&TcpClient::ConnectInLoop, this));
}

void liveStream::network::TcpClient::onRead()
{
    if (m_status == kTcpConnectStatusConnecting)
    {
        if (checkError())
        {
            LOG(ERROR) << "Connect to server error, ip = " << m_serverAddr.toIpPort();
            onClose();
            return;
        }
        updateConnectStatus();
        return;
    }
    else if (m_status == kTcpConnectStatusConnected)
    {
        TcpConnection::onRead();
    }
}

void liveStream::network::TcpClient::onWrite()
{
    if (m_status == kTcpConnectStatusConnecting)
    {
        if (checkError())
        {
            LOG(ERROR) << "Connect to server error, ip = " << m_serverAddr.toIpPort();
            onClose();
            return;
        }
        updateConnectStatus();
        return;
    }
    else if (m_status == kTcpConnectStatusConnected)
    {
        TcpConnection::onWrite();
    }
}

void liveStream::network::TcpClient::onClose()
{
    if (m_status == kTcpConnectStatusConnected || m_status == kTcpConnectStatusConnecting)
    {
        m_loop->removeEvent(std::dynamic_pointer_cast<TcpClient>(shared_from_this()));
    }
    
    m_status = kTcpConnectStatusDisconnected;
    TcpConnection::onClose();
}

void liveStream::network::TcpClient::clientSend(std::list<BufferNodePtr> &msgList)
{
    if (m_status == kTcpConnectStatusConnected)
    {
        TcpConnection::send(msgList);
    }
    
}

void liveStream::network::TcpClient::clientSend(const char *data, size_t len)
{
    if (m_status == kTcpConnectStatusConnected)
    {
        TcpConnection::send(data, len);
    }
}

void liveStream::network::TcpClient::ConnectInLoop()
{
    LOG(INFO) << "TcpClient::ConnectInLoop - " << m_serverAddr.toIpPort();
    m_loop->assertInLoopThread();
    m_fd = SocketOpt::createNonBlockTcpsocket(AF_INET);
    if (m_fd < 0)
    {
        LOG(ERROR) << "Create socket error";
        onClose();
        return;
    }
    
    m_status = kTcpConnectStatusConnecting;
    m_loop->addEvent(std::dynamic_pointer_cast<TcpClient>(shared_from_this()));
    enableWrite(true);
    SocketOpt opt(m_fd);
    auto ret = opt.connect(m_serverAddr);
    if (ret == 0)
    {
        updateConnectStatus();
        return;
    }
    else if (ret < 0)
    {
        if (errno != EINPROGRESS)
        {
            LOG(ERROR) << "Connect to server error, ip = " << m_serverAddr.toIpPort();
            onClose();
            return;
        }       
    }
}

void liveStream::network::TcpClient::updateConnectStatus()
{
    m_status = kTcpConnectStatusConnected;
    if (m_connectionCallback)
    {
        m_connectionCallback(std::dynamic_pointer_cast<TcpClient>(shared_from_this()), true);
    }
}

bool liveStream::network::TcpClient::checkError()
{
    int error = 0;
    socklen_t len = sizeof(error);
    getsockopt(m_fd, SOL_SOCKET, SO_ERROR, &error, &len);
    
    return error != 0;
}
