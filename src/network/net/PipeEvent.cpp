/********************************************************************
 * @Author: jiangshan yaoranyaoran2015@outlook.com
 * @Date: 2025-06-16 18:10:52
 * @LastEditors: jiangshan yaoranyaoran2015@outlook.com
 * @LastEditTime: 2025-07-11 14:30:47
 * @FilePath: /liveStream-study/src/network/net/PipeEvent.cpp
 * @Description:
 * @
 * @Copyright (c) 2025 by jiangshan yaoranyaoran2015@outlook.com, All Rights Reserved.
 *********************************************************************/

#include <unistd.h>
#include <fcntl.h>
#include <error.h>

#include "network/base/Network.h"
#include "PipeEvent.h"

using namespace liveStream::network;

PipeEvent::PipeEvent(EventLoop *loop)
: Event(loop)
{
    int pipefd[2];
    auto ret = ::pipe2(pipefd, O_NONBLOCK);
    if (ret < 0)
    {
        LOG(ERROR) << "create pipe failed!";
        exit(-1);
    }
    
    m_fd = pipefd[0];
    m_writeFd = pipefd[1];
    
}

liveStream::network::PipeEvent::~PipeEvent()
{
    if (m_writeFd > 0)
    {
        ::close(m_writeFd);
        m_writeFd = -1;
    }
}

void PipeEvent::onRead()
{
    uint64_t data;
    auto ret = ::read(m_fd, &data, sizeof(data));
    if (ret < 0)
    {
        LOG(ERROR) << "read pipe failed : " << ::strerror(errno);
    }

    // LOG(INFO) << "read pipe data : " << data;
}

void PipeEvent::onClose()
{
    if (m_writeFd > 0)
    {
        ::close(m_writeFd);
        m_writeFd = -1;
    }
    
}

void PipeEvent::onError(const std::string &msg)
{
    LOG(ERROR) << "PipeEvent error : " << msg;
}

void PipeEvent::write(const char *data, size_t len)
{
    auto ret = ::write(m_writeFd, data, len);
}