/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-28 15:29:36
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-28 15:44:54
 * @FilePath: /liveStream-study/src/network/net/Connection.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "Connection.h"

using namespace liveStream::network;

liveStream::network::Connection::Connection(EventLoop *loop, int sockfd, const InetAddress &localAddr, const InetAddress &peerAddr)
: Event(loop, sockfd),
  m_localAddr(localAddr),
  m_peerAddr(peerAddr),
  m_active(false)
{
}

void liveStream::network::Connection::setLocalAddress(const InetAddress &localAddr)
{
    m_localAddr = localAddr;
}

void liveStream::network::Connection::setPeerAddress(const InetAddress &peerAddr)
{
    m_peerAddr = peerAddr;
}

const InetAddress& liveStream::network::Connection::getLocalAddress() const
{
    return m_localAddr;
}

const InetAddress& liveStream::network::Connection::getPeerAddress() const
{
    return m_peerAddr;
}

void liveStream::network::Connection::setContext(int contextType, const ContextPtr &context)
{
    m_contexts[contextType] = context;
}

void liveStream::network::Connection::setContext(int contextType, ContextPtr &&context)
{
    m_contexts[contextType] = std::move(context);
}

void liveStream::network::Connection::clearContext(int contextType)
{
      m_contexts[contextType].reset();  
}

void liveStream::network::Connection::clearAllContext()
{
      m_contexts.clear();  
}

void liveStream::network::Connection::setActiveCallback(const ActiveCallback &cb)
{
    m_activeCallback = cb;
}

void liveStream::network::Connection::setActiveCallback(ActiveCallback &&cb)
{
    m_activeCallback = std::move(cb);
}

void liveStream::network::Connection::Active()
{
    if (!m_active.load()) // atomic.load() 作用是获取原子变量的值，而不修改它
    {
        m_loop->runInLoop([this]() {
            m_active.store(true);   // atomic.store() 作用是修改原子变量的值
            if (m_activeCallback)
            {
                m_activeCallback(std::dynamic_pointer_cast<Connection>(shared_from_this()));
            }
        });
    }
    
}

void liveStream::network::Connection::deactive()
{
    m_active.store(false); // atomic.store() 作用是修改原子变量的值
}
