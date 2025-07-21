/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 17:27:28
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-30 12:13:11
 * @FilePath: /liveStream-study/src/network/net/EventLoop.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include <sys/epoll.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <queue>
#include <mutex>
#include <memory>

#include "Event.h"
#include "PipeEvent.h"
#include "TimingWheel.h"

namespace liveStream 
{
    namespace network 
    {
        using EventPtr = std::shared_ptr<Event>;
        
        // 任务队列回调函数
        using TaskCallback = std::function<void()>;

        class EventLoop 
        {
        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void quit();
            void addEvent(const Event::ptr& event);
            void removeEvent(const Event::ptr& event);

            bool enableReading(const Event::ptr& event, bool enable);
            bool enableWriting(const Event::ptr& event, bool enable);

            // 任务队列相关
            void assertInLoopThread() const;
            bool isInLoopThread() const;
            void runInLoop(const TaskCallback& cb);
            void runInLoop(TaskCallback&& cb);
        
            // 时间轮相关
            void insertEntry(EntryPtr cb, int64_t delay);
            void runAfter(TaskCallback& cb, int64_t delay);
            void runAfter(TaskCallback&& cb, int64_t delay);
            void runEvery(TaskCallback& cb, int64_t interval);
            void runEvery(TaskCallback&& cb, int64_t interval);

        private:
            void RunCallbackFunctions();
            void wakeup();

            bool m_looping{false};
            int m_epoll_fd{-1};
            std::vector<struct epoll_event> m_epollEvents;
            std::unordered_map<int, Event::ptr> m_eventsMap;   // fd 与 感兴趣的event事件的映射表
            
            std::queue<TaskCallback> m_taskFunctions;
            std::mutex m_mutexLock;
            PipeEvent::ptr m_pipeEvent;
            TimingWheel m_timingWheel;
        };
    }
}