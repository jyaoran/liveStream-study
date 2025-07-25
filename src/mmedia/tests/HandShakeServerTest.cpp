/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 22:08:50
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 22:38:53
 * @FilePath: /liveStream-study/src/mmedia/tests/HandShakeServerTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/TcpServer.h"
#include "mmedia/rtmp/RtmpHandShake.h"
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
        TcpServer server(loop, listen);

        // 设置收到消息回调，直接解析消息的上下文
        server.setMessageCallback([](const TcpConnectionPtr& con, MsgBuffer &buf){
            RtmpHandShakePtr shake = con->getContext<RtmpHandShake>(kNormalContext);
            // 每次收到消息就进行回调
            shake->HandShake(buf);
        });

        // 设置有新连接的时候，设置上下文到conn上，设置上下文解析完成之后的回调
        server.setNewConnectionCallback([&loop](const TcpConnectionPtr &con){
            RtmpHandShakePtr shake = std::make_shared<RtmpHandShake>(con, false);
            con->setContext(kNormalContext, shake);
            // 开始服务端等待C0C1
            shake->Start();
            con->setWriteCompleteCallback([&loop](const TcpConnectionPtr& con){
                LOG(INFO) << "write complete host： " << con->getPeerAddress().toIpPort();
                RtmpHandShakePtr shake = con->getContext<RtmpHandShake>(kNormalContext);
                // 驱动状态机，每次写完都会启动这个回调
                shake->WriteComplete();
            });
        });

        server.Start();

        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}