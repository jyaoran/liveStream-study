/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 13:46:15
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 23:18:35
 * @FilePath: /liveStream-study/src/mmedia/tests/RtmpClientTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/base/InetAddress.h"
#include "mmedia/rtmp/RtmpClient.h"
#include "network/TcpClient.h"
#include "base/easylogging++.h"
#include "base/Config.h"

#include <iostream>
#include <thread>
#include <chrono>

using namespace liveStream::network;
using namespace liveStream::base;
using namespace liveStream::mm;

INITIALIZE_EASYLOGGINGPP

EventLoopThread eventloop_thread;
std::thread th;

class RtmpHandlerImpl : public RtmpHandler
{
    public:
        // 播放
        bool OnPlay(const TcpConnectionPtr &conn, const std::string &session_name, const std::string &param)
        {
            return false;
        }
        // 推流
        bool OnPublish(const TcpConnectionPtr &conn, const std::string &session_name, const std::string &param)
        {
            return false;
        }
        // 暂停
        bool OnPause(const TcpConnectionPtr &conn, bool pause)
        {
            return false;
        }
        // 定位(快进等)≈
        void OnSeek(const TcpConnectionPtr& conn, double time)
        {
            
        }

        void OnNewConnection(const TcpConnectionPtr &conn) override
        {

        } // 新连接的时候，直播业务就可以处理数据，比如创建用户等
        void OnConnectionDestroy(const TcpConnectionPtr &conn) override
        {

        } // 连接断开的时候，业务层可以回收资源，注销用户等
        void OnRecv(const TcpConnectionPtr& conn, const PacketPtr &data) override
        {
            LOG(INFO) << "recv type:" << data->PacketType() << " size:" << data->PacketSize();
        }// 多媒体解析出来的数据，传给直播业务
        void OnRecv(const TcpConnectionPtr& conn, PacketPtr &&data) override
        {
            LOG(INFO) << "recv type:" << data->PacketType() << " size:" << data->PacketSize();
        }
        void OnActive(const ConnectionPtr &conn) override
        {

        }   // 新连接回调通知直播业务
};

int main()
{
    el::Configurations conf("/home/yaoran/3dPart/logConf/my-conf.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    // if(!sConfigMgr->LoadConfig("../config/config.json"))
    // {
    //     std::cerr << "load config file failed.";
    //     return -1;
    // }
    
    // Config::ptr config = sConfigMgr->GetConfig();


    eventloop_thread.run();
    EventLoop* loop = eventloop_thread.loop();

    if(loop)
    {
        RtmpClient client(loop, new RtmpHandlerImpl());
        client.Play("rtmp://127.0.0.1:1935/live/test");
        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}