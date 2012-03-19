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
#include "board.h"
#include "FreeRTOS.h"
#include "iodefine.h"
#include "semphr.h"
#include "task.h"
#include "uart.h"

/****************************************************************************
 *
 ****************************************************************************/
#define REG_BASE(uart) \
   ((volatile unsigned long*) (0x1000A000 + uart * 0x1000))

#define URXD   (0x00 / 4)
#define UTXD   (0x40 / 4)
#define UCR1   (0x80 / 4)
#define UCR2   (0x84 / 4)
#define UCR3   (0x88 / 4)
#define UCR4   (0x8C / 4)
#define UFCR   (0x90 / 4)
#define USR1   (0x94 / 4)
#define USR2   (0x98 / 4)
#define UESC   (0x9C / 4)
#define UTIM   (0xA0 / 4)
#define UBIR   (0xA4 / 4)
#define UBMR   (0xA8 / 4)
#define UBRC   (0xAC / 4)
#define ONEMS  (0xB0 / 4)
#define UTS    (0xB4 / 4)

/****************************************************************************
 *
 ****************************************************************************/
static xSemaphoreHandle rxLock[] = {NULL, NULL};
static xSemaphoreHandle txLock[] = {NULL, NULL};

/****************************************************************************
 *
 ****************************************************************************/
int uartRx(int uart, bool blocking)
{
   volatile unsigned long* base = REG_BASE(uart);
   unsigned long flags = portCPU_FLAGS();
   int c = -1;

   if ((xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) &&
       ((flags & (PORT_I_BIT | PORT_F_BIT)) == 0))
   {
      while (xSemaphoreTake(rxLock[uart], portMAX_DELAY) == pdFALSE);

      while (base[UTS] & 0x00000020)
      {
         if (!blocking)
            break;

         /* TODO: move to interrupt based */
         vTaskDelay(uxTaskGetNumberOfTasks());
      }

      if ((base[UTS] & 0x00000020) == 0)
         c = (unsigned char) base[URXD];

      xSemaphoreGive(rxLock[uart]);
   }
   else
   {
      while (base[UTS] & 0x00000020)
      {
         if (!blocking)
            return c;
      }

      c = (unsigned char) base[URXD];
   }

   return c;
}

/****************************************************************************
 *
 ****************************************************************************/
void uartTx(int uart, int c)
{
   volatile unsigned long* base = REG_BASE(uart);
   unsigned long flags = portCPU_FLAGS();

   if ((xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) &&
       ((flags & (PORT_I_BIT | PORT_F_BIT)) == 0))
   {
      while (xSemaphoreTake(txLock[uart], portMAX_DELAY) == pdFALSE);

      /* TODO: move to interrupt based */
      while (base[UTS] & 0x00000010)
         vTaskDelay(uxTaskGetNumberOfTasks());

      base[UTXD] = (unsigned char) c;

      xSemaphoreGive(txLock[uart]);
   }
   else
   {
      while (base[UTS] & 0x00000010);
      base[UTXD] = (unsigned char) c;
   }
}

/****************************************************************************
 *
 ****************************************************************************/
int uartInit(int uart, int baud, int dps)
{
   volatile unsigned long* base = REG_BASE(uart);
   unsigned long ubir = base[UBIR];
   unsigned long ubmr = base[UBMR];
   unsigned long ucr2 = base[UCR2] & 0x000001E0;

   /* turn on the clocks */
   switch (uart)
   {
      case UART1: PCCR0 |= 0x00000001; break;
      case UART2: PCCR0 |= 0x00000002; break;
      default: return -1;
   }

   /* create the lock, if need be */
   if (rxLock[uart] == NULL)
      rxLock[uart] = xSemaphoreCreateMutex();
   if (txLock[uart] == NULL)
      txLock[uart] = xSemaphoreCreateMutex();

   /* reset the UART and wait for clear */
   base[UCR2] = 0x00000000;
   while ((base[UCR2] & 0x00000001) == 0);

   /* set RFDIV to 1 */
   base[UFCR] = 0x00000A81;

   /* configure the baud */
   switch (baud)
   {
      case UART_BAUD_KEEP: baud = 0; break;
      case UART_BAUD_9600: baud = 9600; break;
      case UART_BAUD_19200: baud = 19200; break;
      case UART_BAUD_38400: baud = 38400; break;
      case UART_BAUD_57600: baud = 57600; break;
      case UART_BAUD_115200: baud = 115200; break;
      default: return -1;
   }

   if (baud != 0)
   {
      ubir = (unsigned long) baud * 16 * ((PCDR1 & 0x0000003F) + 1);
      ubir /= configCPU_FREQUENCY / 0xFFFF;
      ubir--;
      ubmr = 0xFFFF - 1;
   }

   base[UBIR] = ubir;
   base[UBMR] = ubmr;

   /* configure the data/parity/stop settings */
   switch (dps)
   {
      case UART_DPS_KEEP: base[UCR2] |= ucr2; break;
      case UART_DPS_8N1: base[UCR2] |= 0x00000020; break;
      case UART_DPS_8N2: base[UCR2] |= 0x00000060; break;
      case UART_DPS_8Y1: base[UCR2] |= 0x00000120; break;
      case UART_DPS_8Y2: base[UCR2] |= 0x00000160; break;
      case UART_DPS_7N1: base[UCR2] |= 0x00000000; break;
      case UART_DPS_7N2: base[UCR2] |= 0x00000040; break;
      case UART_DPS_7Y1: base[UCR2] |= 0x00000100; break;
      case UART_DPS_7Y2: base[UCR2] |= 0x00000140; break;
      default: return -1;
   }

   portDISABLE_INTERRUPTS();

   /* let the UART settle*/
   mDelay(1);

   /* enable the UART */
   base[UCR1] |= 0x00000001;
   base[UCR2] |= 0x00000006;

   return 0;
}
