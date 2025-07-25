/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 13:46:15
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 16:17:29
 * @FilePath: /liveStream-study/src/mmedia/tests/RtmpServerTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/TcpServer.h"
#include "mmedia/rtmp/RtmpHandShake.h"
#include "mmedia/rtmp/RtmpServer.h"
#include "base/easylogging++.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;
using namespace liveStream::mm;

INITIALIZE_EASYLOGGINGPP

EventLoopThread eventloop_thread;
std::thread th;
using RtmpHandShakePtr = std::shared_ptr<RtmpHandShake>;



const char *http_response = "HTTP/1.0 200 OK\r\nServer: tmms\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
int main()
{

    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    eventloop_thread.run();
    EventLoop* loop = eventloop_thread.loop();

    if(loop)
    {
        InetAddress listen("127.0.0.1:1935");
        std::cout << "addr : " << listen.toIpPort() << std::endl;
        RtmpServer server(loop, listen);
        // 整个Rtmp的回调都设置在里面了，直接启动就行
        // rtmp设置自己的业务流程回调函数到tcpServer中
        // tcpServer设置这个回调到accept和tcpconnection的回调，然后进行调用
        server.Start();

        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}