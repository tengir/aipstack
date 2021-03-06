/*
 * Copyright (c) 2017 Ambroz Bizjak
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdio>
#include <csignal>
#include <stdexcept>

#include <uv.h>

#include "libuv_app_helper.h"

namespace AIpStackExamples {

static char const * GetSignalName (int signum)
{
    return (signum == SIGINT)  ? "SIGINT" :
           (signum == SIGTERM) ? "SIGTERM" :
           (signum == SIGHUP)  ? "SIGHUP" :
#ifndef _WIN32
           (signum == SIGQUIT) ? "SIGQUIT" :
#endif
           "???";
}

LibuvAppHelper::LibuvAppHelper ()
{
    if (uv_loop_init(&m_loop) != 0) {
        throw std::runtime_error("uv_loop_init failed");
    }
    
    for (int i = 0; i < NumWatchedSignals; i++) {
        int signum = WatchedSignals[i];
        UvHandleWrapper<uv_signal_t> &sig_wrapper = m_signals[i];
        int res;
        
        res = sig_wrapper.initialize([&](uv_signal_t *dst) {
            return uv_signal_init(&m_loop, dst);
        });
        if (res != 0) {
            throw std::runtime_error("uv_signal_init failed");
        }
        
        sig_wrapper.get()->data = this;
        
        res = uv_signal_start(sig_wrapper.get(),
                              &LibuvAppHelper::signalHandlerTrampoline, signum);
        if (res != 0) {
            throw std::runtime_error("uv_signal_start failed");
        }
    }
}

LibuvAppHelper::~LibuvAppHelper ()
{
    for (UvHandleWrapper<uv_signal_t> &sig_wrapper : m_signals) {
        sig_wrapper.detach();
    }
    
    if (uv_run(&m_loop, UV_RUN_DEFAULT) != 0) {
        std::fprintf(stderr, "uv_run for cleanup returned nonzero!\n");
    }
    
    if (uv_loop_close(&m_loop) != 0) {
        std::fprintf(stderr, "uv_loop_close failed!\n");
    }
}

int LibuvAppHelper::run ()
{
    return uv_run(&m_loop, UV_RUN_DEFAULT);
}

void LibuvAppHelper::signalHandlerTrampoline (uv_signal_t *handle, int signum)
{
    LibuvAppHelper *obj = reinterpret_cast<LibuvAppHelper *>(handle->data);
    obj->signalHandler(signum);
}

void LibuvAppHelper::signalHandler (int signum)
{
    std::fprintf(stderr, "Got signal %s, terminating...\n", GetSignalName(signum));
    
    uv_stop(&m_loop);
}

}
