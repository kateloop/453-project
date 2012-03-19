/****************************************************************************
 *
 * Copyright (c) 2012, The University of Wisconsin - Madison
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of The University of Wisconsin - Madison nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERISTY OF
 * WISCONSIN - MADISON BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Christopher Karle <ckarle@wisc.edu>
 *
 ****************************************************************************/
#ifndef LWIPOPTS_H
#define LWIPOPTS_H

#include <stdio.h>
#include <stdlib.h>

#define SYS_LIGHTWEIGHT_PROT   1

#define MEM_ALIGNMENT          4
#define MEM_LIBC_MALLOC        1
#define MEMP_MEM_MALLOC        1
#define MEMP_NUM_SYS_TIMEOUT  15
#define mem_realloc           realloc

#define LWIP_NETCONN             1
#define LWIP_PROVIDE_ERRNO       1
#define LWIP_DHCP                1
#define LWIP_SO_RCVTIMEO         1
#define LWIP_NETIF_LINK_CALLBACK 1
#define LWIP_NETIF_API           1

#define LWIP_PLATFORM_BYTESWAP  1
#define LWIP_PLATFORM_HTONL(x)  portBSWAP32(x)
#define LWIP_PLATFORM_HTONS(x)  portBSWAP16(x)

#define LWIP_PLATFORM_DIAG(x)   do { printf x; } while (0)
#define LWIP_PLATFORM_ASSERT(x) do { printf(x); portHALT(); } while (0)

#define TCPIP_THREAD_NAME        "tcp/ip"
#define TCPIP_THREAD_STACKSIZE   configMINIMAL_STACK_SIZE
#define TCPIP_THREAD_PRIO        (configMAX_PRIORITIES - 1)
#define TCPIP_MBOX_SIZE          15
#define TCP_MSS                  (1500 - 40)
#define TCP_SND_BUF              (2 * TCP_MSS)

#define DEFAULT_RAW_RECVMBOX_SIZE  15
#define DEFAULT_UDP_RECVMBOX_SIZE  15
#define DEFAULT_TCP_RECVMBOX_SIZE  15
#define DEFAULT_ACCEPTMBOX_SIZE    15

// Note: enabling DHCP_DEBUG seems to cause data exceptions...

//#define LWIP_DEBUG       1
//#define IP_DEBUG         LWIP_DBG_ON
//#define ETHARP_DEBUG     LWIP_DBG_ON
//#define ICMP_DEBUG       LWIP_DBG_ON
//#define UDP_DEBUG        LWIP_DBG_ON
//#define DHCP_DEBUG       LWIP_DBG_ON
//#define TCP_INPUT_DEBUG  LWIP_DBG_ON
//#define TCP_OUTPUT_DEBUG LWIP_DBG_ON
//#define TCP_DEBUG        LWIP_DBG_ON
//#define TCPIP_DEBUG      LWIP_DBG_ON
//#define PBUF_DEBUG       LWIP_DBG_ON

#endif
