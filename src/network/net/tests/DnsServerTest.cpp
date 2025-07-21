/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-10 23:35:34
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 23:58:55
 * @FilePath: /liveStream-study/src/network/net/tests/DnsServerTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/DnsServer.h"
#include <iostream>
#include "base/easylogging++.h"

using namespace liveStream::network;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    std::vector<InetAddress::ptr> list;
    sDnsService->AddHost("www.baidu.com");
    sDnsService->Start();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    list = sDnsService->GetHostAddress("www.baidu.com");
    
    for(auto &i : list)
    {
        LOG(INFO) << "ip: " << i->getAddr();
    }
    return 0;
}