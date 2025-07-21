/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 16:19:02
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 21:58:56
 * @FilePath: /liveStream-study/src/base/TTime.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "TTime.h"
#include "easylogging++.h"

uint64_t liveStream::base::TTime::getCurrentTimeInMS()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

uint64_t liveStream::base::TTime::getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    return tv.tv_sec;
}

uint64_t liveStream::base::TTime::getCurrentTime(int &year, int &month, int &day, int &hour, int &minute, int &second)
{
    struct tm tm;
    
    time_t t = time(nullptr);
    localtime_r(&t, &tm);

    year = tm.tm_year + 1900;
    month = tm.tm_mon + 1;
    day = tm.tm_mday;
    hour = tm.tm_hour;
    minute = tm.tm_min;
    second = tm.tm_sec;
    
    return t;
}

std::string liveStream::base::TTime::getCurrentTimeStr()
{
    struct timeval tv;
    struct tm tm;

    gettimeofday(&tv, nullptr);
    time_t t = time(nullptr);
    localtime_r(&t, &tm);

    char buf[128];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

    return std::string(buf);
}

// INITIALIZE_EASYLOGGINGPP

// int main(int argc, char const *argv[])
// {
//     liveStream::base::TTime t;
//     LOG(INFO) << "Current time in ms: " << t.getCurrentTimeInMS();
//     LOG(INFO) << "Current time: " << t.getCurrentTime();
    
//     LOG(INFO) << "Current time str: " << t.getCurrentTimeStr();

//     return 0;
// }
