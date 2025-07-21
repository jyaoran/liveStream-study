/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 17:25:57
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-23 16:24:41
 * @FilePath: /liveStream-study/src/base/taskManager.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "TaskManager.h"
#include "TTime.h"

using namespace liveStream::base;

// 初始化静态成员
TaskManager *TaskManager::m_pTaskManagerInstance = nullptr;
std::mutex TaskManager::m_TaskManagerMutex;

// 析构函数只负责清理当前对象的资源
liveStream::base::TaskManager::~TaskManager()
{
    // 当前对象的清理逻辑
    // 例如，清理其他非静态成员
}

// 获取单例实例
TaskManager *TaskManager::Instance()
{
    if (m_pTaskManagerInstance == nullptr)
    {
        std::lock_guard<std::mutex> lock(m_TaskManagerMutex);
        if (m_pTaskManagerInstance == nullptr)
        {
            m_pTaskManagerInstance = new TaskManager();
        }
    }
    return m_pTaskManagerInstance;
}

bool liveStream::base::TaskManager::addTask(liveStream::base::Task::ptr &task)
{
    std::lock_guard<std::mutex> lock(m_TaskManagerMutex);

    auto iter = m_tasks.find(task);
    if (iter != m_tasks.end())
    {
        return false;
    }

    m_tasks.insert(task);
    
    return true;
}

bool liveStream::base::TaskManager::removeTask(liveStream::base::Task::ptr &task)
{
    std::lock_guard<std::mutex> lock(m_TaskManagerMutex);

    auto iter = m_tasks.find(task);
    if (iter != m_tasks.end())
    {
        m_tasks.erase(iter);
        return true;
    }
    return false;
}

void liveStream::base::TaskManager::onWork()
{
    // 获取当前时间（以毫秒为单位）
    int64_t currentTime = TTime::getCurrentTimeInMS();
    
    // 使用锁保护任务列表的访问
    std::lock_guard<std::mutex> lock(m_TaskManagerMutex);
    
    // 遍历任务列表
    for (auto iter = m_tasks.begin(); iter != m_tasks.end();)
    {
        // 检查任务是否到期
        if ((*iter)->when() < currentTime)
        {
            // 执行任务
            (*iter)->run();
            
            // 再次检查任务是否需要移除
            if ((*iter)->when() < currentTime)
            {
                // 从任务列表中移除任务，并更新迭代器（返回的是下一个元素的迭代器）
                iter = m_tasks.erase(iter);
                continue; // 跳过当前迭代的剩余部分，并继续下一次迭代
            }
        }
        // 如果任务没有到期，继续遍历下一个任务
        ++iter;
    }
}


// 静态方法用于销毁单例实例
void TaskManager::DestroyInstance()
{
    std::lock_guard<std::mutex> lock(m_TaskManagerMutex);
    if (m_pTaskManagerInstance != nullptr)
    {
        delete m_pTaskManagerInstance;
        m_pTaskManagerInstance = nullptr;
    }
}
