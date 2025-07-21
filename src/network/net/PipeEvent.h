/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 17:28:21
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-06-25 16:09:31
 * @FilePath: /liveStream-study/src/network/net/PipeEvent.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once

#include "Event.h"
#include <memory>

namespace liveStream 
{
    namespace network 
    {
        class PipeEvent : public Event 
        {
        public:
            using ptr = std::shared_ptr<PipeEvent>;

            PipeEvent(EventLoop *loop);
            ~PipeEvent();

            void onRead() override;
            void onClose() override;
            void onError(const std::string &msg) override;

            void write(const char *data, size_t len);

        private:
            int m_writeFd{-1};
        };
    }
}