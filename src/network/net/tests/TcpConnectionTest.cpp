/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-30 12:09:34
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 22:28:34
 * @FilePath: /liveStream-study/src/network/net/tests/TcpConnectionTest.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "network/net/TcpConnection.h"

#include "base/TaskManager.h"
#include "base/TTime.h"
#include "base/easylogging++.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;

EventLoopThread eventloop_thread;
std::thread th;

INITIALIZE_EASYLOGGINGPP

const char *http_response = "HTTP/1.0 200 OK\r\nServer: tmms\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
int main()
{

    el::Configurations conf("./my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);
    
    eventloop_thread.run();
    EventLoop* loop = eventloop_thread.loop();

    if(loop)
    {
        std::vector<TcpConnectionPtr> list;

        InetAddress server("0.0.0.0:34444");
        std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(loop, server); 
        // accept回调，每个accept都创建一个tcpconnetion
        acceptor->setAcceptCallback([&loop, &server, &list](int fd, const InetAddress &addr)
        {
            LOG(INFO) << "host : " << addr.toIpPort();
            TcpConnectionPtr connection = std::make_shared<TcpConnection>(loop, fd, server, addr);
            // 收到消息的回调
            connection->setOnMessageCallback([](const TcpConnectionPtr& conn, MsgBuffer& buffer){
                LOG(INFO) << "recv msg: " << buffer.Peek();
                buffer.RetrieveAll();
                conn->send(http_response, strlen(http_response));
            });
            // 写完成回调
            connection->setWriteCompleteCallback([&loop](const TcpConnectionPtr& conn){
                LOG(INFO) << "write complete host : " << conn->getPeerAddress().toIpPort();
                // loop->removeEvent(conn);
                // conn->forceClose();
            });

            // 测试超时事件
            connection->enableCheckIdleTimeout(3);
            list.push_back(connection);
            loop->addEvent(connection); // 加入到事件循环中监听，epoll调用OnRead，调用回调
        });
        acceptor->Start();

        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}