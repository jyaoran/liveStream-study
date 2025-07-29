/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-25 18:09:45
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:05:27
 * @FilePath: /liveStream-study/src/base/AppInfo.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#include "AppInfo.h"
// #include "Target.h"
#include "easylogging++.h"

using namespace liveStream::base;

AppInfo::AppInfo(DomainInfo &d)
:domain_info(d)
{
}

bool AppInfo::ParseAppInfo(Json::Value &root)
{
    Json::Value nameObj = root["name"];
    if(!nameObj.isNull())
    {
        app_name = nameObj.asString();
    }

    Json::Value maxBufferObj = root["max_buffer"];
    if(!maxBufferObj.isNull())
    {
        max_buffer = maxBufferObj.asUInt();
    }

    Json::Value hlsObj = root["hls_support"];
    if(!hlsObj.isNull())
    {
        hls_support = hlsObj.asString() == "on" ? true : false;
    }

    Json::Value flvObj = root["flv_support"];
    if(!flvObj.isNull())
    {
        flv_support = flvObj.asString() == "on" ? true : false;
    }

    Json::Value rtmpObj = root["rtmp_support"];
    if(!rtmpObj.isNull())
    {
        rtmp_support = rtmpObj.asString() == "on" ? true : false;
    }

    Json::Value clObj = root["content_latency"];
    if(!clObj.isNull())
    {
        content_latency = clObj.asUInt() * 1000; // ms
    }

    Json::Value sitObj = root["stream_idle_time"];
    if(!sitObj.isNull())
    {
        stream_idle_time = sitObj.asUInt();
    }

    Json::Value sttObj = root["stream_timeout_time"];
    if(!sttObj.isNull())
    {
        stream_timeout_time = sttObj.asUInt();
    }

    // Json::Value pullObj = root["pull"];  // 回源
    // if(!pullObj.isNull() &&pullObj.isArray())
    // {
    //     for(auto &a : pullObj)
    //     {
    //         // TargetPtr p = std::make_shared<Target>(domain_name, app_name);
    //         // p->ParseTarget(a);
    //         // pulls.emplace_back(p);
    //     }
    // }

    LOG(TRACE) << "app name:" << app_name
            << " max_buffer:" << max_buffer
            << " content_latency:" << content_latency
            << " stream_idle_time:" << stream_idle_time
            << " stream_timeout_time:" << stream_timeout_time
            << " rtmp_support:" << rtmp_support
            << " flv_support:" << flv_support
            << " hls_support:" << hls_support;
    
    return true;
}
