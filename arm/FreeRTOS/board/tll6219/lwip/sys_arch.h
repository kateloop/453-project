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
#ifndef SYS_ARCH_H
#define SYS_ARCH_H

#include "cc.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/****************************************************************************
 *
 ****************************************************************************/
#define SYS_SEM_NULL   NULL
#define SYS_MBOX_NULL  NULL

/****************************************************************************
 *
 ****************************************************************************/
typedef xSemaphoreHandle sys_sem_t;
typedef xQueueHandle sys_mbox_t;
typedef xTaskHandle sys_thread_t;
typedef int sys_prot_t;

/****************************************************************************
 *
 ****************************************************************************/
sys_sem_t sys_sem_new(u8_t count);

/****************************************************************************
 *
 ****************************************************************************/
void sys_sem_free(sys_sem_t handle);

/****************************************************************************
 *
 ****************************************************************************/
void sys_sem_signal(sys_sem_t handle);

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_sem_wait(sys_sem_t handle, u32_t timeout);

/****************************************************************************
 *
 ****************************************************************************/
sys_mbox_t sys_mbox_new(int size);

/****************************************************************************
 *
 ****************************************************************************/
void sys_mbox_free(sys_mbox_t handle);

/****************************************************************************
 *
 ****************************************************************************/
void sys_mbox_post(sys_mbox_t handle, void* msg);

/****************************************************************************
 *
 ****************************************************************************/
err_t sys_mbox_trypost(sys_mbox_t handle, void* msg);

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_mbox_fetch(sys_mbox_t handle, void** msg, u32_t timeout);

/****************************************************************************
 *
 ****************************************************************************/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t handle, void** msg);

/****************************************************************************
 *
 ****************************************************************************/
sys_thread_t sys_thread_new(char* name, void (*thread)(void* arg), void *arg,
                            int stackSize, int priority);

/****************************************************************************
 *
 ****************************************************************************/
struct sys_timeouts* sys_arch_timeouts();

/****************************************************************************
 *
 ****************************************************************************/
sys_prot_t sys_arch_protect();

/****************************************************************************
 *
 ****************************************************************************/
void sys_arch_unprotect(sys_prot_t state);

/****************************************************************************
 *
 ****************************************************************************/
void sys_init();

#endif
