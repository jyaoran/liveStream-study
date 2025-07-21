/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-20 17:01:11
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-21 13:13:43
 * @FilePath: /liveStream-study/src/base/task.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

 #pragma once
 #include <cstdint>
 #include <functional>
 #include <memory>

 namespace liveStream 
 {
    namespace base 
    {
        class Task : public std::enable_shared_from_this<Task> 
        {
        public:
            using ptr = std::shared_ptr<Task>;
            using TaskCallback = std::function<void(const std::shared_ptr<Task>&)>;
        
            Task(const TaskCallback &callback, uint64_t interval);
            Task(const TaskCallback &&callback, uint64_t interval);

            void run();
            void restart();

            int64_t when() const { return m_when; }

        private:
            int64_t m_interval{0};
            int64_t m_when{0};
            TaskCallback m_callback;
        };
    }
 }