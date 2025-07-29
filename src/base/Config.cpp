/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-25 17:26:41
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-30 01:05:11
 * @FilePath: /liveStream-study/src/base/Config.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "Config.h"
#include "easylogging++.h"
#include "DomainInfo.h"
#include "AppInfo.h"
#include <fstream>
#include <sys/stat.h> // 文件属性
#include <unistd.h>
#include <dirent.h> // 目录头文件


using namespace liveStream::base;

namespace
{
    static ServiceInfoPtr service_info_nullptr;
}

bool Config::LoadConfig(const std::string &file)
{
    LOG(DEBUG) << "config file : " << file;

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::ifstream in(file);
    std::string err;
    // 用流构建解析
    auto ok = Json::parseFromStream(reader, in, &root, &err);
    if (!ok) // 解析json文件root到root
    {
        LOG(DEBUG) << "config file:" << file << " parse error!";
        return false;
    }

    Json::Value nameObj = root["name"];
    if (!nameObj.isNull())
    {
        name_ = nameObj.asString();
    }
    Json::Value cpusObj = root["cpu_start"];
    if (!cpusObj.isNull())
    {
        cpu_start_ = cpusObj.asInt();
    }
    Json::Value cpus1Obj = root["cpus"];
    if (!cpus1Obj.isNull())
    {
        cpus_ = cpus1Obj.asInt();
    }
    Json::Value threadsObj = root["threads"];
    if (!threadsObj.isNull())
    {
        thread_num_ = threadsObj.asInt();
    }

    // 解析直播服务信息
    if (!ParseServiceInfo(root["services"]))
    {
        return false;
    }
    ParseDirectory(root["directory"]); //

    return true;
}

const std::vector<ServiceInfoPtr> &liveStream::base::Config::GetServiceInfos()
{
    return services_;
}

const ServiceInfoPtr &liveStream::base::Config::GetServiceInfo(const std::string &protocol, const std::string &transport)
{
    for(auto &s : services_)
    {
        if(s->protocol == protocol && transport == s->transport)
        {
            return s;
        }
    }
    return service_info_nullptr;
}

bool liveStream::base::Config::ParseServiceInfo(const Json::Value &serviceObj)
{
   if(serviceObj.isNull())
    {
        LOG(ERROR) << " config no service section!";
        return false;
    }
    if(!serviceObj.isArray())
    {
        LOG(ERROR) << " service section type is not array!";
        return false;
    }

    for(auto const& s : serviceObj)
    {
        ServiceInfoPtr sinfo = std::make_shared<ServiceInfo>();

        sinfo->addr = s.get("addr", "0.0.0.0").asString();
        sinfo->port = s.get("port", "0").asUInt();
        sinfo->protocol = s.get("portocol", "rtmp").asString();
        sinfo->transport = s.get("transport", "tcp").asString();

        LOG(DEBUG) << "service info addr:" << sinfo->addr
                << " port:" << sinfo->port
                << " protocol:" << sinfo->protocol
                << " transport:" << sinfo->transport;
        
        services_.emplace_back(sinfo);
    }
    return true;

}

AppInfoPtr liveStream::base::Config::GetAppInfo(const std::string &domain, const std::string &app)
{
    std::lock_guard<std::mutex> lk(lock_);
    auto iter = domaininfos_.find(domain);
    if(iter != domaininfos_.end()) 
    {
        return iter->second->GetAppInfo(app);
    }
    return AppInfoPtr();
}

DomainInfoPtr liveStream::base::Config::GetDomainInfo(const std::string &domain)
{
    std::lock_guard<std::mutex> lk(lock_);
    auto iter = domaininfos_.find(domain);
    if(iter != domaininfos_.end())
    {
        return iter->second;
    }
    return DomainInfoPtr();
}

bool liveStream::base::Config::ParseDirectory(const Json::Value &root)
{
    if(root.isNull() || !root.isArray())
    {
        return false;
    }
    for(const auto& d : root)
    {
        std::string path = d.asString();
        struct stat st;
        LOG(TRACE) << "ParseDirectory path:" << path;
        auto ret = stat(path.c_str(), &st);
        LOG(TRACE) << "ret:" << ret << " errno:" << errno  << " errmsg : " << strerror(errno);
        if(ret != -1)
        {
            // S_IFMT屏蔽与文件类型无关的标志位，剩下的判断文件类型
            if((st.st_mode & S_IFMT) == S_IFDIR)
            {
                ParseDomainPath(path);
            }
            else if((st.st_mode & S_IFMT) == S_IFREG) // 普通文件
            {
                ParseDomainFile(path);
            }
        }
    }
    return true;;
}

bool liveStream::base::Config::ParseDomainPath(const std::string &path)
{
    DIR *dp = nullptr;
    struct dirent *pp = nullptr;

    LOG(DEBUG) << "parse domain path:" << path;
    dp = opendir(path.c_str()); // 打开目录
    if(!dp)// 打开失败
    {
        return false;
    }

    // 循环遍历目录下的文件
    while (true)
    {
        pp = readdir(dp);   // 从目录中读一个文件
        if(pp == nullptr)  // 为空表示读完了
        {
            break;
        }
        if(pp->d_name[0] == '.')    // 处理隐藏文件不处理
        {
            continue;
        }
        if(pp->d_type == DT_REG) // 是文件类型
        {
            if(path.at(path.size() - 1) != '/') // 要加前面的路径
            {
                ParseDomainFile(path + "/" + pp->d_name);
            }
            else
            {
                ParseDomainFile(path + pp->d_name);
            }
        }
    }
    
    closedir(dp);
    return true;
}

bool liveStream::base::Config::ParseDomainFile(const std::string &file)
{
    LOG(DEBUG) << "parse domain file:" << file;
    DomainInfoPtr d = std::make_shared<DomainInfo>();
    auto ret = d->ParseDomainInfo(file);
    if(ret)
    {
        std::lock_guard<std::mutex> lk(lock_);
        // 查找一下，找到了要删除，因为新的要覆盖
        auto iter = domaininfos_.find(d->DomainName());
        if(iter != domaininfos_.end())
        {
            domaininfos_.erase(iter);
        }
        domaininfos_.emplace(d->DomainName(), d); // 添加新的
    }
    return true;
}

void liveStream::base::Config::SetDomainInfo(const std::string &domain, DomainInfoPtr &p)
{
}

void liveStream::base::Config::SetAppInfo(const std::string &domain, const std::string &app)
{
}



/////////// ConfigMgr ///////////

bool ConfigMgr::LoadConfig(const std::string &file)
{
    LOG(DEBUG) << "load config file:" << file; 
    Config::ptr config = std::make_shared<Config>();
    // 临时变量加载配置，加载成功进行替换热更新
    if(config->LoadConfig(file))
    {
        std::lock_guard<std::mutex> lk(lock_);
        config_ = config;
        return true;
    }
        // 加载失败log打印在LoadConfig()打印了，这里不用再打印
    return false;
}

Config::ptr ConfigMgr::GetConfig()
{
     // 多线程共用一个mgr，上锁
    std::lock_guard<std::mutex> lk(lock_);
    return config_;
}
