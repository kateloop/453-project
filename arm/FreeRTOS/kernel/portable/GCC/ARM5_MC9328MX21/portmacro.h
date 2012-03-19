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
#ifndef PORTMACRO_H
#define PORTMACRO_H

/****************************************************************************
 *
 ****************************************************************************/
#define ALIGNED(value)   __attribute__((aligned(value)))
#define INTERRUPT(type)  __attribute__((interrupt(type)))
#define NORETURN         __attribute__((noreturn))
#define PACKED           __attribute__((packed))

/****************************************************************************
 *
 ****************************************************************************/
#define PORT_USER_MODE        0x10
#define PORT_FIQ_MODE         0x11
#define PORT_IRQ_MODE         0x12
#define PORT_SUPERVISOR_MODE  0x13
#define PORT_ABORT_MODE       0x17
#define PORT_UNDEFINED_MODE   0x1B
#define PORT_SYSTEM_MODE      0x1F
#define PORT_T_BIT            0x20
#define PORT_F_BIT            0x40
#define PORT_I_BIT            0x80

/****************************************************************************
 *
 ****************************************************************************/
#define portTASK_FUNCTION_PROTO(vFunction, pvParameters) \
   void vFunction(void* pvParameters)
#define portTASK_FUNCTION(vFunction, pvParameters) \
   void vFunction(void* pvParameters)

/****************************************************************************
 *
 ****************************************************************************/
#if configUSE_16_BIT_TICKS == 1
#define portTickType                unsigned short
#else
#define portTickType                unsigned long
#endif

#define portCHAR                    char
#define portFLOAT                   float
#define portDOUBLE                  double
#define portLONG                    long
#define portSHORT                   short
#define portSTACK_TYPE              unsigned long
#define portBASE_TYPE               long

#define portMAX_DELAY               ((portTickType) -1)
#define portTICK_RATE_MS            ((portTickType) 1000 / configTICK_RATE_HZ)
#define portSTACK_GROWTH             -1
#define portBYTE_ALIGNMENT           4
#define portINITIAL_SPSR             0x1F
#define portCRITICAL_NESTING_IN_TCB  1

#ifndef __ASM__
/****************************************************************************
 *
 ****************************************************************************/
static inline unsigned short portBSWAP16(unsigned short value)
{
   value = (value >> 8) | (value << 8);
   return value;
}

/****************************************************************************
 *
 ****************************************************************************/
static inline unsigned long portBSWAP32(unsigned long value)
{
   unsigned long tmp = 0;

   __asm__ __volatile__
   (
      "eor   %0, %3, %3, ror #16 \n"
      "bic   %0, #0x00FF0000     \n"
      "mov   %1, %3, ror #8      \n"
      "eor   %1, %0, lsr #8      \n"
      : "=r" (tmp), "=r" (value) : "0" (tmp), "1" (value)
   );

   return value;
}

/****************************************************************************
 *
 ****************************************************************************/
static inline unsigned long portCPU_FLAGS()
{
   unsigned long cpsr;

   __asm__ __volatile__("mrs %0, CPSR" : "=r" (cpsr));

   return cpsr;
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portNOP()
{
   __asm__ __volatile__("nop");
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portWAIT()
{
   unsigned long junk;

   __asm__ __volatile__("mcr p15, 0, %0, c7, c0, 4" : "=r" (junk));
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portENABLE_INTERRUPTS()
{
   unsigned long cpsr;

   __asm__ __volatile__("mrs %0, CPSR" : "=r" (cpsr));

   __asm__ __volatile__("msr CPSR_c, %0" : : "r" (cpsr & ~0x000000C0));
}

/****************************************************************************
 *
 ****************************************************************************/
static inline int portDISABLE_INTERRUPTS()
{
   unsigned long cpsr;

   __asm__ __volatile__("mrs %0, CPSR" : "=r" (cpsr));

   __asm__ __volatile__("msr CPSR_c, %0" : : "r" (cpsr | 0x000000C0));

   return (cpsr & 0x000000C0) ? 0 : 1;
}  

/****************************************************************************
 *
 ****************************************************************************/
static inline void NORETURN portHALT()
{
   portDISABLE_INTERRUPTS();
   for (;;)
      portWAIT();
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portYIELD()
{
   __asm__ __volatile__("swi 0");
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portENTER_CRITICAL()
{
   extern void vTaskEnterCritical();
   vTaskEnterCritical();
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portEXIT_CRITICAL()
{
   extern void vTaskExitCritical();
   vTaskExitCritical();
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portENABLE_MMU(void* ttb)
{
   __asm__ __volatile__
   (
      "mcr   p15, 0, %0, c2, c0, 0  \n"
      "mov   r12, #0                \n"
      "mcr   p15, 0, r12, c7, c7, 0 \n"
      "mcr   p15, 0, r12, c8, c7, 0 \n"
      "mrc   p15, 0, r12, c1, c0, 0 \n"
      "orr   r12, #0x00001000       \n"
      "orr   r12, #0x00000005       \n"
      "mcr   p15, 0, r12, c1, c0, 0 \n"
      : : "r" (ttb) : "r12", "memory"
   );
}

/****************************************************************************
 *
 ****************************************************************************/
static inline void portDISABLE_MMU()
{
   __asm__ __volatile__
   (
      "mov   r12, #0                \n"
      "mcr   p15, 0, r12, c7, c7, 0 \n"
      "mcr   p15, 0, r12, c8, c7, 0 \n"
      "mrc   p15, 0, r12, c1, c0, 0 \n"
      "bic   r12, #0x00001000       \n"
      "bic   r12, #0x00000005       \n"
      "mcr   p15, 0, r12, c1, c0, 0 \n"
      : : : "r12", "memory"
   );
}

/****************************************************************************
 *
 ****************************************************************************/
int xprintf(const char* format, ...);
 
#endif

#endif
