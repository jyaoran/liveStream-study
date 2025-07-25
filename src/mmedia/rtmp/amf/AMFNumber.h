/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:05:01
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 14:11:13
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFNumber.h
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
        /// @brief 继承AMFAny，实现Number数据类型的解析
        class AMFNumber : public AMFAny
        {
        public:
            AMFNumber(const std::string &name);
            AMFNumber();
            ~AMFNumber();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsNumber() override;
            double Number() override;
            void Dump() const override;

        private:
            double number_{0};
        };
    } // namespace mm
    
} // namespace liveStream