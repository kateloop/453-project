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
#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

/****************************************************************************
 *
 ****************************************************************************/
#define printf xprintf

/****************************************************************************
 *
 ****************************************************************************/
static void portThreadExit()
{
#if (INCLUDE_xTaskGetCurrentTaskHandle == 1) && (INCLUDE_vTaskDelete == 1)
   vTaskDelete(xTaskGetCurrentTaskHandle());
#else
   for (;;)
#if INCLUDE_vTaskDelay == 1
      vTaskDelay(configTICK_RATE_HZ);
#else
      taskYIELD();
#endif
#endif
}

/****************************************************************************
 *
 ****************************************************************************/
int xprintf(const char* format, ...)
{
   char buffer[128];
   va_list args;
   int count;
   int i = 0;

   va_start(args, format);
   count = vsniprintf(buffer, 128, format, args);
   va_end(args);

   while (buffer[i] != '\0')
   {
      if (buffer[i] == '\n')
      {
         uartTx(UART1, '\r');
         count++;
      }

      uartTx(UART1, buffer[i++]);
   }

   return count;
}

/****************************************************************************
 *
 ****************************************************************************/
void vPortEndScheduler()
{
   printf("system halted.\n");
   portHALT();
}

/****************************************************************************
 *
 ****************************************************************************/
portSTACK_TYPE* pxPortInitialiseStack(portSTACK_TYPE* pxTopOfStack,
                                      pdTASK_CODE pxCode,
                                      void* pvParameters)
{
   *(pxTopOfStack -  1) = (portSTACK_TYPE) portThreadExit;
   *(pxTopOfStack -  2) = (portSTACK_TYPE) pxTopOfStack;
   *(pxTopOfStack -  3) = 0xCCCCCCCC;
   *(pxTopOfStack -  4) = 0xBBBBBBBB;
   *(pxTopOfStack -  5) = 0xAAAAAAAA;
   *(pxTopOfStack -  6) = 0x99999999;
   *(pxTopOfStack -  7) = 0x88888888;
   *(pxTopOfStack -  8) = 0x77777777;
   *(pxTopOfStack -  9) = 0x66666666;
   *(pxTopOfStack - 10) = 0x55555555;
   *(pxTopOfStack - 11) = 0x44444444;
   *(pxTopOfStack - 12) = 0x33333333;
   *(pxTopOfStack - 13) = 0x22222222;
   *(pxTopOfStack - 14) = 0x11111111;
   *(pxTopOfStack - 15) = (portSTACK_TYPE) pvParameters;
   *(pxTopOfStack - 16) = (portSTACK_TYPE) pxCode + sizeof(void*);
   *(pxTopOfStack - 17) = (portSTACK_TYPE) portINITIAL_SPSR;

   return pxTopOfStack - 17;
}
