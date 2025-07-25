/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-23 14:10:39
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 15:27:46
 * @FilePath: /liveStream-study/src/mmedia/rtmp/amf/AMFObject.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "AMFAny.h"
#include <vector>

namespace liveStream
{
    namespace mm
    {
        /// @brief 继承AMFAny，实现Object数据类型的解析
        class AMFObject : public AMFAny
        {
        public:
            AMFObject(const std::string &name);
            AMFObject();
            ~AMFObject();
        
        public:
            int Decode(const char *data, int size, bool has = false) override;
            bool IsObject() override;
            AMFObjectPtr Object() override;
            void Dump() const override;

            int DecodeOnce(const char *data, int size, bool has = false);
            const AMFAnyPtr &Property(const std::string &name) const;
            const AMFAnyPtr &Property(int index) const;
        private:
            std::vector<AMFAnyPtr> properties_;
        };
    } // namespace mm
    
} // namespace liveStream
