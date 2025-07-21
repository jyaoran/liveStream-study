/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:13:01
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:31:02
 * @FilePath: /liveStream-study/src/network/TcpServer.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include "network/net/TcpConnection.h"
#include "network/net/EventLoop.h"
#include "network/net/Acceptor.h"
#include "network/base/InetAddress.h"

#include <memory>
#include <functional>
#include <unordered_set>

namespace liveStream
{
    namespace network
    {
        using NewConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
        using DestroyConnectionCallback = std::function<void(const TcpConnectionPtr &)>;

        class TcpServer
        {
        public:
            TcpServer(EventLoop *loop, const InetAddress &listenAddr);
            ~TcpServer();

            // 所有的回调都是为了通知上层的业务层
            void setNewConnectionCallback(const NewConnectionCallback &cb);
            void setNewConnectionCallback(NewConnectionCallback &&cb);
            void setDestroyConnectionCallback(const DestroyConnectionCallback &cb);
            void setDestroyConnectionCallback(DestroyConnectionCallback &&cb);
            void setActiveCallback(const ActiveCallback &cb);
            void setActiveCallback(ActiveCallback &&cb);
            void setWriteCompleteCallback(const wirteCompleteCallback &cb);
            void setWriteCompleteCallback(wirteCompleteCallback &&cb);
            void setMessageCallback(const OnMessageCallback &cb);
            void setMessageCallback(OnMessageCallback &&cb);
            void onAccet(int sockfd, const InetAddress &addr);
            void onConnectionClose(const TcpConnectionPtr &conn);

            virtual void Start();
            virtual void Stop();

        private:
            EventLoop *m_loop{nullptr};
            InetAddress m_listenAddr;
            std::shared_ptr<Acceptor> m_acceptor;
            std::unordered_set<TcpConnectionPtr> m_connections;
            NewConnectionCallback m_newConnectionCallback;
            DestroyConnectionCallback m_destroyConnectionCallback;
            ActiveCallback m_activeCallback;
            wirteCompleteCallback m_writeCompleteCallback;
            OnMessageCallback m_messageCallback;
        };
    } // namespace network
} // namespace liveStream
