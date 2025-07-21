/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:18:07
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:58:34
 * @FilePath: /liveStream-study/src/mmedia/base/MMediaHandler.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#pragma once
#include "base/NonCopyable.h"
#include "network/net/TcpConnection.h"
#include "Packet.h"
#include <memory>

/**
 *  代码中使用了大量回调函数，
 * 所以现在做一个基类统一管理公用的回调函数方便管理
 * 用基类指针就能调用，其他需要的回调可以继承增加
 */

/**
 * @brief MMediaHandler 基类
 * 基类提供了媒体句柄的基本接口。
 * 具体的业务层需要继承该类，并实现相关的接口。
 * 确保回调函数进行一个统一的管理，方便管理。
 */
namespace liveStream
{
    namespace mm
    {
        using namespace network;
        using namespace base;
        
        /// @brief MMediaHandler 基类
        /// 基类提供了媒体句柄的基本接口。
        /// 具体的业务层需要继承该类，并实现相关的接口。
        /// 确保回调函数进行一个统一的管理，方便管理。
        /// 此类为抽象类，不能直接实例化
        class MMediaHandler : public NonCopyable
        {
        public:
            virtual void OnNewConnection(const TcpConnectionPtr &conn) = 0;               // 新连接的时候，直播业务就可以处理数据，比如创建用户等
            virtual void OnConnectionDestroy(const TcpConnectionPtr &conn) = 0;           // 连接断开的时候，业务层可以回收资源，注销用户等
            virtual void OnRecv(const TcpConnectionPtr &conn, const PacketPtr &data) = 0; // 多媒体解析出来的数据，传给直播业务
            virtual void OnRecv(const TcpConnectionPtr &conn, PacketPtr &&data) = 0;
            virtual void OnActive(const ConnectionPtr &conn) = 0; // 新连接回调通知直播业务
        };

    } // namespace mm

} // namespace liveStream