/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-10 22:29:04
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 16:25:50
 * @FilePath: /liveStream-study/src/network/net/tests/SocketOptTest.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#include "network/base/InetAddress.h"
#include "network/base/SocketOpt.h"
#include "base/easylogging++.h"

using namespace liveStream::network;

INITIALIZE_EASYLOGGINGPP

void testClient()
{
    int sockfd = SocketOpt::createNonBlockTcpsocket(AF_INET);
    if (sockfd < 0)
    {
        LOG(ERROR) << "create socket failed";
        return;
    }
    
    // InetAddress serverAddr("127.0.0.1:34444");
    std::string ip = "127.0.0.1";
    uint16_t port = 34444;

    InetAddress serverAddr(ip, port);
    LOG(INFO) << "\n ip: " << serverAddr.getAddr() << ", "
            << "port: " << serverAddr.getPort() << ", "
            << "lan : " << serverAddr.isLanIp() << ", "
            << "wan : " << serverAddr.isWanIp() << ", "
            << "loop: " << serverAddr.isLoopbackIp();
    
    SocketOpt opt(sockfd);
    opt.setNonBlocking(false);
    int ret = opt.connect(serverAddr);

    LOG(INFO) << "connect ret " << ret << " errno : " << errno 
            << "local addr : " << opt.getLocalAddress()->toIpPort() 
            << "peer addr : " << opt.getPeerAddress()->toIpPort();
}
    
void testServer()
{
    int sockfd = SocketOpt::createNonBlockTcpsocket(AF_INET);
    if (sockfd < 0)
    {
        LOG(ERROR) << "create socket failed";
        return;
    }

    InetAddress serverAddr("0.0.0.0:34444");
    SocketOpt opt(sockfd);
    opt.setNonBlocking(false);
    int ret = opt.bindAddress(serverAddr);

    opt.listen();
    InetAddress clientAddr;
    int connfd = opt.accept(&clientAddr);

    LOG(INFO) << "accept ret " << connfd << " errno : " << errno
            << "client addr : " << clientAddr.toIpPort();
}

int main(int argc, char const *argv[])
{
    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    testServer();
    return 0;
}
