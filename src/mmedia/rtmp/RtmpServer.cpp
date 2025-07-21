/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:26:50
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 16:40:01
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
:TcpServer(loop, local), rtmp_handler_(handler)
{
    if (rtmp_handler_ == nullptr)
    {
        RTMP_ERROR<< "RtmpHandler is nullptr, RtmpServer will not handle any rtmp request";
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


}
void RtmpServer::OnConnectionDestroy(const TcpConnectionPtr &conn)
{

}
void RtmpServer::OnMessage(const TcpConnectionPtr &conn, MsgBuffer &buf)
{

}
void RtmpServer::OnWriteComplete(const ConnectionPtr &conn)
{

}
void RtmpServer::OnActive(const ConnectionPtr &conn)
{

}