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
#include <stdint.h>
#include "lwip/tcpip.h"
#include "eth.h"
#include "FreeRTOS.h"
#include "iodefine.h"
#include "libc_glue.h"
#include "task.h"
#include "uart.h"

/****************************************************************************
 *
 ****************************************************************************/
#define printf xprintf

/****************************************************************************
 *
 ****************************************************************************/
extern void _main(void* ptr);

/****************************************************************************
 *
 ****************************************************************************/
extern uint32_t vectorTable[];
static uint32_t ALIGNED(4096) pageTable0[1024];
static uint32_t ALIGNED(16384) baseTable[4096];

/****************************************************************************
 *
 ****************************************************************************/
static void timerISR()
{
   vTaskIncrementTick();
#if configUSE_PREEMPTION == 1
   vTaskSwitchContext();
#endif
   TSTAT1 = 0x00000001;
}

/****************************************************************************
 *
 ****************************************************************************/
void vApplicationIdleHook()
{
   portWAIT();
}

/****************************************************************************
 *
 ****************************************************************************/
void mDelay(unsigned long value)
{
   int state = portDISABLE_INTERRUPTS();
   uint32_t cmp = configCPU_FREQUENCY / 1000;
   cmp /= (PCDR1 & 0x0000003F) + 1;
   cmp *= value;

   PCCR1 |= 0x04000000;

   TCTL2 = 0x00000400;
   TPRER2 = 0;
   TCTL2 = 0x00000103;

   while (TCN2 < cmp);

   PCCR1 &= ~0x04000000;

   if (state)
      portENABLE_INTERRUPTS();
}

/****************************************************************************
 *
 ****************************************************************************/
int boardInit0()
{
   int i;

   /* Here's the problem...  The vector table is in ROM, but we need to
    * install our own handlers.  Luckily, we can solve this problem by using
    * the MMU.  We're going to map all virtual addresses to their same
    * physical address, except for the first 4KB of memory.  We will map that
    * to the SDRAM, so we can change things.
    */

   /* load permission domains (everyone can do everything) */
   __asm__ __volatile__("mcr p15, 0, %0, c3, c0, 0" : : "r" (0xFFFFFFFF));

   /* make 4KB@0x00000000 map to vectorTable (make cacheable too) */
   pageTable0[0] = (uintptr_t) vectorTable | 0x0000000E;

   /* identity map the rest */
   for (i = 1; i < 256; i++)
      pageTable0[i] = (i << 12) | 0x00000002;

   baseTable[0] = (uintptr_t) pageTable0 | 0x00000011;

   /* identity map big 1MB chunks */
   for (i = 1; i < 4096; i++)
      baseTable[i] = (i << 20) | 0x00000C12;

   /* only make the SDRAM memory cacheable */
   for (i = (0xC0000000 >> 20); i < ((0xC0000000 >> 20) + 64); i++)
      baseTable[i] = (i << 20) | 0x00000C1E;

   for (i = 0; i < 64; i++)
      configIRQ_HANDLER(i) = 0x00000000;

   portENABLE_MMU(baseTable);

   libcInit();
   uartInit(UART1, UART_BAUD_KEEP, UART_DPS_KEEP);
   tcpip_init(NULL, NULL);
   ethInit();

   /* Now we must set up the timer for use by FreeRTOS.  First we enable the
    * clock for General Purpose Timer 1 (GPT1_EN), which is really driven by
    * PERCLK1.
    */
   PCCR1 |= 0x02000000;
   TPRER1 = 0;
   TCMP1 = configCPU_FREQUENCY / (((PCDR1 & 0x0000003F) + 1) * 
           configTICK_RATE_HZ);
   TCTL1 = 0x00000013;
   configIRQ_HANDLER(26) = timerISR;
   NIPRIORITY3 &= 0xFFFFF0FF;
   NIPRIORITY3 |= (configTIMER_INTERRUPT_PRIORITY & 0xF) << 8;
   INTENNUM = 26;

   xTaskCreate(_main, (signed char*) "main", configMINIMAL_STACK_SIZE, NULL,
               configMAX_PRIORITIES - 1, NULL);

   vTaskStartScheduler();

   return 0;
}
