/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-30 01:23:16
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:29:13
 * @FilePath: /liveStream-study/src/base/TaskMgr.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "TaskMgr.h"
#include "TTime.h"

using namespace liveStream::base;

/// @brief 定时执行任务
void TaskMgr::OnWork()
{
    std::lock_guard<std::mutex> lk(lock_);

    int64_t now = TTime::getCurrentTimeInMS();
    // 获取当前时间和任务的执行时间，比较。小于当前时间表示任务超时了，执行
    for(auto iter = tasks_.begin(); iter != tasks_.end();)
    {
        if((*iter)->when() < now)
        {
            (*iter)->run();
            if((*iter)->when() < now) // 判断有没有重置定时时间，没有就删除
            {
                iter = tasks_.erase(iter); // 删除返回下一个迭代器的地址
                continue;
            }
        }
        ++iter;
    }
}

/// @brief 加入任务
/// @param task  Task类的指针指针
/// @return 是否成功插入
bool TaskMgr::Add(Task::ptr &task)
{
    std::lock_guard<std::mutex> lk(lock_);

    auto iter = tasks_.find(task);
    if(iter != tasks_.end())        // 已经存在了
    {
        return false;
    }
    tasks_.emplace(task);
    return true;
}

/// @brief 
/// @param task 
/// @return 
bool TaskMgr::Del(Task::ptr &task)
{
    std::lock_guard<std::mutex> lk(lock_);
    
    auto iter = tasks_.find(task);
    if(iter != tasks_.end())
    {
        tasks_.erase(iter);
        return true;
    }
    return false;
}
