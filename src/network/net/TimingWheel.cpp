/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 19:07:42
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-26 23:11:36
 * @FilePath: /liveStream-study/src/network/net/TimingWheel.cpp
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#include "network/base/Network.h"
#include "TimingWheel.h"

liveStream::network::TimingWheel::TimingWheel()
:m_wheels(4)
{
    m_wheels[kTimingSecondType].resize(60);
    m_wheels[kTimingMinuteType].resize(60);
    m_wheels[kTimingHourType].resize(24);
    m_wheels[kTimingDayType].resize(30);
}

liveStream::network::TimingWheel::~TimingWheel()
{
}

void liveStream::network::TimingWheel::InsertEntry(uint32_t delay, EntryPtr entryPtr)
{
    if (delay <= 0)
    {
        entryPtr.reset();
        LOG(WARNING) << "delay is too short, delay <= 0";
    }
    else if (delay < kTimingMinute)
    {
        InsertSecondEntry(delay, entryPtr);
    }
    else if (delay < kTimingHour)
    {
        InsertMinuteEntry(delay, entryPtr);
    }
    else if (delay < kTimingDay)
    {
        InsertHourEntry(delay, entryPtr);
    }
    else
    {
        auto day = delay / kTimingDay;
        if (day > 30)
        {
            LOG(WARNING) << "delay is too long, day > 30";
            return;
        }
        InsertDayEntry(delay, entryPtr);
    }

}

void liveStream::network::TimingWheel::OnTimer(uint64_t now)
{
    if (m_last_ts == 0)
    {
        m_last_ts = now;
    }

    if (now - m_last_ts <= 1000)
    {
        return;
    }
    
    m_last_ts = now;
    ++m_tick;
    PopUp(m_wheels[kTimingSecondType]);

    if (m_tick % kTimingMinute == 0)
    {
        PopUp(m_wheels[kTimingMinuteType]);
    }
    else if (m_tick % kTimingHour == 0)
    {
        PopUp(m_wheels[kTimingHourType]);
    }
    else if (m_tick % kTimingDay == 0)
    {
        PopUp(m_wheels[kTimingDayType]);
    }
}

void liveStream::network::TimingWheel::PopUp(Wheel &bq)
{
    WheelEntry tmp;

    // bug, 会出现丢失事件的情况，1s的runAfter和1s的runEvery一起，1s的runEvery的事件会丢失
    // 原因是调用bq.front().clear()后，回调直接被析构，every事件会再次插入事件，
    // 后面代码中 bq.pop_front(); 后，再次插入的事件的合集就被清空了
    // bq.front().clear(); 

    // 解决办法，每次pop_front之前，先拷贝一份
    bq.front().swap(tmp);
    bq.pop_front();
    bq.push_back(WheelEntry());
}

void liveStream::network::TimingWheel::RunAfter(double delay, const Func &cb)
{
    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([cb](){
        cb();
    });

    InsertEntry(delay, newEntry);
}

void liveStream::network::TimingWheel::RunAfter(double delay, const Func &&cb)
{ 
    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([cb](){
        cb();
    });

    InsertEntry(delay, newEntry);
}

void liveStream::network::TimingWheel::RunEvery(double interval, const Func &cb)
{
    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([this, interval, cb](){
        cb();
        RunEvery(interval, cb);
    });

    InsertEntry(interval, newEntry);
}

void liveStream::network::TimingWheel::RunEvery(double interval, const Func &&cb)
{
    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([this, interval, cb](){
        cb();
        RunEvery(interval, cb);
    });

    InsertEntry(interval, newEntry);
}

void liveStream::network::TimingWheel::InsertSecondEntry(uint32_t delay, EntryPtr entryPtr)
{
    m_wheels[kTimingSecondType][delay - 1].emplace(entryPtr);
    // LOG(INFO) << "insert second entry, wheelEntry size = " << m_wheels[kTimingSecondType][delay - 1].size();
}

void liveStream::network::TimingWheel::InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr)
{
    int minute = delay / kTimingMinute;
    int second = delay % kTimingMinute;

    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([this, entryPtr, second](){
        InsertEntry(second, entryPtr);
    });

    m_wheels[kTimingMinuteType][minute - 1].emplace(newEntry);
}

void liveStream::network::TimingWheel::InsertHourEntry(uint32_t delay, EntryPtr entryPtr)
{
    int hour = delay / kTimingHour;
    int second = delay % kTimingHour;

    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([this, entryPtr, second](){
        InsertEntry(second, entryPtr);
    });

    m_wheels[kTimingHourType][hour - 1].emplace(newEntry);
}

void liveStream::network::TimingWheel::InsertDayEntry(uint32_t delay, EntryPtr entryPtr)
{
    int day = delay / kTimingDay;
    int second = delay % kTimingDay;
    CallbackEntry::ptr newEntry = std::make_shared<CallbackEntry>([this, entryPtr, second](){
        InsertEntry(second, entryPtr);
    });

    m_wheels[kTimingDayType][day - 1].emplace(newEntry);
}

