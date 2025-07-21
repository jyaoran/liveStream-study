/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-10 19:04:19
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 22:53:59
 * @FilePath: /liveStream-study/src/network/net/tests/InetAddressTest.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "network/base/InetAddress.h"
#include "base/easylogging++.h"
#include <string>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char const *argv[])
{
    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    std::string host = "127.0.0.1:8080";

    liveStream::network::InetAddress addr(host);
    LOG(INFO) << "host: " << host;
    LOG(INFO) << "\n ip: " << addr.getAddr() << ", "
            << "port: " << addr.getPort() << ", "
            << "lan : " << addr.isLanIp() << ", "
            << "wan : " << addr.isWanIp() << ", "
            << "loop: " << addr.isLoopbackIp();

    host = "192.168.1.1";
    std::string port  = "8080";
    liveStream::network::InetAddress addr1(host, port);
    LOG(INFO) << "host: " << host << ":" << port;
    LOG(INFO) << "\n ip: " << addr1.getAddr() << ", "
            << "port: " << addr1.getPort() << ", "
            << "lan : " << addr1.isLanIp() << ", "
            << "wan : " << addr1.isWanIp() << ", "
            << "loop: " << addr1.isLoopbackIp();
              
    return 0;
}
