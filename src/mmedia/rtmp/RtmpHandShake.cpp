#include "RtmpHandShake.h"
/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:27:42
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-23 16:21:55
 * @FilePath: /liveStream-study/src/mmedia/rtmp/RtmpHandShake.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/
#include "RtmpHandShake.h"
#include "mmedia/base/MMediaLog.h"
#include "base/TTime.h"
#include <cstring>
#include <random>
#include <openssl/sha.h>
#include <openssl/hmac.h>

using namespace liveStream::mm;

/// 借用openssl的hmac-sha算法加密
/// 因为版本不同,接口不同，选用可以使用的版本
#if OPENSSL_VERSION_NUMBER > 0x10100000L
#define HMAC_setup(ctx, key, len) \
    ctx = HMAC_CTX_new();         \
    HMAC_Init_ex(ctx, key, len, EVP_sha256(), 0)

#define HMAC_crunch(ctx, buf, len) HMAC_Update(ctx, buf, len)

#define HMAC_finish(ctx, dig, dlen) \
    HMAC_Final(ctx, dig, &dlen);    \
    HMAC_CTX_free(ctx)

#else

#define HMAC_setup(ctx, key, len) \
    HMAC_CTX_init(&ctx);          \
    HMAC_Init_ex(&ctx, key, len, EVP_sha256(), 0)
#define HMAC_crunch(ctx, buf, len) HMAC_Update(&ctx, buf, len)
#define HMAC_finish(ctx, dig, dlen) \
    HMAC_Final(&ctx, dig, &dlen);   \
    HMAC_CTX_cleanup(&ctx)

#endif // OPENSSL_VERSION_NUMBER

namespace
{
    /// @brief 服务端的版本号，4字节，协议规定的这些值
    /// 0x0D:(CR)回车符， 0x0E:版本号 , 0x0A:(LR)换行符
    static const unsigned char rtmp_server_ver[4] =
        {
            0x0D, 0x0E, 0x0A, 0x0D
        };

    /// @brief  客户端的版本号，4字节
    /// 0x0C,表示换页符 , 0x00 :这是一个空字节，通常用作分隔符、填充或表示结束 , 0x0E : 版本号
    static const unsigned char rtmp_client_ver[4] =
        {
            0x0C, 0x00, 0x0D, 0x0E
        };

///  以固定的key,做hmac-sha256计算得到digest。下面两个是固定的key
#define PLAYER_KEY_OPEN_PART_LEN 30 ///< length of partial key used for first client digest signing

    /** Client key used for digest signing */ // 都是协议固定的值
    static const uint8_t rtmp_player_key[] = {
        'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
        'F', 'l', 'a', 's', 'h', ' ', 'P', 'l', 'a', 'y', 'e', 'r', ' ', '0', '0', '1',

        0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1, 0x02,
        0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB, 0x93, 0xB8,
        0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
    };

#define SERVER_KEY_OPEN_PART_LEN 36 ///< 长度为部分密钥用于第一次服务器摘要签名
    /** Key used for RTMP server digest signing */
    static const uint8_t rtmp_server_key[] = {
        'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
        'F', 'l', 'a', 's', 'h', ' ', 'M', 'e', 'd', 'i', 'a', ' ',
        'S', 'e', 'r', 'v', 'e', 'r', ' ', '0', '0', '1',

        0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1, 0x02,
        0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB, 0x93, 0xB8,
        0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
    };

    /// @brief 使用固定的key 计算digest ，其中digest-data的大小是固定32bytes
    /// @param src  源buffer
    /// @param len  buffer长度
    /// @param gap  指定中间有没有间隔，间隔就是digest-data的值，没有就计算左右两边
    /// @param key  固定的，客户端和服务端不同
    /// @param keylen key的长度
    /// @param dst 加密之后的结果
    void CalculateDigest(const uint8_t *src, int len, int gap, const uint8_t *key, int keylen, uint8_t *dst)
    {
        uint32_t digestLen = 0;
#if OPENSSL_VERSION_NUMBER > 0x10100000L
        HMAC_CTX *ctx;
#else
        HMAC_CTX ctx;
#endif
        HMAC_setup(ctx, key, keylen);
        if (gap <= 0)
        {
            HMAC_crunch(ctx, src, len);
        }
        else
        {
            HMAC_crunch(ctx, src, gap);
            // src+gap+SHA256_DIGEST_LENGTH  右边
            // len-gap-SHA256_DIGEST_LENGTH 左边
            HMAC_crunch(ctx, src + gap + SHA256_DIGEST_LENGTH, len - gap - SHA256_DIGEST_LENGTH);
        }
        HMAC_finish(ctx, dst, digestLen);
    }

