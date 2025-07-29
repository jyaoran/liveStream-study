/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-23 14:57:33
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:34:04
 * @FilePath: /liveStream-study/src/network/net/EventLoop.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "EventLoop.h"
#include "base/TTime.h"
#include "base/easylogging++.h"

#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

using namespace liveStream::network;

static thread_local EventLoop* t_loopInThisThread = nullptr;

liveStream::network::EventLoop::EventLoop()
:m_epoll_fd(epoll_create1(EPOLL_CLOEXEC)), m_epollEvents(1024)
{
    if(t_loopInThisThread)
    {
        LOG(ERROR) << "there alread had a eventloop!!!";
        exit(-1);
    }
    
    t_loopInThisThread = this;
}

liveStream::network::EventLoop::~EventLoop()
{
    quit();
}

void liveStream::network::EventLoop::loop()
{
    m_looping = true;
    uint64_t timeout = 1000;

    while (m_looping)
    {
        memset(&m_epollEvents[0], 0, sizeof(struct epoll_event));

        auto ret = ::epoll_wait(m_epoll_fd, 
            (struct epoll_event*)&m_epollEvents[0], m_epollEvents.size(), timeout);

        if (ret >= 0)
        {
            for (int i = 0; i < ret; ++i)
            {
                struct epoll_event &activEvent = m_epollEvents[i];

                if (activEvent.data.fd <= 0)
                    continue;
                
                auto iter = m_eventsMap.find(activEvent.data.fd);
                if (iter == m_eventsMap.end())
                    continue;

                Event::ptr &event = iter->second;

                if (activEvent.events & EPOLLERR)
                {
                    int errorNum = 0;
                    socklen_t len = sizeof(errorNum);
                    getsockopt(activEvent.data.fd, SOL_SOCKET, SO_ERROR, &errorNum, &len);
                    LOG(ERROR) << "epoll error fd =" << activEvent.data.fd << " error =" << errorNum;
                
                    event->onError(strerror(errorNum));
                    continue;
                }
                
                if ((activEvent.events & EPOLLHUP) && !(activEvent.events & EPOLLIN))
                {
                    LOG(INFO) << "tcp connection close.";
                    event->onClose();
                    continue;
                }
                
                if (activEvent.events &(EPOLLIN | EPOLLPRI | EPOLLRDHUP))
                {
                    // LOG(TRACE) << "fd = " << activEvent.data.fd << " read event.";
                    event->onRead();
                }
                
                if (activEvent.events & EPOLLOUT)
                {
                    // LOG(TRACE) << "fd = " << activEvent.data.fd << " write event.";
                    event->onWrite();
                }
            }
        
            if (m_epollEvents.size() == ret)
            {   
                m_epollEvents.resize(m_epollEvents.size() * 2);
            }

            RunCallbackFunctions();
            auto now = liveStream::base::TTime::getCurrentTimeInMS();
            m_timingWheel.OnTimer(now);
        }
        else
        {
            LOG(ERROR) << "epoll_wait error. errno = " << errno;
        }
        
    }
    
}

void liveStream::network::EventLoop::quit()
{
    m_looping = false;
}

void liveStream::network::EventLoop::addEvent(const Event::ptr &event)
{
    int fd = event->fd();

    auto iter = m_eventsMap.find(fd);
    if (iter != m_eventsMap.end())
    {
        LOG(INFO) << "fd already exist in event loop";
        return;
    }
    
    event->m_event = kEventRead;
    m_eventsMap[fd] = event;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = event->m_event;
    ev.data.fd = fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &ev) < 0)
    {
        LOG(ERROR) << "epoll_ctl add fd error. fd = " << fd << " errno = " << errno;
    }
    
}

void liveStream::network::EventLoop::removeEvent(const Event::ptr &event)
{
    int fd = event->fd();
    auto iter = m_eventsMap.find(fd);
    if (iter == m_eventsMap.end())
    {
        LOG(INFO) << "fd not exist in event loop";
        return;
    }
    
    m_eventsMap.erase(iter);

    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = event->m_event;
    ev.data.fd = fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, &ev) < 0)
    {
        LOG(ERROR) << "epoll_ctl del fd error. fd = " << fd << " errno = " << errno;
    }
    
}

bool liveStream::network::EventLoop::enableReading(const Event::ptr &event, bool enable)
{
    int fd = event->fd();
    auto iter = m_eventsMap.find(fd);
    if (iter == m_eventsMap.end())
    {
        LOG(INFO) << "fd not exist in event loop";
        return false;
    }
    
    if (enable)
    {
        event->m_event |= kEventRead;
    }
    else
    {
        event->m_event &= ~kEventRead;
    }
    
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = event->m_event;
    ev.data.fd = fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
    {
        LOG(ERROR) << "epoll_ctl mod fd error. fd = " << fd << " errno = " << errno;
        return false;
    }
    return true;
}

