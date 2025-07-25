/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 22:22:24
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 18:51:44
 * @FilePath: /liveStream-src/home/yaoran/gitpro/liveStream-study/src/mmedia/tests/HandShakeClientTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "mmedia/rtmp/RtmpHandShake.h"
#include "network/TcpClient.h"
#include "base/easylogging++.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;
using namespace liveStream::mm;
using RtmpHandShakePtr = std::shared_ptr<RtmpHandShake>;


INITIALIZE_EASYLOGGINGPP

EventLoopThread eventloop_thread;
std::thread th;

const char *http_request = "GET / HTTP/1.0\r\nHost: 10.101.128.69\r\nAccept: */*\r\nContent-Length: 0\r\n\r\n";
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
        InetAddress server("127.0.0.1:1935");
        // 继承了std::enable_shared_from_this<Event>必须使用智能指针
        std::shared_ptr<TcpClient> client = std::make_shared<TcpClient>(loop, server);
        client->setOnMessageCallback([](const TcpConnectionPtr& con, MsgBuffer &buf){
            RtmpHandShakePtr shake = con->getContext<RtmpHandShake>(kNormalContext);
            // 每次收到消息就进行回调
            shake->HandShake(buf);
        });

        client->setCloseCallback([](const TcpConnectionPtr & con){
            if(con)
            {
                LOG(INFO)  << "host: "<< con->getPeerAddress().toIpPort() << " closed";
            }
        });
        
        // 写完成要调用
        client->setWriteCompleteCallback([](const TcpConnectionPtr& con){
            if(con)
            {
                LOG(INFO) << "host: "<< con->getPeerAddress().toIpPort() << " write complete";
                RtmpHandShakePtr shake = con->getContext<RtmpHandShake>(kNormalContext);
                shake->WriteComplete();
            }
        });

        // 建立连接之后，设置上下文
        client->setConnectionCallback([](const TcpConnectionPtr& con, bool connected){
            if(connected)
            {
                RtmpHandShakePtr shake = std::make_shared<RtmpHandShake>(con, true);
                con->setContext(kNormalContext, shake);
                // 开始客户端发送C0C1
                shake->Start();
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