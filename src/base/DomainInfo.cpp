#include "DomainInfo.h"
#include "base/AppInfo.h"
#include "json/json.h"
#include "easylogging++.h"

#include <fstream>

using namespace liveStream::base;

const std::string &DomainInfo::DomainName() const
{
    return name_;
    
}

const std::string &DomainInfo::Type() const
{
    return type_;
}

bool DomainInfo::ParseDomainInfo(const std::string &file)
{
    LOG(DEBUG) << "domain file : " << file;

    Json::Value root;
    Json::CharReaderBuilder reader;
    std::ifstream in(file);
    std::string err;
    // 用流构建解析
    auto ok = Json::parseFromStream(reader, in, &root, &err);
    if(!ok)       // 解析json文件root到root
    {
        LOG(DEBUG) << "domain config file:" << file << " parse error.err: " << err;
        return false;
    }

    Json::Value domainObj = root["domain"];
    if(domainObj.isNull())
    {
        LOG(ERROR) << "domain config invalid content.no domain.";
        return false;
    }
    // name
    Json::Value nameObj = domainObj["name"];
    if(!nameObj.isNull())
    {
        name_ = nameObj.asString();
    }
    // type
    Json::Value typeObj = domainObj["type"];
    if(!typeObj.isNull())
    {
        type_ = typeObj.asString();
    }
    // app
    Json::Value appsObj = domainObj["app"];
    if(appsObj.isNull())
    {
        LOG(ERROR) << "domain config invalid content.no apps.";
        return false;
    }
    // app是数组
    for(auto &appObj : appsObj)
    {
        AppInfoPtr appinfo = std::make_shared<AppInfo>(*this);
        auto ret = appinfo->ParseAppInfo(appObj);
        if(ret)
        {
            std::lock_guard<std::mutex> lk(lock_);
            appinfos_.emplace(appinfo->app_name, appinfo);
        }
    }

    return true;
}

AppInfoPtr DomainInfo::GetAppInfo(const std::string &app_name)
{
    std::lock_guard<std::mutex> lk(lock_);

    auto iter = appinfos_.find(app_name);
    if(iter != appinfos_.end())
    {
        return iter->second;
    }
    return AppInfoPtr();
}