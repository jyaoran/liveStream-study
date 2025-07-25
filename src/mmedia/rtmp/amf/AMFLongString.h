/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:10:24
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 15:25:08
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFLongString.h
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
        /// @brief 继承AMFAny，实现LongString数据类型的解析
        class AMFLongString : public AMFAny
        {
        public:
            AMFLongString(const std::string &name);
            AMFLongString();
            ~AMFLongString();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsString() override;
            const std::string &String() override;
            void Dump() const override;

        private:
            std::string longString_;
        };
    } // namespace mm
    
} // namespace liveStream
