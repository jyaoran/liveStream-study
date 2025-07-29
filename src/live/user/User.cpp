/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-28 22:23:40
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:16:29
 * @FilePath: /liveStream-study/src/live/user/User.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "User.h"
#include "base/TTime.h"
#include "live/Stream.h"

using namespace liveStream::live;

User::User(const ConnectionPtr &ptr, const StreamPtr &stream, const SessionPtr &s)
    : connection_(ptr), stream_(stream), session_(s)
{
    start_timestamp_ = TTime::getCurrentTimeInMS();
    user_id_ = ptr->getPeerAddress().toIpPort();
}

User::~User()
{
}

const std::string &User::DomainName() const
{
    return domain_name_;
}

void User::SetDomainName(const string &domain)
{
    domain_name_ = domain;
}

const std::string &User::AppName() const
{
    return app_name_;
}

void User::SetAppName(const std::string &app_name)
{
    app_name_ = app_name;
}

const string &User::StreamName() const
{
    return stream_name_;
}

void User::SetStreamName(const string &domain)
{
    stream_name_ = domain;
}

const string &User::Param() const
{
    return param_;
}

void User::SetParam(const string &domain)
{
    param_ = domain;
}

const AppInfoPtr &User::GetAppInfo() const
{
    return app_info_;
}

void User::SetAppInfo(const AppInfoPtr &info)
{
    app_info_ = info;
}

UserType User::GetUserType() const
{
    return type_;
}

void User::SetUserType(UserType t)
{
    type_ = t;
}

UserProtocol User::GetUserProtocol() const
{
    return protocol_;
}

void User::SetUserProtocol(UserProtocol p)
{
    protocol_ = p;
}

void User::Close()
{
    if (connection_)
    {
        // 清除连接，调用close回调函数通知
        connection_->forceClose();
    }
}

ConnectionPtr User::getConnection()
{
    return connection_;
}

uint64_t User::ElaspsedTime()
{
    return TTime::getCurrentTimeInMS() - start_timestamp_;
}

/**
 * @description: 在webrtc中使用，因为在第一次sdp部分使用的是tcp，一次性关闭之后修改为udp
 * @param {Connection} &conn
 * @return {*}
 */
void User::SetConnection(const ConnectionPtr &conn)
{
    connection_ = conn;
}

void User::Avtive()
{
    if (connection_)
    {
        connection_->Active();
    }
}

void User::Deactive()
{
    if (connection_)
    {
        connection_->deactive();
    }
}
