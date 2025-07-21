/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-14 21:43:26
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:07:35
 * @FilePath: /liveStream-study/src/network/base/SocketOpt.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "Network.h"
#include "SocketOpt.h"

using namespace liveStream::network;

SocketOpt::SocketOpt(int sock, bool v6):m_sock(sock), m_ipv6(v6)
{
}

int SocketOpt::createNonBlockTcpsocket(int family)
{
    // SOCK_STREAM ： TCP socket
    // SOCK_NONBLOCK ： non-blocking mode
    // SOCK_CLOEXEC ： 这个标志位在创建套接字时设置，使得在执行 exec 系统调用（如 execve）时，目标套接字会自动关闭。这是一种防止文件描述符泄露的安全措施。
    // IPPROTO_TCP ： 这个参数指定了协议族，这里指定的是 TCP 协议族。
    int sockfd = ::socket(family, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0) 
    {
        LOG(ERROR) << "create nonblock tcp socket failed";
        return -1;
    }
    return sockfd;
}

int SocketOpt::createNonBlockUdpsocket(int family)
{
    // SOCK_DGRAM ： UDP socket
    // SOCK_NONBLOCK ： non-blocking mode
    // SOCK_CLOEXEC ： 这个标志位在创建套接字时设置，使得在执行 exec 系统调用（如 execve）时，目标套接字会自动关闭。这是一种防止文件描述符泄露的安全措施。
    // IPPROTO_UDP ： 这个参数指定了协议族，这里指定的是 UDP 协议族。
    int sockfd = ::socket(family, SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_UDP);
    if (sockfd < 0) 
    {
        LOG(ERROR) << "create nonblock udp socket failed";
        return -1;
    }
    return sockfd;
}

int SocketOpt::bindAddress(const InetAddress &localaddr)
{
    if (localaddr.isIpv6())
    {
        struct sockaddr_in6 addr6;
        localaddr.getSockAddr((struct sockaddr *)&addr6);
        socklen_t addrlen = sizeof(struct sockaddr_in6);
        if (::bind(m_sock, (struct sockaddr *)&addr6, addrlen) < 0)
        {
            LOG(ERROR) << "bind ipv6 address failed";
            return -1;
        }
    }
    else
    {
        struct sockaddr_in addr4;
        localaddr.getSockAddr((struct sockaddr *)&addr4);
        socklen_t addrlen = sizeof(struct sockaddr_in);
        if (::bind(m_sock, (struct sockaddr *)&addr4, addrlen) < 0)
        {
            LOG(ERROR) << "bind ipv4 address failed";
            return -1;
        }
    }
    
    return 0;
}

int SocketOpt::listen()
{
    // SOMAXCONN ： 这个参数指定了 listen 函数接受的最大连接数。
    return ::listen(m_sock, SOMAXCONN);
}

int SocketOpt::accept(InetAddress *peeraddr)
{
    struct sockaddr_in6 addr;
    socklen_t addrlen = sizeof(struct sockaddr_in6);
    int connfd = ::accept4(m_sock, (struct sockaddr *)&addr, &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd > 0)
    {
        if (addr.sin6_family == AF_INET)   
        {
            char ip[INET_ADDRSTRLEN];
            struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;
            if (inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN) == NULL)
            {
                LOG(ERROR) << "inet_ntop failed";
                return -1;
            }
            peeraddr->setAddr(ip);
            peeraddr->setPort(ntohs(addr_in->sin_port));
        }
        else if (addr.sin6_family == AF_INET6)
        {
            char ip[INET6_ADDRSTRLEN];
            struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr;
            if (inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip, INET6_ADDRSTRLEN) == NULL)
            {
                LOG(ERROR) << "inet_ntop failed";
                return -1;
            }
            peeraddr->setAddr(ip);
            peeraddr->setPort(ntohs(addr_in6->sin6_port));
            peeraddr->setIsIpv6(true);
        }
    }

    return connfd;
}

// 客户端连接服务器
int SocketOpt::connect(const InetAddress &addr)
{
    struct sockaddr_in6 in_addr;
    addr.getSockAddr((struct sockaddr *)&in_addr);

    return ::connect(m_sock, (struct sockaddr *)&in_addr, sizeof(struct sockaddr_in6));
}

InetAddressPtr SocketOpt::getLocalAddress() const
{
    struct sockaddr_in6 addr;
    socklen_t addrlen = sizeof(struct sockaddr_in6);
    ::getsockname(m_sock, (struct sockaddr *)&addr, &addrlen);
    InetAddressPtr localaddr = std::make_shared<InetAddress>();
    if (addr.sin6_family == AF_INET)
    {
        char ip[INET_ADDRSTRLEN];
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;
        if (inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN) == NULL)
        {
            LOG(ERROR) << "inet_ntop failed";
            return nullptr;
        }
        localaddr->setAddr(ip);
        localaddr->setPort(ntohs(addr_in->sin_port));
    }
    else if (addr.sin6_family == AF_INET6)
    {
        char ip[INET6_ADDRSTRLEN];
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr;
        if (inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip, INET6_ADDRSTRLEN) == NULL)
        {
            LOG(ERROR) << "inet_ntop failed";
            return nullptr;
        }
        localaddr->setAddr(ip);
        localaddr->setPort(ntohs(addr_in6->sin6_port));
        localaddr->setIsIpv6(true);
    }
    return localaddr;
}

InetAddressPtr SocketOpt::getPeerAddress() const
{
    struct sockaddr_in6 addr;
    socklen_t addrlen = sizeof(struct sockaddr_in6);
    ::getpeername(m_sock, (struct sockaddr *)&addr, &addrlen);
    InetAddressPtr peeraddr = std::make_shared<InetAddress>();
    if (addr.sin6_family == AF_INET)
    {
        char ip[INET_ADDRSTRLEN];
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr;
        if (inet_ntop(AF_INET, &addr_in->sin_addr, ip, INET_ADDRSTRLEN) == NULL)
        {
            LOG(ERROR) << "inet_ntop failed";
            return nullptr;
        }
        peeraddr->setAddr(ip);
        peeraddr->setPort(ntohs(addr_in->sin_port));
    }
    else if (addr.sin6_family == AF_INET6)
    {
        char ip[INET6_ADDRSTRLEN];
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr;
        if (inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip, INET6_ADDRSTRLEN) == NULL)
        {
            LOG(ERROR) << "inet_ntop failed";
            return nullptr;
        }
        peeraddr->setAddr(ip);
        peeraddr->setPort(ntohs(addr_in6->sin6_port));
        peeraddr->setIsIpv6(true);
    }

    return peeraddr;
}

void SocketOpt::setTcpNoDelay(bool on)
{
    int flag = on ? 1 : 0;
    ::setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
}

void SocketOpt::setReuseAddr(bool on)
{
    int flag = on ? 1 : 0;
    ::setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
}

void SocketOpt::setReusePort(bool on)
{
    int flag = on ? 1 : 0;
    ::setsockopt(m_sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(int));
}

void SocketOpt::setKeepAlive(bool on)
{
    int flag = on ? 1 : 0;
    ::setsockopt(m_sock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
}

void SocketOpt::setNonBlocking(bool on)
{
    int flag = ::fcntl(m_sock, F_GETFL, 0);
    if (on)
    {
        flag |= O_NONBLOCK;
    }
    else
    {
        flag &= ~O_NONBLOCK;
    }
    ::fcntl(m_sock, F_SETFL, flag);

}
