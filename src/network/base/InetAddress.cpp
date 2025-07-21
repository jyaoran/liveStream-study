/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-14 21:43:58
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 22:55:20
 * @FilePath: /liveStream-study/src/network/base/InetAddress.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "Network.h"
#include "InetAddress.h"
#include <cstring>

using namespace liveStream::network;

InetAddress::InetAddress(const std::string &ip, std::string &port, bool ipv6)
:m_addr(ip), m_port(port), m_ipv6(ipv6)
{
}

InetAddress::InetAddress(const std::string &host, bool is_ipv6)
{
    getIpAndPort(host, m_addr, m_port);
    m_ipv6 = is_ipv6;
}

void InetAddress::setAddr(const std::string &addr)
{
    m_addr = addr;
}

void InetAddress::setHost(const std::string &host)
{
    getIpAndPort(host, m_addr, m_port);
}

void InetAddress::setPort(uint16_t port)
{
    m_port = std::to_string(port);
}

void InetAddress::setIsIpv6(bool ipv6)
{
    m_ipv6 = ipv6;
}

const std::string &InetAddress::getAddr() const
{
    return m_addr;
}

uint32_t InetAddress::getIpv4() const
{
    return parseIpv4(m_addr.c_str());
}

std::string InetAddress::toIpPort() const
{
    return (m_addr + ":" + m_port);
}

uint16_t InetAddress::getPort() const
{
    return atoi(m_port.c_str());
}

void InetAddress::getSockAddr(struct sockaddr *addr) const
{
    if (m_ipv6)
    {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
        memset(addr_in6, 0, sizeof(struct sockaddr_in6));

        addr_in6->sin6_family = AF_INET6;
        addr_in6->sin6_port = htons(getPort());
        if (inet_pton(AF_INET6, m_addr.c_str(), &addr_in6->sin6_addr) < 0)
        {
            LOG(ERROR) << "inet_pton failed! " 
                << "ipv6 ip : " << m_addr << " is error.";
        }
        return;
    }

    struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
    addr_in->sin_family = AF_INET;
    addr_in->sin_port = htons(getPort());
    if (inet_pton(AF_INET, m_addr.c_str(), &addr_in->sin_addr) < 0)
    {
        LOG(ERROR) << "inet_pton failed! " 
            << "ipv4 ip : " << m_addr << " is error.";
    }
}

bool InetAddress::isIpv6() const
{
    return m_ipv6;
}

bool InetAddress::isWanIp() const
{
    uint32_t a_start = parseIpv4("10.0.0.0");
    uint32_t a_end = parseIpv4("10.255.255.255");
    uint32_t b_start = parseIpv4("172.16.0.0");
    uint32_t b_end = parseIpv4("172.31.255.255");
    uint32_t c_start = parseIpv4("192.168.0.0");
    uint32_t c_end = parseIpv4("192.168.255.255");

    uint32_t ip = parseIpv4(m_addr.c_str());
    bool is_a  = ip >= a_start && ip <= a_end;
    bool is_b  = ip >= b_start && ip <= b_end;
    bool is_c  = ip >= c_start && ip <= c_end;
    
    return !is_a && !is_b && !is_c && ip != INADDR_LOOPBACK; 
}

bool InetAddress::isLanIp() const
{
    uint32_t a_start = parseIpv4("10.0.0.0");
    uint32_t a_end = parseIpv4("10.255.255.255");
    uint32_t b_start = parseIpv4("172.16.0.0");
    uint32_t b_end = parseIpv4("172.31.255.255");
    uint32_t c_start = parseIpv4("192.168.0.0");
    uint32_t c_end = parseIpv4("192.168.255.255");

    uint32_t ip = parseIpv4(m_addr.c_str());
    bool is_a  = ip >= a_start && ip <= a_end;
    bool is_b  = ip >= b_start && ip <= b_end;
    bool is_c  = ip >= c_start && ip <= c_end;
    
    return  is_a || is_b || is_c; 
}

bool InetAddress::isLoopbackIp() const
{
    return m_addr == "127.0.0.1" || m_addr == "::1";
}

void InetAddress::getIpAndPort(const std::string &host, std::string &ip, std::string &port)
{
    auto pos = host.find(':', 0);
    if (pos != std::string::npos)
    {
        ip = host.substr(0, pos);
        port = host.substr(pos + 1);
    }
    else
    {
        ip = host;
        port = "0";
    }
}

uint32_t InetAddress::parseIpv4(const char *ip) const
{
    struct sockaddr_in addr_int;
    memset(&addr_int, 0, sizeof(addr_int));
    addr_int.sin_family = AF_INET;
    addr_int.sin_port = htons(0);
    if (inet_pton(AF_INET, ip, &addr_int.sin_addr) != 1)
    {
        LOG(ERROR) << "inet_pton failed! " 
            << "ipv4 ip : " << ip << " is error.";
        return 0;
    }
    return ntohl(addr_int.sin_addr.s_addr);
}
