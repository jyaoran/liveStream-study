/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-11 14:23:10
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:26:58
 * @FilePath: /liveStream-study/src/network/net/tests/TcpServerTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/TcpServer.h"
#include "network/TestContext.h"
#include "base/easylogging++.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;

EventLoopThread eventloop_thread;
std::thread th;
using TestContextPtr = std::shared_ptr<TestContext>;

INITIALIZE_EASYLOGGINGPP

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
        InetAddress listen("0.0.0.0:34444");
        TcpServer server(loop, listen);

        // 设置收到消息回调，直接解析消息的上下文
        server.setMessageCallback([](const TcpConnectionPtr& con, MsgBuffer &buf){
            TestContextPtr context = con->getContext<TestContext>(kNormalContext);
            context->ParseMessage(buf);
        });

        // 设置有新连接的时候，设置上下文到conn上，设置上下文解析完成之后的回调
        server.setNewConnectionCallback([&loop](const TcpConnectionPtr &con){
            TestContextPtr context = std::make_shared<TestContext>(con);
            con->setContext(kNormalContext, context);
            context->SetTestMessageCallback([](const TcpConnectionPtr& con, const std::string &msg){
                std::cout << "message : "<< msg << std::endl;
            });
            con->setWriteCompleteCallback([&loop](const TcpConnectionPtr& con){
                std::cout << "write complete host： " << con->getPeerAddress().toIpPort() << std::endl;
                // con->ForceClose();
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