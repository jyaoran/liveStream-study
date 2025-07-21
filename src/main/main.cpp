/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-10 17:06:49
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 19:06:05
 * @FilePath: /liveStream-study/src/main/main.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "base/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char const *argv[])
{
    // el::Configurations conf("log_conf.conf");
    // el::Loggers::reconfigureLogger("default", conf);
    // el::Loggers::reconfigureAllLoggers(conf);

    LOG(INFO) << "Hello, world!";
    
    return 0;
}
