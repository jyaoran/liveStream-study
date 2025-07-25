/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:26:50
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 16:00:44
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpServer.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "RtmpServer.h"
#include "mmedia/base/MMediaLog.h"
#include "mmedia/rtmp/RtmpContext.h"

using namespace liveStream::mm;

RtmpServer::RtmpServer(EventLoop *loop, const InetAddress &local, RtmpHandler *handler)
    : TcpServer(loop, local), rtmp_handler_(handler)
{
    if (rtmp_handler_ == nullptr)
    {
        RTMP_ERROR << "RtmpHandler is nullptr, RtmpServer will not handle any rtmp request";
    }
}

RtmpServer::~RtmpServer()
{
}

void RtmpServer::Start()
{
    TcpServer::setActiveCallback(std::bind(&RtmpServer::OnActive, this, std::placeholders::_1));
    TcpServer::setDestroyConnectionCallback(std::bind(&RtmpServer::OnConnectionDestroy, this, std::placeholders::_1));
    TcpServer::setNewConnectionCallback(std::bind(&RtmpServer::OnNewConnection, this, std::placeholders::_1));
    TcpServer::setWriteCompleteCallback(std::bind(&RtmpServer::OnWriteComplete, this, std::placeholders::_1));
    TcpServer::setMessageCallback(std::bind(&RtmpServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));
    TcpServer::Start();
}
void RtmpServer::Stop()
{
    TcpServer::Stop();
}

void RtmpServer::OnNewConnection(const TcpConnectionPtr &conn)
{
    if (rtmp_handler_)
    {
        rtmp_handler_->OnNewConnection(conn);
    }

    // 2. 处理rtmp服务端连接
    RtmpContextPtr shake = std::make_shared<RtmpContext>(conn, rtmp_handler_);
    conn->setContext(kRtmpContext, shake);
    shake->StartHandShake(); // 等待C0C1
}
void RtmpServer::OnConnectionDestroy(const TcpConnectionPtr &conn)
{
    // 1. 通知上层
    if (rtmp_handler_)
    {
        rtmp_handler_->OnConnectionDestroy(conn);
    }
    conn->clearContext(kRtmpContext);
}
void RtmpServer::OnMessage(const TcpConnectionPtr &conn, MsgBuffer &buf)
{
    RtmpContextPtr shake = conn->getContext<RtmpContext>(kRtmpContext);
    // RtmpHandShakePtr shake = conn->GetContext<RtmpHandShake>(kRtmpContext);
    if (shake)
    {

        // int ret = shakehand->HandShake(buf); // 开始握手
        int ret = shake->Parse(buf); // 开始解析，握手，命令或消息
        if (ret == 0)                // 0握手成功
        {
            RTMP_DEBUG << "host: " << conn->getPeerAddress().toIpPort() << " handshake success.";
        }
        else if (ret == -1) // -1失败
        {
            conn->forceClose();
        }
    }
}
void RtmpServer::OnWriteComplete(const ConnectionPtr &conn)
{
    RtmpContextPtr shake = conn->getContext<RtmpContext>(kRtmpContext);
    if (shake)
    {
        shake->OnWriteComplete(); // 运转状态机
    }
}
void RtmpServer::OnActive(const ConnectionPtr &conn)
{
    if (rtmp_handler_)
    {
        rtmp_handler_->OnActive(conn);
    }
}