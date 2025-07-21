/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-15 16:28:35
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:08:23
 * @FilePath: /liveStream-study/src/network/net/Event.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/base/Network.h"
#include "Event.h"
#include "EventLoop.h"

liveStream::network::Event::Event()
:m_fd(-1), m_loop(nullptr)
{
}

liveStream::network::Event::Event(EventLoop *loop)
:m_loop(loop)
{
}

liveStream::network::Event::Event(EventLoop *loop, int fd)
:m_fd(fd), m_loop(loop)
{
}

liveStream::network::Event::~Event()
{
    close();
}

bool liveStream::network::Event::enableRead(bool enable)
{
    return m_loop->enableReading(shared_from_this(), enable);
}

bool liveStream::network::Event::enableWrite(bool enable)
{

    return m_loop->enableWriting(shared_from_this(), enable);
}

int liveStream::network::Event::fd() const
{
    return m_fd;
}

void liveStream::network::Event::close()
{
    if (m_fd > 0)
    {
        ::close(m_fd);
        m_fd = -1;
    }
}
