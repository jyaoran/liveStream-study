/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-29 13:43:35
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-29 15:26:21
 * @FilePath: /liveStream-study/src/live/Session.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "Session.h"
#include "Stream.h"
#include "base/TTime.h"
#include "base/AppInfo.h"
#include "live/base/LiveLog.h"
#include "base/StringUtils.h"
#include "live/user/RtmpPlayUser.h"


using namespace liveStream::live;

namespace 
{
    static UserPtr user_null;
} // namespace 


Session::Session(const std::string &session_name)
:session_name_(session_name)
{
    // 流创建
    stream_ = std::make_shared<Stream>(session_name, *this);
    player_live_time_ = TTime::getCurrentTimeInMS();
}

Session::~Session()
{
    LIVE_DEBUG << "session:" << session_name_ << " destroy.now:" << base::TTime::getCurrentTimeInMS();
}

/// @brief 会话准备好的时间，和流的时间一致
/// @return 
int32_t Session::ReadyTime() const
{
    return stream_->ReadyTime();
}

int64_t Session::SinceStart() const
{
    return stream_->SinceStart();
}

bool Session::IsTimeout()
{
    // 流超时
    if(stream_->Timeout())
    {
        LIVE_DEBUG << "stream timeout.";
        return true;
    }

    // 会话开始到现在的时间
    auto idle = TTime::getCurrentTimeInMS() - player_live_time_;
    // 没有用户的时间达到配置的时间，超时
    if(players_.empty() && idle > app_info_->stream_idle_time)
    {
        LIVE_DEBUG << "no player timeout.";
       return true; 
    }

    return false;
}

UserPtr Session::CreatePublishUser(const ConnectionPtr &conn, 
                        const std::string &session_name, 
                        const std::string &param, 
                        UserType type)
{
    // 流和创建的用户session不一致，就是不能创建的
    if(session_name != session_name_)
    {
        LIVE_ERROR << "create publish user failed. Invalid session name:" << session_name;
        return user_null;
    }

    // session_name = domain/app/stream_name
    auto list = base::StringUtils::split(session_name, "/");
    if(list.size() != 3)
    {
        LIVE_ERROR << "create publish user failed. Invalid session name:" << session_name;
        return user_null;
    }

    UserPtr user = std::make_shared<User>(conn, stream_, shared_from_this());
    user->SetAppInfo(app_info_);
    user->SetDomainName(list[0]);
    user->SetAppName(list[1]);
    user->SetStreamName(list[2]);
    user->SetParam(param);
    user->SetUserType(type);
    conn->setContext(kUserDefineContext, user);
    return user;
}

UserPtr Session::CreatePlayerUser(const ConnectionPtr &conn,
                         const std::string &session_name, 
                         const std::string &param, 
                         UserType type)
{
    // 流和创建的用户session不一致，就是不能创建的
    if(session_name != session_name_)
    {
        LIVE_ERROR << "create publish user failed. Invalid session name:" << session_name;
        return user_null;
    }

    // session_name = domain/app/stream_name
    auto list = base::StringUtils::split(session_name, "/");
    if(list.size() != 3)
    {
        LIVE_ERROR << "create publish user failed. Invalid session name:" << session_name;
        return user_null;
    }
    
    // TODO: 这里可以用抽象工厂创建
    PlayerUserPtr user;
    if(type == UserType::kUserTypePlayerRtmp)
    {
        user = std::make_shared<RtmpPlayerUser>(conn, stream_, shared_from_this());
    }
    else
    {
        return user_null;
    }
    user->SetAppInfo(app_info_);;
    user->SetDomainName(list[0]);
    user->SetAppName(list[1]);
    user->SetStreamName(list[2]);
    user->SetParam(param);
    user->SetUserType(type);
    conn->setContext(kUserDefineContext, user);
    return user;
}

