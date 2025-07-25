/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:09:52
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 15:13:32
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFNull.h
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
        /// @brief 继承AMFAny，实现Null数据类型的解析
        class AMFNull : public AMFAny
        {
        public:
            AMFNull(const std::string &name);
            AMFNull();
            ~AMFNull();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsNull() override;
            void Dump() const override;

        private:
            bool b_{false};
        };
    } // namespace mm
    
} // namespace liveStream
