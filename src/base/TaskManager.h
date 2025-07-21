/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 17:25:00
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:06:11
 * @FilePath: /liveStream-study/src/base/TaskManager.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#pragma once

#include "Task.h"
#include "NonCopyable.h"
#include <unordered_set>
#include <mutex>

// template<typename T>
// class Singleton 
// {
// public:
//     ~Singleton() = delete;

//     static T* Instance()
//     {
//         if (m_pInstance == nullptr)
//         {
//             std::lock_guard<std::mutex> lock(m_mutex);
//             if (m_pInstance == nullptr)
//             {
//                 m_pInstance = new T();
//             }
//         }
        
//         return m_pInstance;
//     }

// private:
//     Singleton() = delete;

// private:
//     static T* m_pInstance{nullptr};
//     static std::mutex m_mutex;
// };


namespace liveStream 
{
    namespace base 
    {
        class TaskManager : public NonCopyable 
        {
        private:
            TaskManager() = default;
            
        public:
            ~TaskManager();
            
            static TaskManager* Instance();
            bool addTask(liveStream::base::Task::ptr &task);
            bool removeTask(liveStream::base::Task::ptr &task);
            void onWork();
            void DestroyInstance();
        private:
            std::unordered_set<liveStream::base::Task::ptr> m_tasks;
            static std::mutex m_TaskManagerMutex;
            static TaskManager* m_pTaskManagerInstance;
        };
    }
}