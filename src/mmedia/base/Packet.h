/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:19:02
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 12:59:37
 * @FilePath: /liveStream-study/src/mmedia/base/Packet.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include <string>
#include <memory>
#include <cstring>
#include <cstdint>

namespace liveStream
{
    namespace mm
    {
        enum
        {
            /// @brief 用二进制位表示，方便进行&操作，可能同时判断两种类型
            kPacketTypeVideo = 1,      // 视频数据包
            kPacketTypeAudio = 2,      // 音频数据包
            kPacketTypeMeta = 4,       // Rtmp meta数据包
            kPacketTypeMeta3 = 8,      // Rtmp meta3数据包
            kPacketTypeKeyFrame = 16,  // 关键帧
            kPacketTypeIDR = 32,       // IDR帧
            kPacketTypeUnKnowed = 255, // 未知类型
        };

        /**
         *
         * packet 保存着网络数据，包含数据大小，数据类型，时间戳，数据指针等信息
         * packet包括分配内存等操作
         *
         * packet 保存的音视频数据包 并不包含header。
         *
         */
        class Packet;
        using PacketPtr = std::shared_ptr<Packet>;
#pragma pack(push) // 保存字节对齐状态
#pragma pack(1)    // 因为编译器会自动对齐类字节，所以这里设置成1字节对齐节省空间
        class Packet
        {
        public:
            using ptr = std::shared_ptr<Packet>;

            Packet(int32_t size)
                : size_(size) {};
            ~Packet() {};

        public:
            // 静态方法，创建新的packet
            static Packet::ptr NewPacket(int32_t size);

            // 判断数据包类型，是否为视频数据包
            bool IsVideo() const
            {
                return (type_ & kPacketTypeVideo) == kPacketTypeVideo;
            }

            // 判断数据包类型，是否为音频数据包
            bool IsAudio() const
            {
                // 音频没有其他属性，直接判断
                return type_ == kPacketTypeAudio;
            }

            // 判断数据包类型，是否为关键帧
            bool IsKeyFrame() const
            {
                // 只有视频有关键字
                return ((type_ & kPacketTypeVideo) == kPacketTypeVideo &&
                        (type_ & kPacketTypeKeyFrame) == kPacketTypeKeyFrame);
            }

            // 判断数据包类型，是否meta数据包
            bool IsMeta() const
            {
                return type_ == kPacketTypeMeta;
            }

            // 判断数据包类型，是否meta3数据包
            bool IsMeta3() const
            {
                return type_ == kPacketTypeMeta3;
            }

            // 获取数据块大小
            int32_t PacketSize() const
            {
                return size_;
            }

            // 获取剩余空间
            int Space() const // 剩余空间
            {
                return capacity_ - size_;
            }

            // 设置数据块大小
            void SetPacketSize(size_t len)
            {
                size_ = len;
            }

            // 更新数据块大小
            void UpdatePacketSize(size_t len)
            {
                size_ += len;
            }

            // 设置数据包索引
            void SetIndex(int32_t index)
            {
                index_ = index;
            }

            // 获取数据包索引
            int32_t Index() const
            {
                return index_;
            }

            // 设置数据包类型
            void SetPacketType(int32_t type)
            {
                type_ = type;
            }

            // 获取数据包类型
            int32_t PacketType() const
            {
                return type_;
            }

            // 设置数据包时间戳
            void SetTimeStamp(uint64_t timestamp)
            {
                timestamp_ = timestamp;
            }

            // 获取数据包时间戳
            uint64_t TimeStamp() const
            {
                return timestamp_;
            }

            // 获取数据体的指针
            inline char *Data()
            {
                // 数据包是包含类的，this是开头，一直到类大小是数据的开头
                return (char *)this + sizeof(Packet);
            }

            // 获取数据额外的指针，可能是rtmp头等。
            template <typename T>
            std::shared_ptr<T> Ext() const;

            // 设置数据额外的指针，可能是rtmp头等。
            void SetExt(const std::shared_ptr<void> &ext)
            {
                ext_ = ext;
            }

        private:
            int32_t type_{kPacketTypeUnKnowed};
            uint32_t size_{0};          // packet使用的大小
            int32_t index_{-1};         // packet的索引
            uint64_t timestamp_{0};     // 时间戳
            uint32_t capacity_{0};      // packet的容量
            std::shared_ptr<void> ext_; // 指向rtmp消息头
        };

        template <typename T>
        inline std::shared_ptr<T> Packet::Ext() const
        {
            return std::static_pointer_cast<T>(ext_);
        }
#pragma pack() // 恢复对齐方式
    } // namespace mm

} // namespace liveStream
