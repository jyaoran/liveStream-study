/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-18 16:55:49
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-20 15:50:51
 * @FilePath: /liveStream-study/src/base/stringUtils.h
 * @Description: 字符串处理工具类
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once
#include <string>
#include <vector>

namespace liveStream 
{
    namespace base 
    {
        class StringUtils 
        {
        public:

            // 字符串匹配工具
            // 字符前缀匹配
            static bool startWith(const std::string &str, const std::string &Start);

            // 字符后缀匹配
            static bool endWith(const std::string &str, const std::string &end);


            // 文件名、文件路径处理
            // 从完整的文件路径，找到文件所在的目录
            static std::string getFilePath(const std::string &filePath);

            // 完整的文件路径中， 取出文件名+文件后缀
            static std::string getFileNameExt(const std::string &filePath);

            // 完整的文件路径中， 取出文件名
            static std::string getFileName(const std::string &filePath);

            // 完整的文件路径中， 取出文件后缀
            static std::string getFileExtension(const std::string &filePath);

        public:
            // 字符串分割
            // 把一个字符串按照分隔符分割成多个子串，存放入一个vector中
            static std::vector<std::string> split(const std::string &str, const std::string &delimiter);

            // 有限状态自动机
            static std::vector<std::string> splitWithFSM(const std::string &str, const char delimiter);
        };
    } // namespace base
} // namespace liveStream