/*
 * Copyright (c) 2016 Ambroz Bizjak
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

#ifndef AIPSTACK_ARP_PROTO_H
#define AIPSTACK_ARP_PROTO_H

#include <stdint.h>

#include <aipstack/infra/Struct.h>
#include <aipstack/proto/IpAddr.h>
#include <aipstack/proto/EthernetProto.h>

namespace AIpStack {

AIPSTACK_DEFINE_STRUCT(ArpIp4Header,
    (HwType,       uint16_t)
    (ProtoType,    uint16_t)
    (HwAddrLen,    uint8_t)
    (ProtoAddrLen, uint8_t)
    (OpType,       uint16_t)
    (SrcHwAddr,    MacAddr)
    (SrcProtoAddr, Ip4Addr)
    (DstHwAddr,    MacAddr)
    (DstProtoAddr, Ip4Addr)
)

static uint16_t const ArpHwTypeEth = 1;

static uint16_t const ArpOpTypeRequest = 1;
static uint16_t const ArpOpTypeReply   = 2;

}

#endif
