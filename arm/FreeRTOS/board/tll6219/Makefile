##############################################################################
#
#  Copyright (c) 2012, The University of Wisconsin - Madison
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of The University of Wisconsin - Madison nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
#  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
#  PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERISTY OF
#  WISCONSIN - MADISON BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
#  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
#  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#  Author: Christopher Karle <ckarle@wisc.edu>
#
##############################################################################
CROSS_COMPILE = /opt/arm-2011.03/bin/arm-none-eabi-
C_FLAGS = -Wall -O2 -mcpu=arm926ej-s -fno-strict-aliasing
LD_FLAGS = --section-start=.init=0xC0200000

##############################################################################
# Board build stuff...
##############################################################################
OUTPUT_FILE = demo
VPATH = ./lwip
INCLUDES = -I. -I./lwip
S_FILES = board_asm.S
C_FILES = board.c libc_glue.c main.c uart.c eth.c sys_arch.c

##############################################################################
# RTOS build stuff...
##############################################################################
RTOS_PATH = ../../kernel
VPATH += $(RTOS_PATH) $(RTOS_PATH)/portable/GCC/ARM5_MC9328MX21 \
         $(RTOS_PATH)/portable/MemMang
INCLUDES += -I$(RTOS_PATH)/include \
            -I$(RTOS_PATH)/portable/GCC/ARM5_MC9328MX21
S_FILES += port_asm.S
C_FILES += croutine.c list.c queue.c tasks.c timers.c heap_3.c port.c

##############################################################################
# lwIP build stuff...
##############################################################################
LWIP_PATH = ../../lwip
VPATH += $(LWIP_PATH)/api $(LWIP_PATH)/core $(LWIP_PATH)/core/ipv4 \
         $(LWIP_PATH)/core/snmp $(LWIP_PATH)/netif
INCLUDES += -I$(LWIP_PATH)/include -I$(LWIP_PATH)/include/ipv4
C_FILES += api_lib.c api_msg.c err.c netbuf.c netdb.c netifapi.c sockets.c \
           tcpip.c dhcp.c init.c mem.c netif.c raw.c stats.c tcp.c tcp_out.c \
           dns.c memp.c pbuf.c sys.c tcp_in.c udp.c autoip.c icmp.c igmp.c \
           inet.c inet_chksum.c ip_addr.c ip.c ip_frag.c asn1_dec.c \
           asn1_enc.c mib2.c mib_structs.c msg_in.c msg_out.c etharp.c \
           loopif.c

##############################################################################
#
##############################################################################
OBJ_FILES = $(S_FILES:%.S=%.o) $(C_FILES:%.c=%.o)
DEP_FILES = $(S_FILES:%.S=%.d) $(C_FILES:%.c=%.d)

$(OUTPUT_FILE): $(OBJ_FILES)
	$(CROSS_COMPILE)gcc -Xlinker "$(LD_FLAGS)" -o $@ $^

%.o: %.S
	$(CROSS_COMPILE)gcc -D__ASM__ $(C_FLAGS) $(INCLUDES) -M -o $*.d $<
	$(CROSS_COMPILE)gcc -D__ASM__ $(C_FLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.c
	$(CROSS_COMPILE)gcc $(C_FLAGS) $(INCLUDES) -M -o $*.d $<
	$(CROSS_COMPILE)gcc $(C_FLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OUTPUT_FILE) $(OBJ_FILES) $(DEP_FILES)

-include $(DEP_FILES)
