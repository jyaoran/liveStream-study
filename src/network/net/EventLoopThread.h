/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-24 17:30:26
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-26 14:33:28
 * @FilePath: /liveStream-study/src/network/net/EventLoopThread.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>


#include "base/NonCopyable.h"
#include "EventLoop.h"

namespace liveStream 
{
    namespace network 
    {
        class EventLoopThread : public base::NonCopyable
        {
        public:
            EventLoopThread();
            ~EventLoopThread();

            void run();
            std::thread &getThread();
            EventLoop *loop() const;
        private:
            void startEventLoop();

            EventLoop *m_loop{nullptr};
            bool m_running{false};
            
            std::mutex m_eventLoopThreadMutex;
            std::condition_variable m_eventLoopThreadCondition;
            std::once_flag m_eventLoopThreadOnceFlag;
            std::promise<int> m_eventLoopThreadPromiseLoop;
        
            std::thread m_thread;
        };  
    } // namespace network
} // namespace liveStream