    /// @brief sha算法不可逆，要再计算携带的消息的digest和自己保存是是否相同
    /// @param buf  接收的消息
    /// @param digest_pos diest-data所在的位置
    /// @param key
    /// @param keyLen
    /// @return 对比digest-data是否相同
    bool VerifyDigest(uint8_t *buf, int digest_pos, const uint8_t *key, size_t keyLen)
    {
        uint8_t digest[SHA256_DIGEST_LENGTH];
        // 计算新的加密值
        CalculateDigest(buf, 1536, digest_pos, key, keyLen, digest);
        // 对比digest-data是否相同
        return memcmp(&buf[digest_pos], digest, SHA256_DIGEST_LENGTH) == 0;
    }

    /// @brief 获取在c1 / s1结构中的digest的偏移位置
    /// @param buf  c1 / s1的数据
    /// @param off 整个digest的总偏移值
    /// @param mod_val 余数（其实是固定的 764 - 4 - 32）
    /// @return digest-data的总偏移量
    int32_t GetDigestOffset(const uint8_t *buf, int off, int mod_val)
    {
        uint32_t offset = 0;
        // off有两种，一个是4字节，一个是4+764字节，看怎么排放的
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(buf + off);
        uint32_t res;

        // digest的开头是data的offset，4字节，现在加起来
        offset = ptr[0] + ptr[1] + ptr[2] + ptr[3];
        // offset % mod_val ： 怕超出764字节
        // off + 4： 总偏移量 + 前面的timestamp + version(4bytes)
        res = (offset % mod_val) + (off + 4);
        return res;
    }
}

RtmpHandShake::RtmpHandShake(const TcpConnectionPtr &conn, bool client)
    : connection_(conn), is_client_(client)
{
}

void RtmpHandShake::Start()
{
    CreateC1S1();   // 携带C0C1
    if(is_client_)  // 客户端就发送C0C1
    {
        state_ = kHandShakePostC0C1;
        SendC1S1();
    }
    else            // 服务端先等待
    {
        state_ = kHandShakeWaitC0C1;
    }
}

int32_t RtmpHandShake::HandShake(MsgBuffer &buf)
{
 switch (state_)
    {
        case kHandShakeWaitC0C1:        // 服务端等待客户端发送C0C1，收到就是进行检测，处理，然后发送S0S1
        {
            
            if(buf.ReadableBytes() < 1537)  // 数据不够C0C1 + S0S1
            {
                return 1;
            }
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",Recv C0C1";
            // 数据够了
            auto offset = CheckC1S1(buf.Peek(), 1537);
            if(offset >= 0) // 检测成功
            {
                // C1接收成功了，创建S2准备下一轮的发送
                CreateC2S2(buf.Peek() + 1, 1536, offset);
                buf.Retrieve(1537);
                state_ = kHandShakePostS0S1;    // 发送S0S1
                SendC1S1(); // S0S1 == S1
            }
            else
            {
                RTMP_ERROR << "host : " << connection_->getPeerAddress().toIpPort() << ",check C0C1 error\n";
                return -1;
            }
            break;
        }
        case kHandShakeWaitC2:          // 服务端收到客户端发送C2的状态，就可以检测，标记结束
        {
            if(buf.ReadableBytes() < 1536)  // 数据不够C2的大小
            {
                return 1;
            }
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ", Recv C2";
            if(CheckC2S2(buf.Peek(), 1536))
            {
                buf.Retrieve(1536);
                RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ", handshake done";
                state_ = kHandShakeDone; // 完成
                return 0;
            }
            else
            {
                RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ", check C2 error";        
                return -1;
            }
            break;
        }
        case kHandShakeWaitS0S1:    // 客户端等到服务端的S0S1
        {
            if(buf.ReadableBytes() < 1537)  // 数据不够S0S1的长度
            {
                return 1;
            }
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",Recv S0S1.";
            // 数据够了
            auto offset = CheckC1S1(buf.Peek(), 1537);
            if(offset >= 0) // 检测成功
            {
                // S1接收成功了，创建C2准备下一轮的发送
                CreateC2S2(buf.Peek() + 1, 1536, offset);
                // 移除掉S0S1
                buf.Retrieve(1537);

                // 因为服务端发送完了S0S1，接着就发S2了，判断S2是否同时到达，写入了buf中
                if(buf.ReadableBytes() == 1536)  // S2
                {
                    // S2一起接收到了，状态改变位即将结束的状态
                    // 并不对S2进行任何处理，所以直接进入done状态
                    RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",Recv S2.";
                    state_ = kHandShakeDoning;  
                    buf.Retrieve(1536);
                    // 发送C2完成后，会将状态置为done
                    SendC2S2();
                    return 0;
                }
                else // 没有随着S0S1一起收到S2
                {
                    state_ = kHandShakePostC2;  // 状态变了，因为没有收到S2，就是准备发送C2的状态
                    SendC2S2();
                    // 发送完成，此时状态更改位done，
                    // 即使S2延迟收到，也不影响握手成功
                }
            }
            else
            {
                RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",check S0S1 error.";
                return -1;
            }
            break;
        }
    }
    return 1;
}

