/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-24 18:33:43
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-13 18:01:56
 * @FilePath: /liveStream-study/src/network/net/tests/EventLoopThreadTest.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include <iostream>
#include "network/net/EventLoopThreadPool.h"
#include "network/net/EventLoopThread.h"
#include "network/net/EventLoop.h"
#include "network/net/PipeEvent.h"
#include "base/TTime.h"


#include "base/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

using namespace liveStream::network;

EventLoopThread  eventLoopThread;
std::thread threadGlobal;


void testEventLoopThread()
{
    eventLoopThread.run();
    EventLoop* eventLoop = eventLoopThread.loop();
    
    if (eventLoop != nullptr)
    {
        LOG(INFO) << "eventLoopThread run loop = " << eventLoop;

        PipeEvent::ptr pipeEvent = std::make_shared<PipeEvent>(eventLoop);
        eventLoop->addEvent(pipeEvent);

        int64_t data = 12345;
        pipeEvent->write((const char*)&data, sizeof(data));

        threadGlobal = std::thread([&pipeEvent](){
            while (1)
            {
                int64_t now = liveStream::base::TTime::getCurrentTime();
                pipeEvent->write((const char*)&now, sizeof(now));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
        
        while (1)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        
    }
    

}

int main_EventLoopThread(int argc, char const *argv[])
{
    testEventLoopThread();
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}

void testEventLoopThreadPool()
{
    EventLoopThreadPool pool(2, 0, 2);      // 2个线程，0cpu开始，2个cpu跑

    pool.Start();

    LOG(INFO) << "thread id : "<< std::this_thread::get_id();

    std::vector<EventLoop*> list = pool.getLoops();

    for(auto &e : list)
    {
        // 打印不同的线程loop
        // LOG(INFO) << "loop : " << e;   
        e->runInLoop([&e](){
            LOG(INFO) << "loop : " << e << "   this id = " << std::this_thread::get_id(); 
        });
    }

    EventLoop* loop = pool.getNextLoop();
    LOG(INFO) << "loop : " << loop; 
    loop = pool.getNextLoop();
    LOG(INFO) << "loop : " << loop; 
}

void TestTimingWheelInThreadLoop()
{
    EventLoopThreadPool pool(2, 0, 2);      // 2个线程，0cpu开始，2个cpu跑
    pool.Start();
    EventLoop* loop = pool.getNextLoop();
    LOG(INFO) << "loop : " << loop; 

    // 设置任务
        loop->runEvery([](){
        LOG(INFO) << "run every 1s now : " << liveStream::base::TTime::getCurrentTime();
    }, 1);
    
    loop->runAfter([](){
        LOG(INFO) << "run after 1s now : " << liveStream::base::TTime::getCurrentTime();
    }, 1);

    loop->runAfter([](){
        LOG(INFO) << "run after 5s now : " << liveStream::base::TTime::getCurrentTime();
    }, 5);

    loop->runEvery([](){
        LOG(INFO) << "run every 5s now : " << liveStream::base::TTime::getCurrentTime();
    }, 5);

    while(1)  // 退出函数线程池就析构了
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    testEventLoopThreadPool();
    // TestTimingWheelInThreadLoop();
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
