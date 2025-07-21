/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 17:01:17
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-20 17:22:22
 * @FilePath: /liveStream-study/src/base/task.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "Task.h"
#include "TTime.h"
#include "easylogging++.h"

liveStream::base::Task::Task(const TaskCallback &callback, uint64_t interval)
: m_callback(callback), m_interval(interval), 
m_when(liveStream::base::TTime::getCurrentTimeInMS() + interval)
{

}

liveStream::base::Task::Task(const TaskCallback &&callback, uint64_t interval)
: m_callback(std::move(callback)), m_interval(interval), 
m_when(liveStream::base::TTime::getCurrentTimeInMS() + interval)
{

}

void liveStream::base::Task::run()
{
    if (m_callback)
    {
        m_callback(shared_from_this());
    }
    
}

void liveStream::base::Task::restart()
{
    m_when = liveStream::base::TTime::getCurrentTimeInMS() + m_interval;
}