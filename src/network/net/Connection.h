/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-27 17:46:27
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:33:40
 * @FilePath: /liveStream-study/src/network/net/Connection.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/

#pragma once
#include "network/base/InetAddress.h"
#include "Event.h"
#include "EventLoop.h"
#include "base/easylogging++.h"
#include <memory>
#include <functional>
#include <atomic>

namespace liveStream 
{
    namespace network 
    {
        struct BufferNode
        {
            using Ptr = std::shared_ptr<BufferNode>;
            BufferNode(void *buf, size_t size)
            : m_addr(buf), m_size(size)
            {

            }


            void *m_addr{nullptr};
            size_t m_size{0};
        };
        
        using BufferNodePtr = std::shared_ptr<BufferNode>;

        enum
        {
            kNormalContext = 0, // 普通上下文
            kRtmpContext,       // rtmp上下文
            kHttpContext,       // http上下文
            kUserDefineContext, // 用户自定义上下文
            kFlvContext,        // flv上下文
        };

        class Connection;
        using ConnectionPtr = std::shared_ptr<Connection>;
        using ContextPtr = std::shared_ptr<void>;

        using ActiveCallback = std::function<void(const ConnectionPtr&)>;

        class Connection : public Event
        {
        public:
            Connection(EventLoop* loop, int sockfd, 
                const InetAddress& localAddr, const InetAddress& peerAddr);
            virtual ~Connection() = default;


            // 地址相关方法
            void setLocalAddress(const InetAddress& localAddr);
            void setPeerAddress(const InetAddress& peerAddr);
            const InetAddress& getLocalAddress() const;
            const InetAddress& getPeerAddress() const;

            // 上下文相关方法
            void setContext(int contextType, const ContextPtr& context);
            void setContext(int contextType, ContextPtr&& context);
            
            template<typename T>
            std::shared_ptr<T> getContext(int contextType) const;
            
            void clearContext(int contextType);
            void clearAllContext();

            void setActiveCallback(const ActiveCallback& cb);
            void setActiveCallback(ActiveCallback&& cb);

            void activa();
            void deactivate();

            virtual void forceClose() = 0; // 强制关闭连接

        protected:
            InetAddress m_localAddr; // 本地地址
            InetAddress m_peerAddr; // 对端地址

        private:
            std::unordered_map<int, ContextPtr> m_contexts; // 上下文映射表
            ActiveCallback m_activeCallback; // 激活回调
            std::atomic<bool> m_active; // 是否激活
        };

        template <typename T>
        inline std::shared_ptr<T> Connection::getContext(int contextType) const
        {
            auto it = m_contexts.find(contextType);
            if (it!= m_contexts.end())
            {
                LOG(INFO) << "Get context type success";
                return std::static_pointer_cast<T>(it->second);
            }

            LOG(INFO) << "New context shared_ptr for type!"; 
            return std::shared_ptr<T>();
        }
    } // namespace network
} // namespace liveStream