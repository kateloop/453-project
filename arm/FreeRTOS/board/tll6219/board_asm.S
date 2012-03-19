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
#include "FreeRTOSConfig.h"

   .file "board_asm.S"

/****************************************************************************
 *
 ****************************************************************************/
#define printf xprintf

/****************************************************************************
 *
 ****************************************************************************/
#define USER_MODE        0x10
#define FIQ_MODE         0x11
#define IRQ_MODE         0x12
#define SUPERVISOR_MODE  0x13
#define ABORT_MODE       0x17
#define UNDEFINED_MODE   0x1B
#define SYSTEM_MODE      0x1F
#define T_BIT            0x20
#define F_BIT            0x40
#define I_BIT            0x80

/****************************************************************************
 *
 ****************************************************************************/
#define INTCNTL  0x10040000
#define NIVECSR  0x10040040
#define FIVECSR  0x10040044

/****************************************************************************
 *
 ****************************************************************************/
   .bss
   .align 2
   .skip 5 * configIRQ_STACK_SIZE
stack:

/****************************************************************************
 * Note!: On error conditions most of the interrupt/exception handlers will
 *        call printf().  You should really never call printf() from an 
 *        interrupt handler (it calls malloc(), which is the problem).
 ****************************************************************************/
   .data
   .align 12
   .globl vectorTable
vectorTable:
   nop
   ldr   pc, vector1
   ldr   pc, vector2
   ldr   pc, vector3
   ldr   pc, vector4
   nop
   ldr   pc, vector5
   ldr   pc, vector6
vector1:
   .word undefinedISR
vector2:
   .word swiISR
vector3:
   .word prefetchAbortISR
vector4:
   .word dataAbortISR
vector5:
   .word irqISR
vector6:
   .word fiqISR
   .skip 4096 - 14 * 4

/****************************************************************************
 *
 ****************************************************************************/
   .section .rodata
undefinedError:
   .asciz "undefined instruction exception\n"
prefetchAbortError:
   .asciz "prefetch abort exception\n"
dataAbortError:
   .asciz "data abort exception\n"
swiError:
   .asciz "unhandled SWI: %d\n"
irqError:
   .asciz "unhandled IRQ: %d\n"
fiqError:
   .asciz "unhandled FIQ: %d\n"

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
undefinedISR:
   ldr   r0, =undefinedError
   bl    printf
U1:
   b     U1

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
prefetchAbortISR:
   ldr   r0, =prefetchAbortError
   bl    printf
P1:
   b     P1

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
dataAbortISR:
   ldr   r0, =dataAbortError
   bl    printf
D1:
   b     D1

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
swiISR:
   /* For SWIs lr is not offset 4 like it is in IRQs, so make it */
   add   lr, #4
#include "portSAVE_CONTEXT.S"
   ldr   r1, [lr, #-8]
   bic   r1, #0xFF000000
   tst   r1, r1
   bne   S1
   bl    vTaskSwitchContext
#include "portRESTORE_CONTEXT.S"
   subs  pc, lr, #4
S1:
   ldr   r0, =swiError
   bl    printf
S2:
   b     S2

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
fiqISR:
   ldr   r0, =fiqError
   ldr   r1, =FIVECSR
   ldr   r1, [r1]
   lsr   r1, #16
   bl    printf
F1:
   b     F1

/****************************************************************************
 *
 ****************************************************************************/
   .text
   .align 2
irqISR:
#include "portSAVE_CONTEXT.S"
   ldr   r1, =NIVECSR
   ldr   r1, [r1]
   lsr   r1, #16
   ldr   r2, =INTCNTL
   ldr   r2, [r2]
   tst   r2, #0x00010000
   ldr   r3, =0xFFFFF003
   bic   r2, r3
   ldreq r2, =0xFFFFFF00
   lsl   r1, #2
   ldr   r3, [r2, r1]
   tst   r3, r3
   beq   I1
   blx   r3
#include "portRESTORE_CONTEXT.S"
   subs  pc, lr, #4
I1:
   ldr   r0, =irqError
   lsr   r1, #2
   bl    printf
I2:
   b     I2

/****************************************************************************
 *
 ****************************************************************************/
   .section .init
   .align 2
   .globl _start
_start:
   push  {lr}

   mov   r1, sp
   ldr   r0, =stack

   msr   CPSR_c, #(I_BIT | F_BIT | UNDEFINED_MODE)
   mov   sp, r0
   sub   r0, #configIRQ_STACK_SIZE

   msr   CPSR_c, #(I_BIT | F_BIT | ABORT_MODE)
   mov   sp, r0
   sub   r0, #configIRQ_STACK_SIZE

   msr   CPSR_c, #(I_BIT | F_BIT | FIQ_MODE)
   mov   sp, r0
   sub   r0, #configIRQ_STACK_SIZE

   msr   CPSR_c, #(I_BIT | F_BIT | IRQ_MODE)
   mov   sp, r0
   sub   r0, #configIRQ_STACK_SIZE

   msr   CPSR_c, #(I_BIT | F_BIT | SUPERVISOR_MODE)
   mov   sp, r0

   push  {r1}

   bl    boardInit0

   pop   {r1}
   mov   sp, r1

   pop   {pc}
