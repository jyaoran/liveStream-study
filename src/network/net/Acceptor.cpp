/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-15 16:27:22
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-27 17:14:43
 * @FilePath: /liveStream-study/src/network/net/Acceptor.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "Acceptor.h"
#include "base/easylogging++.h"

#include <errno.h>

using namespace liveStream::network;

liveStream::network::Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr)
:Event(loop), m_addr(listenAddr)
{
}

liveStream::network::Acceptor::~Acceptor()
{
    Stop();
    if (m_socketOpt)
    {
        delete m_socketOpt;
        m_socketOpt = nullptr;
    }
    
}

void liveStream::network::Acceptor::setAcceptCallback(const AcceptorCallback &cb)
{
    m_acceptCallback = cb;
}

void liveStream::network::Acceptor::setAcceptCallback(AcceptorCallback &&cb)
{
    m_acceptCallback = std::move(cb);
}

void liveStream::network::Acceptor::Start()
{
    m_loop->runInLoop([this](){
        open();
    });
}

void liveStream::network::Acceptor::Stop()
{
    m_loop->removeEvent(std::dynamic_pointer_cast<Acceptor>(shared_from_this()));
}

void liveStream::network::Acceptor::onRead()
{
    if (!m_socketOpt)
    {
        return;
    }
    
    while (true)
    {
        InetAddress peerAddr;
        int sockfd = m_socketOpt->accept(&peerAddr);
        if (sockfd >= 0)
        {
            if (m_acceptCallback)
            {
                m_acceptCallback(sockfd, peerAddr);
            }
            
        }
        else
        {
            if (errno != EINTR && errno != EAGAIN)
            {
                LOG(ERROR) << "accept onRead error. errno = " << errno << "; " << strerror(errno);
                onClose();
            }
            break;
        }
    } 
}

void liveStream::network::Acceptor::onError(const std::string &msg)
{

}

void liveStream::network::Acceptor::onClose()
{
    LOG(INFO) << "Acceptor closed";
    Stop();
    open();
}

void liveStream::network::Acceptor::open()
{
    if (m_fd > 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
    
    if (m_addr.isIpv6())
    {
        m_fd = SocketOpt::createNonBlockTcpsocket(AF_INET6);
    }
    else
    {
        m_fd = SocketOpt::createNonBlockTcpsocket(AF_INET);
    }
    
    if (m_fd < 0)
    {
        LOG(ERROR) << "create socket error. errno = " << errno << "; " << strerror(errno);
        exit(-1);
    }
    
    if (m_socketOpt)
    {
        delete m_socketOpt;
        m_socketOpt = nullptr;
    }
    
    m_loop->addEvent(std::dynamic_pointer_cast<Acceptor>(shared_from_this()));

    m_socketOpt = new SocketOpt(m_fd);
    m_socketOpt->setReuseAddr(true);
    m_socketOpt->setReusePort(true);
    m_socketOpt->bindAddress(m_addr);
    m_socketOpt->listen();
    
    
}