void RtmpHandShake::WriteComplete()
{
    switch (state_)
    {
        case kHandShakePostS0S1:    // 服务端状态发送完S0S1，准备发送S2
        {
            // 发送S2
            state_ = kHandShakePostS2;
            SendC2S2();
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",Wait C2.";
            break;
        }
        case kHandShakePostS2:      // 服务端状态，服务端发送完成S2，等待客户端的C2
        {
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",send S2 cpmplete.";
            state_ = kHandShakeWaitC2; // 等待客户端的C2
            break;
        }
        case kHandShakePostC0C1:    // 客户端状态， 发送完C0C1，等待服务端的S0S1
        {
            state_ = kHandShakeWaitS0S1; // 等待服务端的S0S1
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",wait S0S1.";
            break;
        }
        case kHandShakePostC2:          // 客户端收到S0S1之后，但是没有连续收到S2，就变为PostC2完成状态 。客户端PostC2就完成
        {
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",Post C2.done";
            state_ = kHandShakeDone;
            break;
        }
        case kHandShakeDoning:
        {
            RTMP_TRACE << "host : " << connection_->getPeerAddress().toIpPort() << ",wait S0S1.";
            state_ = kHandShakeDone; // 完成
            break;
        }
    }
}

// 随机生成一个字节
uint8_t RtmpHandShake::GenRandom()
{
    // 初始化随机数生成器
    std::mt19937 mt{std::random_device{}()};
    // 离散均匀分布类，指定随机数范围,不超过uint8_t, 1字节
    std::uniform_int_distribution<> rand(0, 256);

    return rand(mt) % 256;
}


// 填充C1/S1的结构
void RtmpHandShake::CreateC1S1()
{
    // C1/S1的数据大多都是随机数，所以这里先用随机数进行填充
    for (size_t i = 0; i < kRtmpHandShakePacketSize + 1; i++)
    {
        C1S1_[i] = GenRandom();
    }
    
    C1S1_[0] = 0x03; // version

    // 填充C1/S1的timestamp， 可以是任意值，协议没有规定，这里清零
    memset(C1S1_ + 1, 0x00, 4);

    // 判断是否启用简单握手方式
    if (is_complex_handshake_ == false)
    {
        memset(C1S1_ + 5, 0x00, 4);  // 简单握手方式，C1/S1的zero字段清零
    }
    else    // 复杂握手方式
    {
        // 这里采用digest字段在key字段前的方案，直接通过4字节的偏移量取余获取到digest-data的位置
        // 这里的偏移量是固定的，等于764 - 4 - 32 = 72
        // C1S1_ + 1 ： 跳过version字段（C0/S0）
        // 8 ： 跳过timestamp + offset字段
        auto offset = GetDigestOffset(C1S1_ + 1, 8, 728);
        uint8_t *data = C1S1_ + offset + 1;
        if (is_client_)
        {
            memcpy(C1S1_ + 5, rtmp_client_ver, 4); // 客户端的版本号
            CalculateDigest(C1S1_ + 1, kRtmpHandShakePacketSize, offset, 
                            rtmp_player_key, PLAYER_KEY_OPEN_PART_LEN, data);
        }
        else
        {
            memcpy(C1S1_ + 5, rtmp_server_ver, 4); // 服务端的版本号
            CalculateDigest(C1S1_ + 1, kRtmpHandShakePacketSize, offset, 
                            rtmp_server_key, SERVER_KEY_OPEN_PART_LEN, data);

        }
        memcpy(digest_, data, SHA256_DIGEST_LENGTH);
    }
}

