/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-11 14:19:53
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:29:27
 * @FilePath: /liveStream-study/src/network/net/tests/TcpClientTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/TcpClient.h"
#include "base/easylogging++.h"

#include <iostream>
#include <thread>
#include <chrono>


using namespace liveStream::network;
using namespace liveStream::base;

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
        InetAddress server("127.0.0.1:34444");
        // 继承了std::enable_shared_from_this<Event>必须使用智能指针
        std::shared_ptr<TcpClient> client = std::make_shared<TcpClient>(loop, server);
        client->setOnMessageCallback([](const TcpConnectionPtr& con, MsgBuffer &buf){
            std::cout << "host : " << con->getPeerAddress().toIpPort() << "msg : "<< buf.Peek() << std::endl;
            buf.RetrieveAll();

            con->send(http_response, strlen(http_response));
        });

        client->setCloseCallback([](const TcpConnectionPtr & con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " closed" << std::endl;
            }
        });
        
        client->setWriteCompleteCallback([](const TcpConnectionPtr& con){
            if(con)
            {
                std::cout << "host: "<< con->getPeerAddress().toIpPort() << " write complete" << std::endl;
            }
        });

        client->setConnectionCallback([](const TcpConnectionPtr& con, bool connected){
            if(connected)
            {
                // 小端转大端
                auto size = htonl(strlen(http_request));
                con->send((const char*)&size, sizeof(size));
                con->send(http_request, strlen(http_request));
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