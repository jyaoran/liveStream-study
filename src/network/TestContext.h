/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:13:48
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 00:32:39
 * @FilePath: /liveStream-study/src/network/TestContext.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include "network/net/TcpConnection.h"
#include <string>
#include <functional>
#include <memory>

namespace liveStream
{
    namespace network
    {
        // 回调到哪个tcpconnect, 解析的消息
        using TestMessageCallback = std::function<void(const TcpConnectionPtr&, const std::string&)>;

        class TestContext
        {
            enum
            {
                kTestContextHeader = 1,
                kTestContextBody = 2,
            };

        public:
            TestContext(const TcpConnectionPtr &con);
            ~TestContext() = default;

        public:
            int ParseMessage(MsgBuffer &buf);
            void SetTestMessageCallback(const TestMessageCallback &cb);
            void SetTestMessageCallback(TestMessageCallback &&cb);

        private:
            TcpConnectionPtr connection_;   // 解析之后要给connection
            int state_{kTestContextHeader}; // 状态机
            int32_t message_length_{0};     // 消息长度
            std::string message_body_;      // 消息体
            TestMessageCallback cb_;
        };
        
        
        
    } // namespace network
    
} // namespace liveStream