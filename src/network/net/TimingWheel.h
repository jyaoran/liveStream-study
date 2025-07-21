/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 19:07:29
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-26 17:07:40
 * @FilePath: /liveStream-study/src/network/net/TimingWheel.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include <vector>
#include <deque>
#include <unordered_set>
#include <functional>
#include <memory>
#include <cstdint>

namespace liveStream 
{
    namespace network 
    {
        using EntryPtr = std::shared_ptr<void>;
        using WheelEntry = std::unordered_set<EntryPtr>;
        using Wheel = std::deque<WheelEntry>;
        using Wheels = std::vector<Wheel>;
        using Func = std::function<void()>;

        const int kTimingMinute = 60;
        const int kTimingHour = 60 * 60;
        const int kTimingDay = 24 * 60 * 60;

        enum TimingType
        {
            kTimingSecondType = 0,
            kTimingMinuteType = 1,
            kTimingHourType = 2,
            kTimingDayType = 3,        
        };

        class CallbackEntry 
        {
        public:
            using ptr = std::shared_ptr<CallbackEntry>;

            CallbackEntry(Func callback) : m_callback(callback) {}

            ~CallbackEntry() 
            {
                if (m_callback)
                {
                    m_callback();
                } 
            }

        private:
            Func m_callback;    
        };

        class TimingWheel 
        {
        public:
            using ptr = std::shared_ptr<TimingWheel>;
            TimingWheel();
            ~TimingWheel();
        
            void InsertEntry(uint32_t delay, EntryPtr entryPtr);
            void OnTimer(uint64_t now);
            void PopUp(Wheel &bq);
            void RunAfter(double delay, const Func &cb);
            void RunAfter(double delay, const Func &&cb);
            void RunEvery(double interval, const Func &cb);
            void RunEvery(double interval, const Func &&cb);

        private:
            void InsertSecondEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertHourEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertDayEntry(uint32_t delay, EntryPtr entryPtr);

        private:
            Wheels m_wheels;
            int64_t m_last_ts{0};
            uint64_t m_tick{0};
        };
    }
}