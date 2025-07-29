/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-25 18:00:46
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 18:17:56
 * @FilePath: /liveStream-study/src/base/DomainInfo.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

namespace liveStream
{
    namespace base
    {
        class AppInfo;
        using AppInfoPtr = std::shared_ptr<AppInfo>;

        class DomainInfo
        {
        public:
            DomainInfo(/* args */) = default;
            ~DomainInfo() = default;

        public:
            const std::string &DomainName() const; // 返回域名
            const std::string &Type() const;
            bool ParseDomainInfo(const std::string &file);
            AppInfoPtr GetAppInfo(const std::string &app_name);

        private:
            std::string name_; // domain name
            std::string type_;
            std::mutex lock_;
            std::unordered_map<std::string, AppInfoPtr> appinfos_;
        };

    } // namespace base

} // namespace liveStream
