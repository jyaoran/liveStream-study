/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-30 01:23:16
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:25:37
 * @FilePath: /liveStream-study/src/base/TaskMgr.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once
#include "Task.h"
#include "NonCopyable.h"
#include "Singleton.h"
#include <unordered_set>
#include <mutex>

namespace liveStream
{
    namespace base
    {
        class TaskMgr : public NonCopyable      // 基本管理类都是单例
        {
        public:
            TaskMgr() = default;
            ~TaskMgr() = default;
        
        public:
            void OnWork();
            bool Add(Task::ptr &task);
            bool Del(Task::ptr &task);

        private:
            std::unordered_set<Task::ptr> tasks_; // 直接遍历使用hash
            std::mutex lock_;
        };
        // 方便单例访问
        #define sTaskMgr liveStream::base::Singleton<liveStream::base::TaskMgr>::Instance()
    } 
} // namespace liveStream
