/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-14 21:43:46
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 23:37:46
 * @FilePath: /liveStream-study/src/network/base/InetAddress.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bits/socket.h>

#include <string>
#include <memory>

namespace liveStream
{
    namespace network
    {
        class InetAddress
        {
        public:
            using ptr = std::shared_ptr<InetAddress>;
            
            InetAddress() = default;
            InetAddress(const std::string &ip, std::string &port, bool ipv6 = false);
            InetAddress(const std::string &host, bool is_ipv6 = false);
            ~InetAddress() = default;

            // 赋值函数
            void setAddr(const std::string &addr);
            void setHost(const std::string &host);
            void setPort(uint16_t port);
            void setIsIpv6(bool ipv6);

            // 取值函数
            const std::string &getAddr() const;
            uint32_t getIpv4() const;
            std::string toIpPort() const;
            uint16_t getPort() const;
            void getSockAddr(struct sockaddr *addr) const;

            // 测试函数
            bool isIpv6() const;
            bool isWanIp() const;
            bool isLanIp() const;
            bool isLoopbackIp() const;

            static void getIpAndPort(const std::string &host, std::string &ip, std::string &port);

        private:
            // 私有函数
            uint32_t parseIpv4(const char *ip) const;

            std::string m_addr;
            std::string m_port;
            bool m_ipv6{false};
        };
    }
}