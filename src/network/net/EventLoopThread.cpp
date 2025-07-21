/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-24 17:30:11
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 16:47:32
 * @FilePath: /liveStream-study/src/network/net/EventLoopThread.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "EventLoopThread.h"

using namespace liveStream::network;

liveStream::network::EventLoopThread::EventLoopThread()
    // : m_thread(std::bind(&EventLoopThread::startEventLoop, this))
    : m_thread([this](){startEventLoop();})
{
}

liveStream::network::EventLoopThread::~EventLoopThread()
{
    run();
    if (m_loop)
    {
        m_loop->quit();
    }

    if (m_thread.joinable())
    {
        m_thread.join();
    } 
}

void liveStream::network::EventLoopThread::run()
{
    std::call_once(m_eventLoopThreadOnceFlag, [this]()
                   {
        {
            std::lock_guard<std::mutex> lock(m_eventLoopThreadMutex);
            m_running = true;
            m_eventLoopThreadCondition.notify_all();

        }
        auto f = m_eventLoopThreadPromiseLoop.get_future();
        f.get(); 
                   });
}

std::thread &liveStream::network::EventLoopThread::getThread()
{
    return m_thread;
}

EventLoop *liveStream::network::EventLoopThread::loop() const
{
    return m_loop;
}

void liveStream::network::EventLoopThread::startEventLoop()
{
    EventLoop loop;

    std::unique_lock<std::mutex> lock(m_eventLoopThreadMutex);

    m_eventLoopThreadCondition.wait(lock, [this]() { return m_running; });
    m_loop = &loop;
    m_eventLoopThreadPromiseLoop.set_value(1);
    
    loop.loop();

    m_loop = nullptr;
}
