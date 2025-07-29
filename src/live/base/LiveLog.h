/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-07-28 18:13:04
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-28 18:15:17
 * @FilePath: /liveStream-study/src/live/base/LiveLog.h
 * @Description: 
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved. 
 *********************************************************************/
#pragma once
#include "base/easylogging++.h"
#include <iostream>


#define LIVE_DEBUG_ON 1
#define PULLER_DEBUG_ON 1

// 这些可以不开,debug才开
#ifdef LIVE_DEBUG_ON
#define LIVE_TRACE LOG(TRACE) << "LIVE::"
#define LIVE_DEBUG LOG(DEBUG) << "LIVE::"
#define LIVE_INFO  LOG(INFO) << "LIVE::"
#else
#define LIVE_TRACE if(0) LOG(TRACE) 
#define LIVE_DEBUG if(0) LOG(DEBUG) 
#define LIVE_INFO if(0) LOG(INFO) 
#endif

// 出错和警告必须开
#define LIVE_WARN LOG(WARNING)
#define LIVE_ERROR LOG(ERROR)

// 这些可以不开,debug才开
#ifdef PULLER_DEBUG_ON
#define PULLER_TRACE LOG(TRACE) << "PULLER::"
#define PULLER_DEBUG LOG(DEBUG) << "PULLER::"
#define PULLER_INFO  LOG(INFO) << "PULLER::"
#else
#define PULLER_TRACE if(0) LOG(TRACE) 
#define PULLER_DEBUG if(0) LOG(DEBUG) 
#define PULLER_INFO if(0) LOG(INFO) 
#endif

// 出错和警告必须开
#define PULLER_WARN LOG(WARNING)
#define PULLER_ERROR LOG(ERROR)