/// @brief 在session中删除一个用户，清除指针
/// @param user 
void Session::CloseUser(const UserPtr &user)
{
    // atomic变量，交换新的值， 返回原来的值
    if(!user->destroyed_.exchange(true))
    {
        {
            std::lock_guard<std::mutex> lk(lock_);
            // fixbug:这里应该是判断Publis类型，如果是player，在播放用户关闭的时候，进入判断，推流用户也关闭了
            if(user->GetUserType() <= UserType::kUserTypePublishWebRtc)
            {
                // 销毁publisher用户
                if(publisher_)
                {
                    LIVE_DEBUG << "remove publisher, session name: "<< session_name_
                            << ", user: " << user->UserId()
                            << ", elapsed:" << user->ElaspsedTime()
                            << ", ReadyTime:" << ReadyTime()
                            << ", stream time:" << SinceStart();
                    publisher_.reset();
                }
            }
            else
            {
                // 销毁player用户
                players_.erase(std::dynamic_pointer_cast<PlayerUser>(user));
                player_live_time_ = liveStream::base::TTime::getCurrentTimeInMS();
                LIVE_DEBUG << "remove player, session name: "<< session_name_
                    << ", user: " << user->UserId()
                    << ", elapsed:" << user->ElaspsedTime()
                    << ", ReadyTime:" << ReadyTime()
                    << ", stream time:" << SinceStart();
            }
        }
    }
}

/**
 * @description: 在stream中被调用，stream有数据就会激活
 * @return {*}
 */
void Session::ActiveAllPlayers()
{
    // 用户的容器是所有线程共用的
    std::lock_guard<std::mutex> lk(lock_);
    for(auto const &u : players_)
    {
        u->Avtive();
    }
}

void Session::AddPlayer(const PlayerUserPtr &user)
{
    {
        std::lock_guard<std::mutex> lk(lock_);  
        players_.insert(user);
    }

    LIVE_DEBUG << " add player, session name: "<< session_name_ << ", user:" << user->UserId();

    // 如果此时publisher还没有，就是一个回源的动作
    // 在这rtmp服务器没有流媒体内容，需要从别的服务器找到Publisher
    if(!publisher_)
    {

    }
    user->Avtive();
}

void Session::SetPublisher(UserPtr &user)
{
    std::lock_guard<std::mutex> lk(lock_); 
    if(publisher_ == user)
    {
        return;
    }

    // 不为空且没有销毁
    if(publisher_ && !publisher_->destroyed_.exchange(true))
    {
        // 关闭原来的，设置新的推流
        publisher_->destroyed_.exchange(true);
        publisher_->Close();
    }

    // 如果为空，直接赋值
    publisher_ = user;
}

StreamPtr Session::GetStream()
{
    return stream_;
}

const string &Session::SessionName() const
{
    return session_name_;
}

void Session::SetAppInfo(AppInfoPtr &ptr)
{
    app_info_ = ptr;
}

AppInfoPtr &Session::GetAppInfo()
{
    return app_info_;
}

bool Session::IsPublishing() const
{
    // !publisher_不是空指针就是false
    // !!publisher就是true
    return !!publisher_;
}

void Session::Clear()
{
    std::lock_guard<std::mutex> lk(lock_); 
    if(publisher_)
    {
        CloseUserNoLock(publisher_);
    }
    for(auto const & p : players_)
    {
        CloseUserNoLock(std::dynamic_pointer_cast<User>(p));
    }
    players_.clear();
}

/// @brief 这个是不带锁的，因为上面带锁的被Clear()调用就重入锁造成死锁了
/// @param user 
void Session::CloseUserNoLock(const UserPtr &user)
{
    // atomic变量，交换新的值， 返回原来的值
    if(!user->destroyed_.exchange(true))
    {
        if(user->GetUserType() <= UserType::kUserTypePlayerWebRTC)
        {
            // 销毁publisher用户
            if(publisher_)
            {
                LIVE_DEBUG << "remove publisher, session name: "<< session_name_
                        << ", user: " << user->UserId()
                        << ", elapsed:" << user->ElaspsedTime()
                        << ", ReadyTime:" << ReadyTime()
                        << ", stream time:" << SinceStart();

                user->Close();
                publisher_.reset();
            }
        }
        else
        {
            LIVE_DEBUG << "remove player, session name: "<< session_name_
                << ", user: " << user->UserId()
                << ", elapsed:" << user->ElaspsedTime()
                << ", ReadyTime:" << ReadyTime()
                << ", stream time:" << SinceStart();
            // 销毁player用户, publiser也可能是player
            // 不能在vector循环中操作删除，不然迭代器失效
            // players_.erase(std::dynamic_pointer_cast<PlayerUser>(user));
            user->Close();
            player_live_time_ = liveStream::base::TTime::getCurrentTimeInMS();
        }
    }
}
