/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-10 18:55:26
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:59:32
 * @FilePath: /liveStream-study/src/network/net/tests/UdpServerTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/UdpServer.h"
#include "base/easylogging++.h"

#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;

INITIALIZE_EASYLOGGINGPP

EventLoopThread eventloop_thread;
std::thread th;
int main()
{

    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    eventloop_thread.run();
    EventLoop* loop = eventloop_thread.loop();

    if(loop)
    {
        InetAddress listen("127.0.0.1:34444");
        // 继承了std::enable_shared_from_this<Event>必须使用智能指针
        std::shared_ptr<UdpServer> server = std::make_shared<UdpServer>(loop, listen);
        
        // 测试OnRead，收到什么发回什么,echo服务器
        server->setOnMessageCallback([&server](const UdpSocketPtr& con, const InetAddress &addr, MsgBuffer &buf){
            std::cout << "host : " << addr.toIpPort() << " \nmsg : "<< buf.Peek() << std::endl;
            char buffer[1024];
            std::cout << "input: ";
            std::cin >> buffer;
            struct sockaddr_in6 saddr;
            socklen_t len = sizeof(saddr);
            addr.getSockAddr((struct sockaddr*)&saddr);
            server->udpSend(buf.Peek(), buf.ReadableBytes(), (struct sockaddr*)&saddr, len);

            buf.RetrieveAll();
        });

        server->setCloseConnectionCallback([](const UdpSocketPtr & con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " closed" << std::endl;
            }
        });
        
        server->setWriteCompleteCallback([](const UdpSocketPtr& con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " write complete" << std::endl;
            }
        });
        
        server->Start();

        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}