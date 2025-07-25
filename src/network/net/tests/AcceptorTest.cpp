/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-27 16:51:32
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 22:36:43
 * @FilePath: /liveStream-study/src/network/net/tests/AcceptorTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"

#include "base/easylogging++.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;

INITIALIZE_EASYLOGGINGPP

EventLoopThread t;
std::thread thread;

int main(int argc, char const *argv[])
{
#if 1

    EventLoop loop;

    InetAddress server("127.0.0.1:34444");
    std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(&loop, server); 
    acceptor->setAcceptCallback([](int fd, const InetAddress &addr){
        LOG(INFO) << "host : " << addr.toIpPort();
    });

    acceptor->Start();

    loop.loop();
#else 
    t.run();
    EventLoop *loop = t.loop();

    if(loop)
    {
        InetAddress server("127.0.0.1:34444");
        std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(loop, server); 
        acceptor->setAcceptCallback([](int fd, const InetAddress &addr){
            LOG(INFO) << "host : " << addr.toIpPort();
        });
        acceptor->Start();
    }

#endif
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
