/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-15 16:28:28
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-27 16:18:25
 * @FilePath: /liveStream-study/src/network/net/Event.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include <sys/epoll.h>
#include <string>
#include <memory>


namespace liveStream 
{
    namespace network
    {
        // 前向声明 EventLoop 类
        class EventLoop;
        
        // 定义读事件的标志，包括 EPOLLIN（可读）、EPOLLPRI（高优先级可读）、EPOLLET（边缘触发）
        const int kEventRead = (EPOLLIN | EPOLLPRI | EPOLLET);
        
        // 定义写事件的标志，包括 EPOLLOUT（可写）、EPOLLET（边缘触发）
        const int kEventWrite = (EPOLLOUT | EPOLLET);

        // 定义 Event 类，它继承自 std::enable_shared_from_this<Event>
        // 这使得 Event 对象可以通过 std::shared_ptr 进行动态管理
        class Event : public std::enable_shared_from_this<Event>
        {
            // 声明 EventLoop 为友元类，这样 EventLoop 可以访问 Event 的私有成员
            friend class EventLoop;
        
        public:
            // 定义 ptr 为 std::shared_ptr<Event> 的类型别名
            using ptr = std::shared_ptr<Event>;

            // 默认构造函数
            Event();
            
            // 构造函数，接受一个 EventLoop 指针
            Event(EventLoop* loop);
            
            // 构造函数，接受一个 EventLoop 指针和一个文件描述符
            Event(EventLoop* loop, int fd);
            
            // 虚析构函数，确保派生类的析构函数被正确调用
            virtual ~Event();

            // 虚拟的 onRead 方法，当文件描述符可读时调用
            virtual void onRead() {}
            
            // 虚拟的 onWrite 方法，当文件描述符可写时调用
            virtual void onWrite() {}
            
            // 虚拟的 onClose 方法，当文件描述符关闭时调用
            virtual void onClose() {}
            
            // 虚拟的 onError 方法，当发生错误时调用，并传递错误信息
            virtual void onError(const std::string &msg) {}

            // 启用或禁用读事件
            bool enableRead(bool enable);
            
            // 启用或禁用写事件
            bool enableWrite(bool enable);

            // 返回当前事件关联的文件描述符
            int fd() const;
            
            // 关闭当前事件关联的文件描述符
            void close();
        
        protected:
            // 指向 EventLoop 对象的指针，初始值为 nullptr
            EventLoop *m_loop{nullptr};
            
            // 文件描述符，初始值为 -1，表示没有关联的文件描述符
            int m_fd{-1};
            
            // 事件标志，初始值为 0，表示没有设置任何事件
            int m_event{0};
        };
    }
}
