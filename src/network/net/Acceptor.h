/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-15 16:28:16
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-27 16:32:39
 * @FilePath: /liveStream-study/src/network/net/Acceptor.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once

#include "network/base/SocketOpt.h"
#include "network/base/InetAddress.h"
#include "network/net/Event.h"
#include "network/net/EventLoop.h"

#include <functional>

namespace liveStream 
{
    namespace network 
    {
        using AcceptorCallback = std::function<void(int sockfd, const InetAddress& addr)>;

        class Acceptor : public Event
        {
        public:
            Acceptor(EventLoop* loop, const InetAddress& listenAddr);
            ~Acceptor();

            void setAcceptCallback(const AcceptorCallback& cb);
            void setAcceptCallback(AcceptorCallback&& cb);

            void Start();
            void Stop();

            void onRead() override;
            void onError(const std::string& msg) override;
            void onClose() override;

        private:
            void open();
            InetAddress m_addr;
            AcceptorCallback m_acceptCallback;
            SocketOpt *m_socketOpt{nullptr};
        };
    } // namespace network
} // namespace liveStream

