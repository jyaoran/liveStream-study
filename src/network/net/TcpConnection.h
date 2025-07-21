/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-28 18:38:51
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-30 17:09:42
 * @FilePath: /liveStream-study/src/network/net/TcpConnection.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include "Connection.h"
#include "network/base/InetAddress.h"
#include "network/base/MsgBuffer.h"

#include <functional>
#include <memory>
#include <list>
#include <sys/uio.h>

namespace liveStream
{
    namespace network
    {
        class TimeoutEntry;
        class TcpConnection;
        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using CloseConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
        using OnMessageCallback = std::function<void(const TcpConnectionPtr &, MsgBuffer &)>;
        using wirteCompleteCallback = std::function<void(const TcpConnectionPtr &)>;
        using TimeoutCallback = std::function<void(const TcpConnectionPtr &)>;

        class TcpConnection : public Connection
        {
        public:
            using ptr = std::shared_ptr<TcpConnection>;
            TcpConnection(EventLoop *loop, int sockfd,
                          const InetAddress &local, const InetAddress &peer);
            ~TcpConnection();

            // 事件处理方法

            // 1、关闭事件
            void setCloseCallback(const CloseConnectionCallback &cb);
            void setCloseCallback(CloseConnectionCallback &&cb);
            void onClose() override;          // 关闭事件处理
            void forceClose() override;       // 强制关闭连接

            // 2、读事件
            void setOnMessageCallback(const OnMessageCallback &cb);
            void setOnMessageCallback(OnMessageCallback &&cb);
            void onRead() override;

            // 3、写事件
            void setWriteCompleteCallback(const wirteCompleteCallback &cb);
            void setWriteCompleteCallback(wirteCompleteCallback &&cb);
            void onWrite() override;
            void send(std::list<BufferNodePtr> &msgList); // 发送消息
            void send(const void *data, size_t size); // 发送数据

            // 4、超时事件
            void setTimeoutCallback(int timeout, const TimeoutCallback &cb);
            void setTimeoutCallback(int timeout, TimeoutCallback &&cb);
            void onTimeout(); // 超时事件处理
            void enableCheckIdleTimeout(uint32_t maxTime); // 开启空闲超时检测

            // error处理
            void onError(const std::string &msg) override; // 错误处理


        private:
            void sendInLoop(std::list<BufferNodePtr> &msgList); // 发送消息
            void sendInLoop(const void *data, size_t size); // 发送数据
            
            void extendLifetime(); // 延长生命周期
            
            bool m_closed = false; // 连接是否关闭
            CloseConnectionCallback m_CloseCallback;       // 关闭回调
            OnMessageCallback m_OnMessageCallback;         // 接收消息回调
            wirteCompleteCallback m_WriteCompleteCallback; // 写完成回调
            // TimeoutCallback m_TimeoutCallback;             // 超时回调

            MsgBuffer m_messageBuffer; // 读缓存
            std::vector<iovec> m_iovecList; // 写缓存
            std::weak_ptr<TimeoutEntry> m_timeoutEntry; // 超时对象
            uint32_t m_maxIdleTime = 30; // 最大空闲时间，单位秒
        };

        struct TimeoutEntry
        {
        public:
            TimeoutEntry(const TcpConnectionPtr& conn) : m_connection(conn) {}
        
            ~TimeoutEntry()
            {
                auto conn = m_connection.lock();
                if (conn)
                {
                    conn->onTimeout();
                }
            }
        private:
            std::weak_ptr<TcpConnection> m_connection;
        };

    } // namespace network
} // namespace liveStream