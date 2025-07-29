/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-25 17:24:06
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 22:53:53
 * @FilePath: /liveStream-study/src/base/Config.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include "NonCopyable.h"
#include "Singleton.h"
#include "json/json.h"

#include <string>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace liveStream
{
    namespace base
    {

        // 直播服务的信息
        struct ServiceInfo
        {
            std::string addr;
            uint16_t port;
            std::string protocol;  // 业务协议如:rtmp
            std::string transport; // 传输协议如:tcp
        };
        using ServiceInfoPtr = std::shared_ptr<ServiceInfo>;

        class DomainInfo;
        class AppInfo;
        using DomainInfoPtr = std::shared_ptr<DomainInfo>;
        using AppInfoPtr = std::shared_ptr<AppInfo>;

        class Config;
        using ConfigPtr = std::shared_ptr<Config>;
        // 基本配置类
        class Config
        {
        public:
            Config() = default;
            ~Config() = default;

            using ptr = std::shared_ptr<Config>;

        public:
            bool LoadConfig(const std::string &file);

            const std::vector<ServiceInfoPtr> &GetServiceInfos();
            const ServiceInfoPtr &GetServiceInfo(const std::string &protocol, const std::string &transport);
            bool ParseServiceInfo(const Json::Value &serviceObj);
            // 直播业务层配置相关
            AppInfoPtr GetAppInfo(const std::string &domain, const std::string &app);
            DomainInfoPtr GetDomainInfo(const std::string &domain);

        public:
            std::string name_;
            int32_t cpu_start_{0}; // 开始绑定的cpu，用第一个
            int32_t thread_num_{-1};
            int32_t cpus_{-1};

        private:
            // 直播业务层配置相关
            bool ParseDirectory(const Json::Value &root);
            bool ParseDomainPath(const std::string &path);
            bool ParseDomainFile(const std::string &file);
            void SetDomainInfo(const std::string &domain, DomainInfoPtr &p);
            void SetAppInfo(const std::string &domain, const std::string &app);

            std::vector<ServiceInfoPtr> services_; // 每个用户都会有一个info
            std::unordered_map<std::string, DomainInfoPtr> domaininfos_;
            std::mutex lock_;
        };

        // 实现config的动态热更新，原理就是动态替换config的智能指针
        class ConfigMgr : public NonCopyable // 管理类都是单例，不可拷贝
        {
        public:
            ConfigMgr() = default;
            ~ConfigMgr() = default;

        public:
            bool LoadConfig(const std::string &file); // 因为外部访问的都是mgr类
            Config::ptr GetConfig();

        private:
            Config::ptr config_;
            std::mutex lock_;
        };

#define sConfigMgr liveStream::base::Singleton<liveStream::base::ConfigMgr>::Instance()
    }
}
