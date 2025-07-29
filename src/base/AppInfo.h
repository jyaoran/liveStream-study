/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-25 18:01:14
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-25 18:03:57
 * @FilePath: /liveStream-study/src/base/AppInfo.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include <string>
#include <json/json.h>
#include <memory>

namespace liveStream
{
    namespace base
    {
        class DomainInfo;
        class AppInfo;
        class Target;

        using TargetPtr = std::shared_ptr<Target>;
        using AppInfoPtr = std::shared_ptr<AppInfo>;
        class AppInfo
        {
        public:
            explicit AppInfo(DomainInfo &d);
            ~AppInfo() = default;

            bool ParseAppInfo(Json::Value &root);
        
        public:
            DomainInfo &domain_info; // 引用数据成员一定要在构造函数的序列化参数初始化
            std::string domain_name;
            std::string app_name;
            uint32_t max_buffer{1000}; //默认1000帧
            bool rtmp_support{false}; // 默认不支持rtmp
            bool flv_support{false};
            bool hls_support{false};
            uint32_t content_latency{3 * 1000}; // 直播延时，单位ms
            uint32_t stream_idle_time{30 * 1000}; // 流间隔最大时长
            uint32_t stream_timeout_time{30 * 1000}; // 流超时时间

            std::vector<TargetPtr> pulls;
        };
        
        
    } // namespace base
    
} // namespace liveStream
