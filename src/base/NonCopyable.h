/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 16:41:55
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-20 17:00:27
 * @FilePath: /liveStream-study/src/base/nonCopyable.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once


namespace liveStream 
{
    namespace base 
    {
        class NonCopyable 
        {
        public:
            NonCopyable() = default;
            ~NonCopyable() = default;
            
        protected:    
            // c++  3/5法则，有默认的拷贝构造，拷贝赋值运算符，析构，移动构造，移动赋值
            // 定义了其中一个，默认生成其他四个。删除其中一个，默认不会生成其他四个
            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };
    }
}