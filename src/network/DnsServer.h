/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:14:24
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 23:36:28
 * @FilePath: /liveStream-study/src/network/DnsServer.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "network/base/InetAddress.h"
#include "base/NonCopyable.h"
#include "base/Singleton.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include <vector>
#include <string>


 namespace liveStream
{
    namespace network
    {
        using namespace base;

        class DnsServer : public base::NonCopyable      // 单例
        {
        public:
            DnsServer() = default;
            ~DnsServer() = default;
        
        public:
            void AddHost(const std::string& host);
            InetAddress::ptr GetHostAddress(const std::string &host,int index);
            std::vector<InetAddress::ptr> GetHostAddress(const std::string &host);
            void UpdateHost(const std::string &host,std::vector<InetAddress::ptr> &list);
            std::unordered_map<std::string,std::vector<InetAddress::ptr>> GetHosts();
            void SetDnsServiceParam(int32_t interval,int32_t sleep,int32_t retry);
            void Start();
            void Stop();
            void OnWork();
            static void GetHostInfo(const std::string&host,std::vector<InetAddress::ptr>&list);
        
        private:
            std::thread thread_;
            bool running_{false};
            std::mutex lock_;
            // 域名，地址对
            std::unordered_map<std::string , std::vector<InetAddress::ptr>> hosts_info_;
            int32_t retry_{3};
            int32_t sleep_{200}; // ms
            int32_t interval_{180 * 1000}; // ms，6mintue
        };
        
        #define sDnsService liveStream::base::Singleton<liveStream::network::DnsServer>::Instance()
    } // namespace network
    
} // namespace liveStream