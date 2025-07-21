/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-29 18:12:43
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-10 16:39:53
 * @FilePath: /liveStream-study/src/network/TcpClient.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once

#include "network/base/InetAddress.h"
#include "network/net/TcpConnection.h"
#include "network/net/EventLoop.h"

namespace liveStream
{
    namespace network
    {
        enum
        {
            kTcpConnectStatusInit = 0,          // 初始状态
            kTcpConnectStatusConnecting = 1,    // 正在连接中
            kTcpConnectStatusConnected = 2,     // 连接成功
            kTcpConnectStatusDisconnected = 3,  // 连接断开
        };

        class TcpClient;
        using TcpClientPtr = std::shared_ptr<TcpClient>;
        using ConnectionCallback = std::function<void(const TcpConnectionPtr&, bool)>;
        
        class TcpClient : public TcpConnection
        {
        public:
            TcpClient(EventLoop *loop, const InetAddress &serverAddr);
            // virtual 修饰的析构函数，为了确保当通过基类指针删除一个派生类对象时，能够正确的调用派生类的析构函数
            virtual ~TcpClient();
            
            void setConnectionCallback(const ConnectionCallback &cb);
            void setConnectionCallback(ConnectionCallback &&cb);
            void connect();

            void onRead() override;
            void onWrite() override;
            void onClose() override;

            void clientSend(std::list<BufferNodePtr> &msgList);
            void clientSend(const char *data, size_t len);
        private:
            void ConnectInLoop();
            void updateConnectStatus();
            bool checkError();
        
        private:
            InetAddress m_serverAddr; // 服务器地址
            uint32_t m_status{kTcpConnectStatusInit}; // 连接状态
            ConnectionCallback m_connectionCallback; // 连接回调
        };
    } // namespace network
} // namespace liveStream
