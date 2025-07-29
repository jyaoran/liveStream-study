/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-10 17:06:49
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 15:27:27
 * @FilePath: /liveStream-study/src/main/main.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "base/easylogging++.h"
#include "base/Config.h"
#include "live/Session.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "base/TaskManager.h"
#include "live/LiveService.h"
#include "base/TTime.h"

INITIALIZE_EASYLOGGINGPP

using namespace liveStream::base;

int main(int argc, char const *argv[])
{
    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    LOG(INFO) << "Hello, world!";

    if(!sConfigMgr->LoadConfig("../config/config.json"))
    {
        std::cerr << "load config file failed." << std::endl;
        return -1;
    }
    
    Config::ptr config = sConfigMgr->GetConfig();

    sLiveService->Start();

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
