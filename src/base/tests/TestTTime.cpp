/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-23 16:25:25
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:06:26
 * @FilePath: /liveStream-study/src/base/tests/TestTTime.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "base/easylogging++.h"
#include "base/TTime.h"


 INITIALIZE_EASYLOGGINGPP

int main(int argc, char const *argv[])
{
    liveStream::base::TTime t;
    LOG(INFO) << "Current time in ms: " << t.getCurrentTimeInMS();
    LOG(INFO) << "Current time: " << t.getCurrentTime();
    
    LOG(INFO) << "Current time str: " << t.getCurrentTimeStr();

    return 0;
}