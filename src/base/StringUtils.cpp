/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-18 16:56:25
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-20 16:01:38
 * @FilePath: /liveStream-study/src/base/stringUtils.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

 #include "easylogging++.h"
 #include "StringUtils.h"


bool liveStream::base::StringUtils::startWith(const std::string &str, const std::string &Start)
{
    if (Start.empty())
    {
        return true;
    }
    
    if (str.empty())
    {
        return false;
    }
    
    auto startLen = Start.length();
    auto strLen = str.length();

    if (strLen < startLen)
    {
        return false;
    }
    
    return str.compare(0, startLen, Start) == 0;
}

bool liveStream::base::StringUtils::endWith(const std::string &str, const std::string &end)
{
    if (end.empty())
    {
        return true;
    }
    
    if (str.empty())
    {
        return false;
    }
    
    auto endLen = end.length();
    auto strLen = str.length();
    if (strLen < endLen)
    {
        return false;
    }
    
    return str.compare(strLen - endLen, endLen, end) == 0;
}

// 返回文件的完整路径
std::string liveStream::base::StringUtils::getFilePath(const std::string &filePath)
{
    auto pos = filePath.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        return filePath.substr(0, pos);
    }
    else
    {
        return "./";
    }

    return "";
}

// 返回文件名，包含后缀
std::string liveStream::base::StringUtils::getFileNameExt(const std::string &filePath)
{
    auto pos = filePath.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        if (pos + 1 == filePath.size())
        {
            return filePath.substr(pos + 1);
        }
        
    }
    return filePath;
}

// 返回文件名，不带后缀
std::string liveStream::base::StringUtils::getFileName(const std::string &filePath)
{
    std::string fileName = getFileNameExt(filePath);
    auto pos = fileName.find_last_of(".");
    if (pos != std::string::npos)
    {
        return fileName.substr(0, pos);
    }

    return fileName;
}

// 返回文件后缀
std::string liveStream::base::StringUtils::getFileExtension(const std::string &filePath)
{
    std::string fileName = getFileNameExt(filePath);
    auto pos = fileName.find_last_of(".");
    if (pos != std::string::npos)
    {
        if (pos != 0 && pos + 1 < fileName.size())
        {
            return fileName.substr(pos + 1);
        }
        
    }
    return fileName;
}

std::vector<std::string> liveStream::base::StringUtils::split(const std::string &str, const std::string &delimiter)
{
    // 如果分隔符为空，直接返回一个空的字符串向量
    if (delimiter.empty())
    {
        return std::vector<std::string>();
    }
    
    // 定义一个字符串向量来存储分割后的子串
    std::vector<std::string> result;
    // 定义 last 为上一个分隔符的位置，初始值为 0
    size_t last = 0;
    // 定义 next 为下一个分隔符的位置，初始值为 0
    size_t next = 0;

    // 循环查找分隔符的位置
    while ((next = str.find(delimiter, last)) != std::string::npos)
    {
        // 如果当前分隔符的位置大于上一个分隔符的位置，说明找到了一个非空的子串
        if (next > last) 
        {
            // 将子串添加到结果向量中
            result.emplace_back(str.substr(last, next - last));
        }
        else
        {
            // 如果当前分隔符的位置等于上一个分隔符的位置，说明找到了一个空的子串
            result.emplace_back("");
        }
        
        // 更新 last 为下一个分隔符的位置加上分隔符的长度，以便继续查找
        last = next + delimiter.length();
    }
    
    // 如果最后一个分隔符之后还有剩余的子串，将其添加到结果向量中
    if (last < str.size())
    {
        result.emplace_back(str.substr(last));
    }

    // 返回分割后的字符串向量
    return result;
}


std::vector<std::string> liveStream::base::StringUtils::splitWithFSM(const std::string &str, const char delimiter)
{
    enum 
    {
        kStateInit = 0,
        kStateNormal = 1,
        kStateDelimiter = 2,
        kStateEnd = 3,
    };

    int state = kStateInit;
    std::vector<std::string> result;
    std::string tmp;

    state = kStateNormal;
    for (int pos = 0; pos < str.size();)
    {
        if (state == kStateNormal)
        {
            if (str.at(pos) == delimiter)
            {
                state = kStateDelimiter;
                continue;
            }

            tmp.push_back(str.at(pos));
            pos++;
        }
        else if (state == kStateDelimiter)
        {
            result.push_back(tmp);
            tmp.clear();
            state = kStateNormal;
            pos++;
        }
    }

    if (!tmp.empty())
    {
        result.push_back(tmp);
    }
    
    state = kStateEnd;

    return result;
}
