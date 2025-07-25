/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:10:02
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 14:24:45
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFDate.h
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
        /// @brief 继承AMFAny，实现Date数据类型的解析
        class AMFDate : public AMFAny
        {
        public:
            AMFDate(const std::string &name);
            AMFDate();
            ~AMFDate();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsDate() override;
            double Date() override;
            void Dump() const override;

            int16_t UtcOffset() const;

        private:
            double utc_{0.0f};
            int16_t utc_offset_{0};
        };
    } // namespace mm
    
} // namespace liveStream