/// @brief  校验C1/S1的正确性
/// @param data  接收到的C1/S1数据
/// @param bytes  数据长度
/// @return 成功返回digest-data的offset，失败返回-1, =0表示简单握手方式
int32_t RtmpHandShake::CheckC1S1(const char *data, int bytes)
{
    if (bytes < 1537)
    {
        RTMP_ERROR << "unexpected C1S1, len : " << bytes;
        return -1;
    }

    if (data[0] != 0x03)
    {
        RTMP_ERROR << "unexpected version, version : " << data[0];
        return -1;
    }
    
    uint32_t *version = (uint32_t *)(data + 5);
    if (*version == 0)
    {
        is_complex_handshake_ = false;
        return 0;
    }
    
    uint32_t offset = 0;
    if (is_complex_handshake_)
    {
        // 复杂握手方式
        uint8_t *handshakeData = (uint8_t *)(data + 1);
        offset = GetDigestOffset(handshakeData, 8, 728);
        if (is_client_)
        {
            // 先验证第一种格式， digest在key字段前的方案
            // client验证服务端的key
            if (!VerifyDigest(handshakeData, offset, rtmp_server_key, SERVER_KEY_OPEN_PART_LEN))
            {
                // 不是第一种验证方式，采取第二种
                // key在digest字段前的方案，偏移764(key length) + 8(timestamp + server_ver)；
                offset = GetDigestOffset(handshakeData, 772, 728);
                if (!VerifyDigest(handshakeData, offset, rtmp_server_key, SERVER_KEY_OPEN_PART_LEN))
                {
                    return -1;
                }                
            }
        }
        else
        {
            if (!VerifyDigest(handshakeData, offset, rtmp_player_key, PLAYER_KEY_OPEN_PART_LEN))
            {
                // 不是第一种验证方式，采取第二种
                // key在digest字段前的方案，偏移764(key length) + 8(timestamp + server_ver)；
                offset = GetDigestOffset(handshakeData, 772, 728);
                if (!VerifyDigest(handshakeData, offset, rtmp_player_key, PLAYER_KEY_OPEN_PART_LEN))
                {
                    return -1;
                }                
            } 
        }
        
    }
    return offset;
}

// 发送C1/S1
void RtmpHandShake::SendC1S1()
{
    connection_->send((const char *)C1S1_, kRtmpHandShakePacketSize + 1);
}

/// @brief  计算digest-data
// S2: 先通过C1的digest-data计算出一个key，然后用这个key计算出S2的digest-data，再发送给客户端
// C2: 先通过S1的digest-data计算出一个key，然后用这个key计算出C2的digest-data，再发送给服务端
/// @param data  接收到的C1/S1数据
/// @param bytes  数据长度
/// @param offset digest-data的偏移量
void RtmpHandShake::CreateC2S2(const char *data, int bytes, int offset)
{
    for(int i = 0; i < kRtmpHandShakePacketSize; i++)
    {
        C2S2_[i] = GenRandom();
    }

    memcpy(C2S2_, data, 8); // data是C1S1的前八字节,timestamp和version

    auto timestamp = liveStream::base::TTime::getCurrentTime();      //设置当前时间戳，version不变
    char* t = (char*)&timestamp;    // C2S2_是单字节的数组，要转换下
    // 网络端要转换字节序，小端变大端 ，timestamp一共4字节
    C2S2_[3] = t[0];
    C2S2_[2] = t[1];
    C2S2_[1] = t[2];
    C2S2_[0] = t[3];

    if (is_complex_handshake_)
    {
        uint8_t key_data[SHA256_DIGEST_LENGTH];
        if (is_client_)
        {
            // 客户端利用S1的digest-data计算出key
            CalculateDigest((const uint8_t *)data + offset, 32, 0, 
                        rtmp_player_key, sizeof(rtmp_player_key), key_data);
        }
        else
        {
            // 服务端利用C1的digest-data计算出key
            CalculateDigest((const uint8_t *)data + offset, 32, 0, 
                        rtmp_server_key, sizeof(rtmp_server_key), key_data);
        }
        
        // 使用计算出来的key计算出C2/S2的digest-data
        CalculateDigest(C2S2_, kRtmpHandShakePacketSize - 32, 0, key_data, sizeof(key_data), 
                        &C2S2_[kRtmpHandShakePacketSize - 32]);
    }
    
}

/// @brief  校验C2/S2的正确性
/// @param data  接收到的C2/S2数据
/// @param bytes  数据长度
/// @return 成功返回true，失败返回false
bool RtmpHandShake::CheckC2S2(const char *data, int bytes)
{
    // 没有使用
    return true;
}

// 发送C2/S2
void RtmpHandShake::SendC2S2()
{
    connection_->send((const char *)C2S2_, kRtmpHandShakePacketSize);
}