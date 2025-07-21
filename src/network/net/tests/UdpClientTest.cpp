/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-11 14:57:56
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 15:09:22
 * @FilePath: /liveStream-study/src/network/net/tests/UdpClientTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/UdpClient.h"
#include "base/easylogging++.h"

#include <iostream>
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
        InetAddress server("127.0.0.1:34444");
        // 继承了std::enable_shared_from_this<Event>必须使用智能指针
        std::shared_ptr<UdpClient> client = std::make_shared<UdpClient>(loop, server);
        
        client->setOnMessageCallback([](const UdpSocketPtr& con, const InetAddress &addr, MsgBuffer &buf){
            std::cout << "host : " << addr.toIpPort() << " \nmsg : "<< buf.Peek() << std::endl;
            buf.RetrieveAll();
        });

        client->setCloseConnectionCallback([](const UdpSocketPtr & con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " closed" << std::endl;
            }
        });
        
        client->setWriteCompleteCallback([](const UdpSocketPtr& con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " write complete" << std::endl;
            }
        });

        client->setConnectionCallback([&client](const UdpSocketPtr& con, bool connected){
            if(connected)
            {
                std::cout << "host: " << con->getPeerAddress().toIpPort() << " connected." << std::endl;
                client->send("1111", strlen("1111"));
            }
        });
        client->connect();

        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}