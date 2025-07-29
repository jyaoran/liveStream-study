/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-28 22:17:44
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:56:49
 * @FilePath: /liveStream-study/src/live/user/User.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "network/net/Connection.h"
#include "base/AppInfo.h"
#include <memory>
#include <cstdint>
#include <string>


/**
 *  User 这个类在代码中被设计为一个基类，用于表示不同类型的用户在直播流系统中的行为和属性。这个类的主要意义包括：
 *  统一接口：通过定义公共的接口方法，
 *           如 GetUserType、GetUserProtocol、Close、getConnection 等，
 *           使得 User 类为所有具体类型的用户提供了统一的访问方式，便于系统管理。
 * 属性管理：User 类包含了用户的一些核心属性，
 *           如 domain_name_、app_name_、stream_name_、param_、user_id_、app_info_、type_、protocol_ 等。
 *           这些属性对于每个用户来说都是必需的，无论用户是推流者还是播放者。
 * 生命周期管理：类中提供了 Avtive、Deactive 和 Close 方法，
 *              用于管理用户的活动状态和生命周期。destroyed_ 是一个原子布尔变量，
 *              用于标识用户是否已经被销毁，这在多线程环境下非常重要。
 * 多态性：通过虚函数（如 GetUserType 和 GetUserProtocol），
 *         User 类允许其派生类以多态的方式实现这些方法。
 *         这意味着系统可以根据用户的具体类型来执行不同的操作。
 * 关联关系：User 类与其他类（如 Connection、Stream、Session 和 AppInfo）通过指针关联，
 *           使得这些指针可以被多态地使用，从而实现更灵活的系统架构。
 * 资源管理：使用 std::shared_ptr 来管理 Connection、Stream 和 Session 对象的生命周期，
 *           并确保资源的正确释放，避免内存泄漏。
 * 这个抽象类的设计使得系统可以轻松地扩展以支持更多的用户类型（如不同的推流协议或播放方式），
 * 同时保持系统的稳定性和可维护性。
 * 
 */

namespace liveStream
{
    namespace live
    {
        using namespace liveStream::network;
        using namespace liveStream::base;
        using std::string;

        enum class UserType
        {
            kUserTypePublishRtmp = 0,  // RTMP协议的推流用户类型
            kUserTypePublishMpegts,    // MPEG-TS协议的推流用户类型
            kUserTypePublishPav,       // PA协议的推流用户类型
            kUserTypePublishWebRtc,    // WebRTC协议的推流用户类型
            kUserTypePlayerPav,        // PA协议的播放用户类型
            kUserTypePlayerFlv,        // FLV格式的播放用户类型
            kUserTypePlayerHls,        // HLS格式的播放用户类型
            kUserTypePlayerRtmp,       // RTMP协议的播放用户类型
            kUserTypePlayerWebRTC,     // WebRTC协议的播放用户类型
            kUserTypeUnknowed = 255,   // 未知类型的用户
        };


        /**
         * @brief c++11 class，枚举有类的作用域和属性
         * enum class 是 C++11 引入的一种枚举类型，称为强类型枚举（strongly-typed enum）。
         * 它与传统的 enum 不同，enum class 具有以下特性：
         *      强类型：enum class 的枚举值不会隐式转换为整数类型，必须显式转换。
         *      作用域：enum class 的枚举值在枚举类型的作用域内，避免了命名冲突。
         *      隐式转换限制：enum class 不允许隐式转换为整数类型，必须使用 static_cast 进行显式转换。
         *      更好的类型安全：由于 enum class 不允许隐式转换，可以提高代码的类型安全性和可读性。
         * 
         */
        enum class UserProtocol
        {
            kUserProtocolHttp = 0,    // HTTP协议
            kUserProtocolHttps,       // HTTPS协议
            kUserProtocolQuic,        // QUIC协议
            kUserProtocolRtsp,        // RTSP协议
            kUserProtocolWebRTC,      // WebRTC协议
            kUserProtocolUdp,         // UDP协议
            kUserProtocolUnknowed = 255 // 未知协议
        };

        // 前向声明Stream类，避免循环包含头文件
        class Stream;
        using StreamPtr = std::shared_ptr<Stream>;

        // 前向声明Session类，避免循环包含头文件
        class Session; 
        using SessionPtr = std::shared_ptr<Session>;


        class Stream;
        using StreamPtr = std::shared_ptr<Stream>;
        class Session; // session要包含user头文件，这里如果加入了session头文件就循环包含了,用前向声明解决
        using SessionPtr = std::shared_ptr<Session>;
        
        class User : public std::enable_shared_from_this<User>
        {
        public:
            friend class Session;
            explicit User(const ConnectionPtr &ptr, const StreamPtr &stream, const SessionPtr &s);
            virtual ~User();
        
        public:
            const string &DomainName() const ;
            void SetDomainName(const string &domain);
            const string &AppName() const ;
            void SetAppName(const string &domain);
            const string &StreamName() const ;
            void SetStreamName(const string &domain);
            const string &Param() const ;
            void SetParam(const string &domain);   
            const AppInfoPtr &GetAppInfo()const;
            void SetAppInfo(const AppInfoPtr &info);
            virtual UserType GetUserType() const;
            void SetUserType(UserType t);
            virtual UserProtocol GetUserProtocol() const ;
            void SetUserProtocol(UserProtocol p) ;
        
        public:
            void Close();
            ConnectionPtr getConnection();
            uint64_t ElaspsedTime(); // 自用户创建开始经历了多少时间
            virtual void SetConnection(const ConnectionPtr &conn);
            void Avtive();
            void Deactive();
            const std::string &UserId() const
            {
                return user_id_;
            }
            SessionPtr GetSession() const
            {
                return session_;
            }
            StreamPtr GetStream() const
            {
                return stream_;
            }
        
        protected:
            ConnectionPtr connection_; // 用户的active通过conn放在loop中之执行
            StreamPtr stream_;  // 用户的推流，拉流的流
            string domain_name_;
            string app_name_;
            string stream_name_;
            string param_;
            string user_id_;    // 标识不同的user，用ip:port标识
            AppInfoPtr app_info_;
            int64_t start_timestamp_{0};
            UserType type_{UserType::kUserTypeUnknowed};
            UserProtocol protocol_{UserProtocol::kUserProtocolUnknowed};
            std::atomic_bool destroyed_{false}; // 用户有没有被销毁
            SessionPtr session_;
        };
    
        
    } // namespace live
    
} // namespace liveStream