bool liveStream::network::EventLoop::enableWriting(const Event::ptr &event, bool enable)
{
    int fd = event->fd();
    auto iter = m_eventsMap.find(fd);
    if (iter == m_eventsMap.end())
    {
        LOG(INFO) << "fd not exist in event loop";
        return false;
    }
    
    if (enable)
    {
        event->m_event |= kEventWrite;
    }
    else
    {
        event->m_event &= ~kEventWrite;
    }
    
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = event->m_event;
    ev.data.fd = fd;
    if (epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, fd, &ev) < 0)
    {
        LOG(ERROR) << "epoll_ctl mod fd error. fd = " << fd << " errno = " << errno;
        return false;
    }
    return true;
}

void liveStream::network::EventLoop::assertInLoopThread() const
{
    if (!isInLoopThread())
    {
        LOG(ERROR) << "must be in event loop thread";
        exit(-1);
    }
    
}

bool liveStream::network::EventLoop::isInLoopThread() const
{
    // LOG(INFO) << "isInLoopThread = " << t_loopInThisThread << ", this = " << this;
    return t_loopInThisThread == this;
}

void liveStream::network::EventLoop::runInLoop(const TaskCallback &cb)
{
    if (isInLoopThread())
    {
        cb();     
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_mutexLock);
        m_taskFunctions.push(cb);

        wakeup();
    }
    
}

void liveStream::network::EventLoop::runInLoop(TaskCallback &&cb)
{
    if (isInLoopThread())
    {
        // LOG(INFO) << "runInLoop: Is in loop thread";
        cb();     
    }
    else
    {
        std::lock_guard<std::mutex> lock(m_mutexLock);
        m_taskFunctions.push(std::move(cb));
        // LOG(INFO) << "runInLoop: Is not in loop thread" << ", this = " << std::this_thread::get_id();
        wakeup();
    }
}

void liveStream::network::EventLoop::insertEntry(EntryPtr cb, int64_t delay)
{
    if (isInLoopThread())
    {
        m_timingWheel.InsertEntry(delay, cb);
    }
    else
    {
        runInLoop([this, cb, delay]() {
            m_timingWheel.InsertEntry(delay, cb);
        });
    }
    
}

void liveStream::network::EventLoop::runAfter(TaskCallback &cb, int64_t delay)
{
    if (isInLoopThread())
    {
        m_timingWheel.RunAfter(delay, cb);
    }
    else
    {
        runInLoop([this, cb, delay]() {
            m_timingWheel.RunAfter(delay, cb);
        });
    }
}

void liveStream::network::EventLoop::runAfter(TaskCallback &&cb, int64_t delay)
{
    if (isInLoopThread())
    {
        m_timingWheel.RunAfter(delay, cb);
    }
    else
    {
        runInLoop([this, cb, delay]() {
            m_timingWheel.RunAfter(delay, cb);
        });
    }
    
}

void liveStream::network::EventLoop::runEvery(TaskCallback &cb, int64_t interval)
{
    if (isInLoopThread())
    {
        m_timingWheel.RunEvery(interval, cb);
    }
    else
    {
        runInLoop([this, cb, interval]() {
            m_timingWheel.RunEvery(interval, cb);
        });
    }
}

void liveStream::network::EventLoop::runEvery(TaskCallback &&cb, int64_t interval)
{
    if (isInLoopThread())
    {
        m_timingWheel.RunEvery(interval, cb);
    }
    else
    {
        runInLoop([this, cb, interval]() {
            m_timingWheel.RunEvery(interval, cb);
        });
    }
}

void liveStream::network::EventLoop::RunCallbackFunctions()
{
    std::queue<TaskCallback> functionsTmp;
    {
        std::lock_guard<std::mutex> lock(m_mutexLock);
        m_taskFunctions.swap(functionsTmp);
    }

    while (!functionsTmp.empty())
    {
        auto f = functionsTmp.front();
        f();
        functionsTmp.pop(); 
    }
}

void liveStream::network::EventLoop::wakeup()
{
    if (!m_pipeEvent)
    {
        m_pipeEvent = std::make_shared<PipeEvent>(this);
        addEvent(m_pipeEvent);
    }
    
    int64_t one = 1;
    m_pipeEvent->write((const char*)&one, sizeof(one));
}
