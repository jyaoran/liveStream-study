/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-24 20:44:38
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:09:04
 * @FilePath: /liveStream-study/src/network/net/EventLoopThreadPool.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once

#include "base/NonCopyable.h"
#include "EventLoopThread.h"
#include "EventLoop.h"

#include <vector>
#include <atomic>

namespace liveStream 
{
    namespace network 
    {
        using EventLoopThreadPtr = std::shared_ptr<EventLoopThread>;

        class EventLoopThreadPool : public base::NonCopyable 
        {
        public:
            EventLoopThreadPool(int numThreads, int Start = 0, int cpus = 4);
            ~EventLoopThreadPool();
            
            std::vector<EventLoop *> getLoops() const;
            EventLoop *getNextLoop();

            size_t eventLoopThreadPoolSize();
            void Start();
        
        private:
            std::vector<EventLoopThreadPtr> m_threads;
            std::atomic_int32_t m_loopIndex{0};
        };

    } // namespace network
} // namespace liveStream


