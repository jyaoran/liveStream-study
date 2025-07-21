/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-14 21:40:29
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:07:47
 * @FilePath: /liveStream-study/src/network/base/SocketOpt.h
 * @Description: socket接口的封装，以便于InetAddress的使用
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

 #pragma once
 
 #include "InetAddress.h"
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <netinet/tcp.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <fcntl.h>

 #include <memory>


namespace liveStream 
{
    namespace network 
    {
        using InetAddressPtr = std::shared_ptr<InetAddress>;
        class SocketOpt 
        {
        public:
            SocketOpt(int sock, bool v6 = false);
            ~SocketOpt() = default;
            
            // 创建接口
            static int createNonBlockTcpsocket(int family);
            static int createNonBlockUdpsocket(int family);
            
            // 服务器
            int bindAddress(const InetAddress &localaddr);
            int listen();
            int accept(InetAddress *peeraddr);

            // 客户端
            int connect(const InetAddress &addr);

            // 获取地址
            InetAddressPtr getLocalAddress() const;
            InetAddressPtr getPeerAddress() const;

            // 使能函数
            void setTcpNoDelay(bool on);
            void setReuseAddr(bool on);
            void setReusePort(bool on);
            void setKeepAlive(bool on);
            void setNonBlocking(bool on);

        private:
            int m_sock{-1};
            bool m_ipv6{false};
        };
    }
} 
