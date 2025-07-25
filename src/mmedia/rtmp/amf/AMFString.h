/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:09:40
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 15:17:24
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFString.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "AMFAny.h"

namespace liveStream
{
    namespace mm
    {
        /// @brief 继承AMFAny，实现String数据类型的解析
        class AMFString : public AMFAny
        {
        public:
            AMFString(const std::string &name);
            AMFString();
            ~AMFString();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsString() override;
            const std::string &String() override;
            void Dump() const override;

        private:
            std::string string_;
        };
    } // namespace mm
    
} // namespace liveStream