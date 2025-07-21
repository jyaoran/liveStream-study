/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-24 21:22:32
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:08:55
 * @FilePath: /liveStream-study/src/network/net/EventLoopThreadPool.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#include "EventLoopThreadPool.h"

using namespace liveStream::network;

namespace
{
    void bindCpus(std::thread &thread, int n)
    {
        cpu_set_t cpuset;    // cpu_set_t 是一个位掩码，用来指定线程运行在哪些cpu上
        CPU_ZERO(&cpuset);   // CPU_ZERO(&cpuset) 用来清空 cpuset 变量
        CPU_SET(n, &cpuset); // CPU_SET(n, &cpuset) 用来将 cpu n 加入到 cpuset 变量中

        // pthread_setaffinity_np() 函数用来将线程绑定到指定的 cpu 上
        // thread.native_handle() 返回 pthread_t 类型，用来表示线程的 ID
        pthread_setaffinity_np(thread.native_handle(), sizeof(cpu_set_t), &cpuset);
    }
}

EventLoopThreadPool::EventLoopThreadPool(int numThreads, int Start, int cpus)
{
    if (numThreads <= 0)
    {
        numThreads = 1;
    }
    
    for (int i = 0; i < numThreads; ++i)
    {
        m_threads.emplace_back(std::make_shared<EventLoopThread>());
        if (cpus > 0)
        {
            int n = (Start + i) % cpus;
            bindCpus(m_threads.back()->getThread(), n);
        }   
    }
}

EventLoopThreadPool::~EventLoopThreadPool()
{

}

std::vector<EventLoop *> EventLoopThreadPool::getLoops() const
{
    std::vector<EventLoop *> result;
    for (auto &t : m_threads)
    {
        result.emplace_back(t->loop());
    }
    return result;
}

EventLoop *liveStream::network::EventLoopThreadPool::getNextLoop()
{
    int index = m_loopIndex;
    m_loopIndex++;

    return m_threads[index % m_threads.size()]->loop();
}


size_t liveStream::network::EventLoopThreadPool::eventLoopThreadPoolSize()
{
    return m_threads.size();
}

void liveStream::network::EventLoopThreadPool::Start()
{
    for (auto &t : m_threads)
    {
        t->run();
    }
}
