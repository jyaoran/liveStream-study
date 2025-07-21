/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 16:18:54
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-20 16:25:20
 * @FilePath: /liveStream-study/src/base/ttime.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include <cstdint>
#include <string>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace liveStream 
{
    namespace base 
    {
        class TTime 
        {
        public:
            static uint64_t getCurrentTimeInMS();
            static uint64_t getCurrentTime();
            static uint64_t getCurrentTime(int &year, int &month, int &day, 
                int &hour, int &minute, int &second);

            static std::string getCurrentTimeStr();
        };
    }
}



