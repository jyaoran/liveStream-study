/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-21 16:17:47
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-21 16:48:30
 * @FilePath: /liveStream-study/src/mmedia/base/MMediaLog.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include "base/easylogging++.h"
#include <iostream>


#define RTMP_DEBUG_ON 1
#define HTTP_DEBUG_ON 1
#define DEMUX_DEBUG_ON 1
#define MPEGTS_DEBUG_ON 1
#define HLS_DEBUG_ON 1
#define WEBRTC_DEBUG_ON 1

// 这些可以不开,debug才开
#ifdef RTMP_DEBUG_ON
#define RTMP_TRACE LOG(TRACE) << "RTMP::"
#define RTMP_DEBUG LOG(DEBUG) << "RTMP::"
#define RTMP_INFO LOG(INFO) << "RTMP::"
#else
#define RTMP_TRACE \
    if (0)         \
    LOG(TRACE)
#define RTMP_DEBUG \
    if (0)         \
    LOG(DEBUG)
#define RTMP_INFO \
    if (0)        \
    LOG(INFO)
#endif

// 出错和警告必须开
#define RTMP_WARN LOG(WARNING) << "RTMP::"
#define RTMP_ERROR LOG(ERROR) << "RTMP::"

//// http

// 这些可以不开,debug才开
#ifdef HTTP_DEBUG_ON
#define HTTP_TRACE LOG(TRACE) << "HTTP::"
#define HTTP_DEBUG LOG(DEBUG) << "HTTP::"
#define HTTP_INFO LOG(INFO) << "HTTP::"
#else
#define HTTP_TRACE \
    if (0)         \
    LOG(TRACE)
#define HTTP_DEBUG \
    if (0)         \
    LOG(DEBUG)
#define HTTP_INFO \
    if (0)        \
    LOG(INFO)
#endif

// 出错和警告必须开
#define HTTP_WARN LOG(WARNING) << "HTTP::"
#define HTTP_ERROR LOG(ERROR) << "HTTP::"

//// demux

// 这些可以不开,debug才开
#ifdef DEMUX_DEBUG_ON
#define DEMUX_TRACE LOG(TRACE) << "DEMUX::"
#define DEMUX_DEBUG LOG(DEBUG) << "DEMUX::"
#define DEMUX_INFO LOG(INFO) << "DEMUX::"
#else
#define DEMUX_TRACE \
    if (0)          \
    LOG(TRACE)
#define DEMUX_DEBUG \
    if (0)          \
    LOG(DEBUG)
#define DEMUX_INFO \
    if (0)         \
    LOG(INFO)
#endif

// 出错和警告必须开
#define DEMUX_WARN LOG(WARNING) << "DEMUX::"
#define DEMUX_ERROR LOG(ERROR) << "DEMUX::"

//// mpegts
// 这些可以不开,debug才开
#ifdef MPEGTS_DEBUG_ON
#define MPEGTS_TRACE LOG(TRACE) << "MPEGTS::"
#define MPEGTS_DEBUG LOG(DEBUG) << "MPEGTS::"
#define MPEGTS_INFO LOG(INFO) << "MPEGTS::"
#else
#define MPEGTS_TRACE \
    if (0)           \
    LOG(TRACE)
#define MPEGTS_DEBUG \
    if (0)           \
    LOG(DEBUG)
#define MPEGTS_INFO \
    if (0)          \
    LOG(INFO)
#endif

// 出错和警告必须开
#define MPEGTS_WARN LOG(WARNING) << "MPEGTS::"
#define MPEGTS_ERROR LOG(ERROR) << "MPEGTS::"

//// HLS
// 这些可以不开,debug才开
#ifdef HLS_DEBUG_ON
#define HLS_TRACE LOG(TRACE) << "HLS::"
#define HLS_DEBUG LOG(DEBUG) << "HLS::"
#define HLS_INFO LOG(INFO) << "HLS::"
#else
#define HLS_TRACE \
    if (0)        \
    LOG(TRACE)
#define HLS_DEBUG \
    if (0)        \
    LOG(DEBUG)
#define HLS_INFO \
    if (0)       \
    LOG(INFO)
#endif

// 出错和警告必须开
#define HLS_WARN LOG(WARNING) << "HLS::"
#define HLS_ERROR LOG(ERROR) << "HLS::"

//// WebRtc
// 这些可以不开,debug才开
#ifdef WEBRTC_DEBUG_ON
#define WEBRTC_TRACE LOG(TRACE) << "WEBRTC::"
#define WEBRTC_DEBUG LOG(DEBUG) << "WEBRTC::"
#define WEBRTC_INFO LOG(INFO) << "WEBRTC::"
#else
#define WEBRTC_TRACE \
    if (0)           \
    LOG(TRACE)
#define WEBRTC_DEBUG \
    if (0)           \
    LOG(DEBUG)
#define WEBRTC_INFO \
    if (0)          \
    LOG(INFO)
#endif

// 出错和警告必须开
#define WEBRTC_WARN LOG(WARNING) << "WEBRTC::"
#define WEBRTC_ERROR LOG(ERROR) << "WEBRTC::"