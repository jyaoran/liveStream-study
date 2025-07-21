/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 16:07:29
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 18:37:00
 * @FilePath: /liveStream-study/src/base/tests/TestStringUtils.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "base/StringUtils.h"
#include "base/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char** argv) 
{
    auto strClass = liveStream::base::StringUtils();

    std::string str = "/liveStream-study/src/base/TestStringUtils.cpp";
    char delimiter = '/';
    std::vector<std::string> vecStr = strClass.splitWithFSM(str, delimiter);

    for (auto str : vecStr)
    {
        LOG(DEBUG) << str;
    }
 
    return 0;
}