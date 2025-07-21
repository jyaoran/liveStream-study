/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 12:15:35
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 16:16:17
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpHeader.h
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#pragma once

#include <cstdint>
#include <memory>

namespace liveStream
{
    namespace mm
    {
        // 常用的message type，都是规定好的
        enum RtmpMsgType
        {
            kRtmpMsgTypeChunkSize = 1, // 设置块大小消息，服务器或客户端使用它来设置通信中块的最大大小。
            kRtmpMsgTypeBytesRead = 3, // 确认消息，用于确认收到的数据。
            kRtmpMsgTypeUserControl,   // 用户控制消息，用于控制事件，例如流开始、流结束等。
            kRtmpMsgTypeWindowACKSize, // 窗口确认大小消息，设置需要确认的字节数。
            kRtmpMsgTypeSetPeerBW,     // 设置对等带宽消息，设置客户端的带宽。
            kRtmpMsgTypeAudio = 8,     // 音频消息，用于传输音频数据。
            kRtmpMsgTypeVideo,         // 视频消息，用于传输视频数据。
            kRtmpMsgTypeAMF3Meta = 15, // AMF3格式的数据消息，传输元数据或命令。
            kRtmpMsgTypeAMF3Shared,    // AMF3格式的共享对象消息，用于数据同步。
            kRtmpMsgTypeAMF3Message,   // AMF3格式的命令消息，用于命令和响应。
            kRtmpMsgTypeAMFMeta,       // AMF0格式的数据消息，传输元数据或命令。
            kRtmpMsgTypeAMFShared,     // AMF0格式的共享对象消息，用于数据同步。
            kRtmpMsgTypeAMFMessage,    // AMF0格式的命令消息，用于命令和响应。
            kRtmpMsgTypeMetadata = 22, // 元数据消息，传输元数据信息。
        };

        // basic header的fmt
        enum RtmpFmt
        {
            kRtmpFmt0 = 0, // 完整的消息头，包含消息流ID、时间戳、消息长度和消息类型ID。
            kRtmpFmt1,     // 部分消息头，包含时间戳差值、消息长度和消息类型ID，不包含消息流ID。
            kRtmpFmt2,     // 只有时间戳差值，不包含消息长度、消息类型ID和消息流ID。
            kRtmpFmt3,     // 没有消息头，使用前一个消息的头信息。
        };

        // stream id
        enum RtmpCSID
        {
            kRtmpCSIDCommand = 2, // 命令消息，例如 NetConnection 和 NetStream 命令。
            kRtmpCSIDAMFIni = 3,  // 使用 AMF（Action Message Format）的初始化消息。
            kRtmpCSIDAudio = 4,   // 音频数据。
            kRtmpCSIDAMF = 5,     // 使用 AMF 的通用消息。
            kRtmpCSIDVideo = 6,   // 视频数据。
        };

#define kRtmpMsID0 0
#define kRtmpMsID1 1

#pragma pack(push) // 单字节对齐，节省空间
#pragma pack(1)
        // 主要作用是记录Rtmp通信时的头部信息。
        // 由于rtmp采用块通信，除第一包外，其余数据包不包含完整的头部信息，因此需要进行构建一个header结构来进行保存头部信息
        // 头部信息包括：chunk stream id、timestamp、message length、message type id、message stream id
        struct RtmpMsgHeader
        {
            uint32_t cs_id{0};     /* chunk stream id */
            uint32_t timestamp{0}; /* timestamp (delta) */
            uint32_t msg_len{0};   /* message length */
            uint8_t msg_type{0};   /* message type id */
            uint32_t msg_sid{0};   /* message stream id */
            RtmpMsgHeader() : cs_id(0), timestamp(0), msg_len(0), msg_type(0), msg_sid(0)
            {
            }
        };
#pragma pack()
        using RtmpMsgHeaderPtr = std::shared_ptr<RtmpMsgHeader>;

    } // namespace mm

} // namespace liveStream
