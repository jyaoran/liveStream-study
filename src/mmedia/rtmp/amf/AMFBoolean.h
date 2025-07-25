/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:05:24
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 14:14:40
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFBoolean.h
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
        /// @brief 继承AMFAny，实现Boolean数据类型的解析
        class AMFBoolean : public AMFAny
        {
        public:
            AMFBoolean(const std::string &name);
            AMFBoolean();
            ~AMFBoolean();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsBoolean() override;
            bool Boolean() override;
            void Dump() const override;

        private:
            bool b_{false};
        };
    } // namespace mm
    
} // namespace liveStream
