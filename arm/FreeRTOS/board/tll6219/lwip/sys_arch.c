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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lwip/sys.h"
#include "sys_arch.h"

/****************************************************************************
 *
 ****************************************************************************/
#define printf xprintf

/****************************************************************************
 *
 ****************************************************************************/
typedef struct _TaskTimeout
{
  struct _TaskTimeout* next;
  struct sys_timeouts timeouts;
  xTaskHandle task;

} TaskTimeout;

/****************************************************************************
 *
 ****************************************************************************/
static TaskTimeout* taskTimeouts = NULL;
static struct sys_timeouts sysTimeouts;

/****************************************************************************
 *
 ****************************************************************************/
sys_sem_t sys_sem_new(u8_t count)
{
   xSemaphoreHandle handle = xSemaphoreCreateCounting(255, count);

   if (handle == NULL)
      return SYS_SEM_NULL;
   
   return handle;
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_sem_free(sys_sem_t handle)
{
   vSemaphoreDelete(handle);
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_sem_signal(sys_sem_t handle)
{
   xSemaphoreGive(handle);
}

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t handle, u32_t timeout)
{
   portTickType startTicks = xTaskGetTickCount();
   portTickType timeoutTicks;

   if (timeout)
      timeoutTicks = (portTickType) (timeout * portTICK_RATE_MS);
   else
      timeoutTicks = portMAX_DELAY;

   while (xSemaphoreTake(handle, timeoutTicks) == pdFALSE)
   {
      if (timeout)
         return SYS_ARCH_TIMEOUT;
   }

   return (u32_t) (xTaskGetTickCount() - startTicks) / portTICK_RATE_MS;
}

/****************************************************************************
 *
 ****************************************************************************/
sys_mbox_t sys_mbox_new(int size)
{
   xQueueHandle handle = xQueueCreate((unsigned portBASE_TYPE) size,
                                      sizeof(void*));

   if (handle == NULL)
      return SYS_MBOX_NULL;

   return handle;
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_mbox_free(sys_mbox_t handle)
{
   vQueueDelete(handle);
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_mbox_post(sys_mbox_t handle, void* msg)
{
   while (xQueueSend(handle, &msg, portMAX_DELAY) == pdFALSE);
}

/****************************************************************************
 *
 ****************************************************************************/
err_t sys_mbox_trypost(sys_mbox_t handle, void* msg)
{
   if (xQueueSend(handle, &msg, 0) == pdFALSE)
      return ERR_MEM;

   return ERR_OK;
}

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t handle, void** msg, u32_t timeout)
{
   portTickType startTicks = xTaskGetTickCount();
   portTickType timeoutTicks;
   void* junk = NULL;

   if (msg == NULL)
      msg = &junk;

   if (timeout)
      timeoutTicks = (portTickType) (timeout * portTICK_RATE_MS);
   else
      timeoutTicks = portMAX_DELAY;

   while (xQueueReceive(handle, msg, timeoutTicks) == pdFALSE)
   {
      if (timeout)
         return SYS_ARCH_TIMEOUT;
   }

   return (u32_t) (xTaskGetTickCount() - startTicks) / portTICK_RATE_MS;
}

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t handle, void** msg)
{
   void* junk = NULL;

   if (msg == NULL)
      msg = &junk;

   if (xQueueReceive(handle, msg, 0) == pdFALSE)
      return SYS_MBOX_EMPTY;

   return 0;
}

/****************************************************************************
 *
 ****************************************************************************/
sys_thread_t sys_thread_new(char* name, void (*thread)(void* arg), void* arg,
                            int stackSize, int priority)
{
   TaskTimeout* taskTimeout = malloc(sizeof(TaskTimeout));
   int state = portDISABLE_INTERRUPTS();

   memset(taskTimeout, 0, sizeof(TaskTimeout));

   taskTimeout->next = taskTimeouts;
   taskTimeouts = taskTimeout;

   xTaskCreate(thread, (signed char*) name, (unsigned short) stackSize, arg,
               priority, &taskTimeout->task);

   if (state)
      portENABLE_INTERRUPTS();   
   
   return taskTimeout->task;
}

/****************************************************************************
 *
 ****************************************************************************/
struct sys_timeouts* sys_arch_timeouts()
{
   xTaskHandle currentTask = xTaskGetCurrentTaskHandle();
   struct sys_timeouts* timeouts = NULL;
   TaskTimeout* taskTimeout = NULL;
   int state = portDISABLE_INTERRUPTS();

   timeouts = &sysTimeouts;

   taskTimeout = taskTimeouts;
   while (taskTimeout != NULL)
   {
      if (taskTimeout->task == currentTask)
      {
         timeouts = &taskTimeout->timeouts;
         break;
      }
      
      taskTimeout = taskTimeout->next;
   }

   if (state)
      portENABLE_INTERRUPTS();   

   return timeouts;
}

/****************************************************************************
 *
 ****************************************************************************/
sys_prot_t sys_arch_protect()
{
   return portDISABLE_INTERRUPTS();
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_arch_unprotect(sys_prot_t state)
{
   if (state)
      portENABLE_INTERRUPTS();
}

/****************************************************************************
 *
 ****************************************************************************/
void sys_init()
{
   memset(&sysTimeouts, 0, sizeof(struct sys_timeouts));
}
