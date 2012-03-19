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
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/****************************************************************************
 *
 ****************************************************************************/
#define configASSERT(x)                                    \
   do                                                      \
   {                                                       \
      if (!x)                                              \
      {                                                    \
         xprintf("\nASSERT - %s:%d\n",__FILE__, __LINE__); \
         portHALT();                                       \
      }                                                    \
   } while (0)

/****************************************************************************
 *
 ****************************************************************************/
#define configUSE_PREEMPTION             1
#define configUSE_IDLE_HOOK              1
#define configUSE_TICK_HOOK              0
#define configCPU_FREQUENCY              256000000
#define configTICK_RATE_HZ               1000
#define configMAX_PRIORITIES             3
#define configMINIMAL_STACK_SIZE         8192
#define configIRQ_STACK_SIZE             4096
#define configMAX_TASK_NAME_LEN          16
#define configIDLE_SHOULD_YIELD          1
#define configUSE_CO_ROUTINES            0
#define configMAX_CO_ROUTINE_PRIORITIES  0
#define configUSE_16_BIT_TICKS           0
#define configUSE_MUTEXES                1
#define configUSE_COUNTING_SEMAPHORES    1
#define configUSE_RECURSIVE_MUTEXES      1
#define configUSE_TRACE_FACILITY         1

/****************************************************************************
 *
 ****************************************************************************/
#define configIRQ_HANDLER(n)            (((volatile void**) 0xFFFFFF00)[n])
#define configTIMER_INTERRUPT_PRIORITY  0
#define configETH_INTERRUPT_PRIORITY    1

/****************************************************************************
 *
 ****************************************************************************/
#define INCLUDE_xTaskGetSchedulerState     1
#define INCLUDE_xTaskGetCurrentTaskHandle  1
#define INCLUDE_vTaskDelete                1
#define INCLUDE_vTaskDelay                 1
#define INCLUDE_vTaskDelayUntil            0
#define INCLUDE_vTaskSuspend               0
#define INCLUDE_vTaskPrioritySet           0
#define INCLUDE_uxTaskPriorityGet          0

/****************************************************************************
 *
 ****************************************************************************/
#define configUSE_DHCP     1

#define configGATEWAY_3  192
#define configGATEWAY_2  168
#define configGATEWAY_1   10
#define configGATEWAY_0    1

#define configNETMASK_3  255
#define configNETMASK_2  255
#define configNETMASK_1  255
#define configNETMASK_0    0

#define configIP_3       192
#define configIP_2       168
#define configIP_1        10
#define configIP_0       200

#endif
