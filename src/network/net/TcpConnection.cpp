/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-28 18:54:39
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 18:37:48
 * @FilePath: /liveStream-study/src/network/net/TcpConnection.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "TcpConnection.h"
#include "base/easylogging++.h"

using namespace liveStream::network;

liveStream::network::TcpConnection::TcpConnection(EventLoop *loop, int sockfd, const InetAddress &local, const InetAddress &peer)
    : Connection(loop, sockfd, local, peer)
{
}

liveStream::network::TcpConnection::~TcpConnection()
{
    // 在webrtc中，sdp使用tcp，之后使用udp
    // 因为在替换udp的时候使用的是udp的线程，tcp不在这个线程中
    // 所以这样保证一下，在同一个线程中
    m_loop->runInLoop([this]()
                      { onClose(); });
}

void liveStream::network::TcpConnection::setCloseCallback(const CloseConnectionCallback &cb)
{
    m_CloseCallback = cb;
}

void liveStream::network::TcpConnection::setCloseCallback(CloseConnectionCallback &&cb)
{
    m_CloseCallback = std::move(cb);
}

void liveStream::network::TcpConnection::onClose()
{
    // LOG(INFO) << "TcpConnection::onClose [" << m_peerAddr.toIpPort() << "]";
    m_loop->assertInLoopThread();
    if (m_closed == false)
    {
        m_closed = true;
        if (m_CloseCallback)
        {
            m_CloseCallback(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
        Event::close();
    }
}

void liveStream::network::TcpConnection::forceClose()
{
    m_loop->runInLoop([this]()
                      { onClose(); });
}

void liveStream::network::TcpConnection::setOnMessageCallback(const OnMessageCallback &cb)
{
    m_OnMessageCallback = cb;
}

void liveStream::network::TcpConnection::setOnMessageCallback(OnMessageCallback &&cb)
{
    m_OnMessageCallback = std::move(cb);
}

void liveStream::network::TcpConnection::onRead()
{
    if (m_closed)
    {
        LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << " connection is closed.";
        return;
    }

    extendLifetime(); // 延长生命周期, 每读一次就应该重新添加进去一次
    while (true)
    {
        int err = 0;
        auto ret = m_messageBuffer.ReadFd(m_fd, &err);
        if (ret > 0)
        {
            // 读到数据了，进行上层回调
            if (m_OnMessageCallback)
            {
                m_OnMessageCallback(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()), m_messageBuffer);
            }
        }
        else if (ret == 0)
        {
            // 读到结尾了，关闭连接
            onClose();
            break;
        }
        else
        {
            // 读错误了，关闭连接
            if (err != EINTR && err != EAGAIN && err != EWOULDBLOCK)
            {
                // err != EINTR && err != EAGAIN && err != EWOULDBLOCK
                // EINTR : 被信号中断，通常可以忽略，重新尝试读取。
                // EAGAIN : 资源暂时不可用（非阻塞IO下无数据可读），不是致命错误，稍后可重试。
                // EWOULDBLOCK : 操作会阻塞（与EAGAIN等价），在非阻塞IO下表示当前没有数据可读，可以
                onClose();
            }
            break; // 出错了，退出循环
        }
    }
}

void liveStream::network::TcpConnection::setWriteCompleteCallback(const wirteCompleteCallback &cb)
{
    m_WriteCompleteCallback = cb;
}

void liveStream::network::TcpConnection::setWriteCompleteCallback(wirteCompleteCallback &&cb)
{
    m_WriteCompleteCallback = std::move(cb);
}

void liveStream::network::TcpConnection::onWrite()
{
    if (m_closed)
    {
        LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << " connection is closed.";
        return;
    }

    extendLifetime(); // 延长生命周期, 每写一次就应该重新添加进去一次
    if (!m_iovecList.empty())
    {
        while (true)
        {
            auto ret = ::writev(m_fd, m_iovecList.data(), m_iovecList.size());
            if (ret >= 0)
            {
                while (ret > 0) // ret 是已经写的字节数
                {
                    // 写入的长度小于数据长度，说明数据没有写完，需要继续写
                    if (m_iovecList.front().iov_len > ret)
                    {
                        // 偏移指针到下一个位置
                        m_iovecList.front().iov_base =
                            (char *)m_iovecList.front().iov_base + ret;
                        m_iovecList.front().iov_len -= ret;
                        break; // 还没有写完，继续写
                    }
                    else // 发送完成了, 直接清理掉第一个
                    {
                        ret -= m_iovecList.front().iov_len;
                        m_iovecList.erase(m_iovecList.begin());
                    }
                }

                if (m_iovecList.empty()) // 全部发送完成了
                {
                    enableWrite(false);
                    if (m_WriteCompleteCallback)
                    {
                        m_WriteCompleteCallback(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
                    }
                    return;
                }
            }
        }
    }
    else
    {
        enableWrite(false);
        if (m_WriteCompleteCallback)
        {
            m_WriteCompleteCallback(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
        return;
    }
}

void liveStream::network::TcpConnection::send(std::list<BufferNodePtr> &msgList)
{
    m_loop->runInLoop([this, &msgList]()
                      { sendInLoop(msgList); });
}

void liveStream::network::TcpConnection::send(const void *data, size_t size)
{
    m_loop->runInLoop([this, data, size]()
                      { sendInLoop(data, size); });
}

void liveStream::network::TcpConnection::setTimeoutCallback(int timeout, const TimeoutCallback &cb)
{
    auto p = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    m_loop->runAfter([p, cb]()
                     { cb(p); }, timeout);
}

void liveStream::network::TcpConnection::setTimeoutCallback(int timeout, TimeoutCallback &&cb)
{
    auto p = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    m_loop->runAfter([p, cb]()
                     { cb(p); }, timeout);
}

void liveStream::network::TcpConnection::onTimeout()
{
    // LOG(ERROR) << "host: " << m_peerAddr.toIpPort() << " timeout and close it!";
    onClose();
}

/// @brief 加入超时事件到定时器
/// @param max_time 
void liveStream::network::TcpConnection::enableCheckIdleTimeout(uint32_t maxTime)
{
    auto timePtr = std::make_shared<TimeoutEntry>(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
    m_maxIdleTime = maxTime;
    m_timeoutEntry = timePtr;
    m_loop->insertEntry(timePtr, m_maxIdleTime);
}

void liveStream::network::TcpConnection::onError(const std::string &msg)
{
    LOG(ERROR) << "host:" << m_peerAddr.toIpPort() << " error msg:" << msg;
    onClose();
}

void liveStream::network::TcpConnection::sendInLoop(std::list<BufferNodePtr> &msgList)
{
    if (m_closed)
    {
        LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << " connection is closed.";
        return;
    }

    for (auto &msg : msgList)
    {
        struct iovec vec;
        vec.iov_base = (void *)msg->m_addr;
        vec.iov_len = msg->m_size;

        m_iovecList.push_back(vec);
    }

    if (!m_iovecList.empty())
    {
        enableWrite(true);
    }
    
}

void liveStream::network::TcpConnection::sendInLoop(const void *data, size_t size)
{
    if (m_closed)
    {
        LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << " connection is closed.";
        return;
    }

    size_t sendSize = 0;
    if (m_iovecList.empty())
    {
        sendSize = ::write(m_fd, data, size);
        if (sendSize < 0)
        {
            if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
            {
                LOG(ERROR) << "host : " << m_peerAddr.toIpPort() << " write error. errno = " << errno;
                onClose();
                return;
            }
            sendSize = 0;
        }
        
        size -= sendSize;
        if (size == 0)
        {
            if (m_WriteCompleteCallback)
            {
                m_WriteCompleteCallback(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
            }
            return;
        }
    }
    
    // 缓冲区不为空，或者一次发送没有发送完，将剩余数据加入到缓冲区中
    if (size > 0)
    {
        struct iovec vec;
        vec.iov_base = (void *)data;
        vec.iov_len = size;
        
        m_iovecList.push_back(vec);
        enableWrite(true);
    }
    
}

void liveStream::network::TcpConnection::extendLifetime()
{
    auto tp = m_timeoutEntry.lock();
    if (tp)
    {
        m_loop->insertEntry(tp, m_maxIdleTime);
    }
}
