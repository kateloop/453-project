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
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "iodefine.h"
#include "semphr.h"
#include "task.h"
#include "uart.h"

/****************************************************************************
 * 1 - The safest malloc() locking mechanism by disabling interrupts.
 * 2 - Uses priority aware mutexes... only use if you know for sure that 
 *     malloc() is not called from within an interrupt service routine.
 * 3 - Lock malloc() by disabling context switches... again, only use if you
 *     know for sure that malloc() is not called by any interrupt service
 *     routines.
 ****************************************************************************/
#define MALLOC_LOCK  1

/****************************************************************************
 * Big assumption here...  I'm assuming that the BSS section is always placed
 * last by the linker.  
 ****************************************************************************/
extern uint8_t __bss_end__[];
static uint8_t* heap = __bss_end__;
#if MALLOC_LOCK == 1
static int mLockNested = 0;
static int mLockState = 0;
#elif MALLOC_LOCK == 2
static xSemaphoreHandle mLock = NULL;
#endif

/****************************************************************************
 *
 ****************************************************************************/
void __malloc_lock(struct _reent* _r)
{
#if MALLOC_LOCK == 1
   int state = portDISABLE_INTERRUPTS();
   if (mLockNested == 0)
      mLockState = state;
   mLockNested++;
#elif MALLOC_LOCK == 2
   while (xSemaphoreTake(mLock, portMAX_DELAY) == pdFALSE);
#elif MALLOC_LOCK == 3
   vTaskSuspendAll();
#endif
}

/****************************************************************************
 *
 ****************************************************************************/
void __malloc_unlock(struct _reent* _r)
{
#if MALLOC_LOCK == 1
   mLockNested--;
   if ((mLockNested == 0) && mLockState)
      portENABLE_INTERRUPTS();
#elif MALLOC_LOCK == 2
   xSemaphoreGive(mLock);
#elif MALLOC_LOCK == 3
   xTaskResumeAll();
#endif
}

/****************************************************************************
 *
 ****************************************************************************/
ssize_t _read(int fd, void* buffer, size_t count)
{
   size_t i;

   if (fd != 0)
   {
      errno = EBADF;
      return -1;
   }

   for (i = 0; i < count; i++)
   {
      int c = uartRx(UART1, i ? false : true);

      if (c == -1)
         break;
      if (c == '\r')
         c = '\n';

      ((uint8_t*) buffer)[i] = (uint8_t) c;

      /* echo the character back to the terminal */
      write(1, &((uint8_t*) buffer)[i], 1);
   }

   return i;
}

/****************************************************************************
 *
 ****************************************************************************/
ssize_t _write(int fd, const void* buffer, size_t count)
{
   size_t i;

   if ((fd != 1) && (fd != 2))
   {
      errno = EBADF;
      return -1;
   }

   for (i = 0; i < count; i++)
   {
      int c = ((const uint8_t*) buffer)[i];

      if (c == '\n')
         uartTx(UART1, '\r');

      uartTx(UART1, c);
   }

   return count;
}

/****************************************************************************
 *
 ****************************************************************************/
void* _sbrk(intptr_t increment)
{
   uint8_t* ptr;

   __malloc_lock(NULL);

   ptr = heap;
   heap += increment;

   __malloc_unlock(NULL);

   return ptr;
}

/****************************************************************************
 *
 ****************************************************************************/
int _close(int fd)
{
   if ((fd != 0) && (fd != 1) && (fd != 2))
   {
      errno = EBADF;
      return -1;
   }

   return 0;
}

/****************************************************************************
 *
 ****************************************************************************/
int _fstat(int fd, struct stat* ptr)
{
   if ((fd != 0) && (fd != 1) && (fd != 2))
   {
      errno = EBADF;
      return -1;
   }

   ptr->st_mode = S_IFCHR;
   return 0;
}

/****************************************************************************
 *
 ****************************************************************************/
off_t _lseek(int fd, off_t offset, int whence)
{
   if ((fd != 0) && (fd != 1) && (fd != 2))
   {
      errno = EBADF;
      return -1;
   }

   errno = ESPIPE;
   return -1;
}

/****************************************************************************
 *
 ****************************************************************************/
int _isatty(int fd)
{
   if ((fd != 0) && (fd != 1) && (fd != 2))
   {
      errno = EBADF;
      return 0;
   }

   return 1;
}

/****************************************************************************
 *
 ****************************************************************************/
void libcInit()
{
#if MALLOC_LOCK == 2
   mLock = xSemaphoreCreateMutex();
#endif
}
