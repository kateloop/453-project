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
#ifndef IO_DEFINE_H
#define IO_DEFINE_H

/****************************************************************************
 *
 ****************************************************************************/
#define TCTL1        (*(volatile unsigned long*) 0x10003000)
#define TPRER1       (*(volatile unsigned long*) 0x10003004)
#define TCMP1        (*(volatile unsigned long*) 0x10003008)
#define TCR1         (*(volatile unsigned long*) 0x1000300C)
#define TCN1         (*(volatile unsigned long*) 0x10003010)
#define TSTAT1       (*(volatile unsigned long*) 0x10003014)
#define TCTL2        (*(volatile unsigned long*) 0x10004000)
#define TPRER2       (*(volatile unsigned long*) 0x10004004)
#define TCMP2        (*(volatile unsigned long*) 0x10004008)
#define TCR2         (*(volatile unsigned long*) 0x1000400C)
#define TCN2         (*(volatile unsigned long*) 0x10004010)
#define TSTAT2       (*(volatile unsigned long*) 0x10004014)
#define TCTL3        (*(volatile unsigned long*) 0x10005000)
#define TPRER3       (*(volatile unsigned long*) 0x10005004)
#define TCMP3        (*(volatile unsigned long*) 0x10005008)
#define TCR3         (*(volatile unsigned long*) 0x1000500C)
#define TCN3         (*(volatile unsigned long*) 0x10005010)
#define TSTAT3       (*(volatile unsigned long*) 0x10005014)

/****************************************************************************
 *
 ****************************************************************************/
#define URXD_1       (*(volatile unsigned long*) 0x1000A000)
#define UTXD_1       (*(volatile unsigned long*) 0x1000A040)
#define UCR1_1       (*(volatile unsigned long*) 0x1000A080)
#define UCR2_1       (*(volatile unsigned long*) 0x1000A084)
#define UCR3_1       (*(volatile unsigned long*) 0x1000A088)
#define UCR4_1       (*(volatile unsigned long*) 0x1000A08C)
#define UFCR_1       (*(volatile unsigned long*) 0x1000A090)
#define USR1_1       (*(volatile unsigned long*) 0x1000A094)
#define USR2_1       (*(volatile unsigned long*) 0x1000A098)
#define UESC_1       (*(volatile unsigned long*) 0x1000A09C)
#define UTIM_1       (*(volatile unsigned long*) 0x1000A0A0)
#define UBIR_1       (*(volatile unsigned long*) 0x1000A0A4)
#define UBMR_1       (*(volatile unsigned long*) 0x1000A0A8)
#define UBRC_1       (*(volatile unsigned long*) 0x1000A0AC)
#define ONEMS_1      (*(volatile unsigned long*) 0x1000A0B0)
#define UTS_1        (*(volatile unsigned long*) 0x1000A0B4)
#define URXD_2       (*(volatile unsigned long*) 0x1000B000)
#define UTXD_2       (*(volatile unsigned long*) 0x1000B040)
#define UCR1_2       (*(volatile unsigned long*) 0x1000B080)
#define UCR2_2       (*(volatile unsigned long*) 0x1000B084)
#define UCR3_2       (*(volatile unsigned long*) 0x1000B088)
#define UCR4_2       (*(volatile unsigned long*) 0x1000B08C)
#define UFCR_2       (*(volatile unsigned long*) 0x1000B090)
#define USR1_2       (*(volatile unsigned long*) 0x1000B094)
#define USR2_2       (*(volatile unsigned long*) 0x1000B098)
#define UESC_2       (*(volatile unsigned long*) 0x1000B09C)
#define UTIM_2       (*(volatile unsigned long*) 0x1000B0A0)
#define UBIR_2       (*(volatile unsigned long*) 0x1000B0A4)
#define UBMR_2       (*(volatile unsigned long*) 0x1000B0A8)
#define UBRC_2       (*(volatile unsigned long*) 0x1000B0AC)
#define ONEMS_2      (*(volatile unsigned long*) 0x1000B0B0)
#define UTS_2        (*(volatile unsigned long*) 0x1000B0B4)

/****************************************************************************
 *
 ****************************************************************************/
#define PTA_DDIR     (*(volatile unsigned long*) 0x10015000)
#define PTB_DDIR     (*(volatile unsigned long*) 0x10015100)
#define PTC_DDIR     (*(volatile unsigned long*) 0x10015200)
#define PTD_DDIR     (*(volatile unsigned long*) 0x10015300)
#define PTE_DDIR     (*(volatile unsigned long*) 0x10015400)
#define PTF_DDIR     (*(volatile unsigned long*) 0x10015500)
#define PTA_OCR1     (*(volatile unsigned long*) 0x10015004)
#define PTB_OCR1     (*(volatile unsigned long*) 0x10015104)
#define PTC_OCR1     (*(volatile unsigned long*) 0x10015204)
#define PTD_OCR1     (*(volatile unsigned long*) 0x10015304)
#define PTE_OCR1     (*(volatile unsigned long*) 0x10015404)
#define PTF_OCR1     (*(volatile unsigned long*) 0x10015504)
#define PTA_OCR2     (*(volatile unsigned long*) 0x10015008)
#define PTB_OCR2     (*(volatile unsigned long*) 0x10015108)
#define PTC_OCR2     (*(volatile unsigned long*) 0x10015208)
#define PTD_OCR2     (*(volatile unsigned long*) 0x10015308)
#define PTE_OCR2     (*(volatile unsigned long*) 0x10015408)
#define PTF_OCR2     (*(volatile unsigned long*) 0x10015508)
#define PTA_ICONFA1  (*(volatile unsigned long*) 0x1001500C)
#define PTB_ICONFA1  (*(volatile unsigned long*) 0x1001510C)
#define PTC_ICONFA1  (*(volatile unsigned long*) 0x1001520C)
#define PTD_ICONFA1  (*(volatile unsigned long*) 0x1001530C)
#define PTE_ICONFA1  (*(volatile unsigned long*) 0x1001540C)
#define PTF_ICONFA1  (*(volatile unsigned long*) 0x1001550C)
#define PTA_ICONFA2  (*(volatile unsigned long*) 0x10015010)
#define PTB_ICONFA2  (*(volatile unsigned long*) 0x10015110)
#define PTC_ICONFA2  (*(volatile unsigned long*) 0x10015210)
#define PTD_ICONFA2  (*(volatile unsigned long*) 0x10015310)
#define PTE_ICONFA2  (*(volatile unsigned long*) 0x10015410)
#define PTF_ICONFA2  (*(volatile unsigned long*) 0x10015510)
#define PTA_ICONFB1  (*(volatile unsigned long*) 0x10015014)
#define PTB_ICONFB1  (*(volatile unsigned long*) 0x10015114)
#define PTC_ICONFB1  (*(volatile unsigned long*) 0x10015214)
#define PTD_ICONFB1  (*(volatile unsigned long*) 0x10015314)
#define PTE_ICONFB1  (*(volatile unsigned long*) 0x10015414)
#define PTF_ICONFB1  (*(volatile unsigned long*) 0x10015514)
#define PTA_ICONFB2  (*(volatile unsigned long*) 0x10015018)
#define PTB_ICONFB2  (*(volatile unsigned long*) 0x10015118)
#define PTC_ICONFB2  (*(volatile unsigned long*) 0x10015218)
#define PTD_ICONFB2  (*(volatile unsigned long*) 0x10015318)
#define PTE_ICONFB2  (*(volatile unsigned long*) 0x10015418)
#define PTF_ICONFB2  (*(volatile unsigned long*) 0x10015518)
#define PTA_DR       (*(volatile unsigned long*) 0x1001501C)
#define PTB_DR       (*(volatile unsigned long*) 0x1001511C)
#define PTC_DR       (*(volatile unsigned long*) 0x1001521C)
#define PTD_DR       (*(volatile unsigned long*) 0x1001531C)
#define PTE_DR       (*(volatile unsigned long*) 0x1001541C)
#define PTF_DR       (*(volatile unsigned long*) 0x1001551C)
#define PTA_GIUS     (*(volatile unsigned long*) 0x10015020)
#define PTB_GIUS     (*(volatile unsigned long*) 0x10015120)
#define PTC_GIUS     (*(volatile unsigned long*) 0x10015220)
#define PTD_GIUS     (*(volatile unsigned long*) 0x10015320)
#define PTE_GIUS     (*(volatile unsigned long*) 0x10015420)
#define PTF_GIUS     (*(volatile unsigned long*) 0x10015520)
#define PTA_SSR      (*(volatile unsigned long*) 0x10015024)
#define PTB_SSR      (*(volatile unsigned long*) 0x10015124)
#define PTC_SSR      (*(volatile unsigned long*) 0x10015224)
#define PTD_SSR      (*(volatile unsigned long*) 0x10015324)
#define PTE_SSR      (*(volatile unsigned long*) 0x10015424)
#define PTF_SSR      (*(volatile unsigned long*) 0x10015524)
#define PTA_ICR1     (*(volatile unsigned long*) 0x10015028)
#define PTB_ICR1     (*(volatile unsigned long*) 0x10015128)
#define PTC_ICR1     (*(volatile unsigned long*) 0x10015228)
#define PTD_ICR1     (*(volatile unsigned long*) 0x10015328)
#define PTE_ICR1     (*(volatile unsigned long*) 0x10015428)
#define PTF_ICR1     (*(volatile unsigned long*) 0x10015528)
#define PTA_ICR2     (*(volatile unsigned long*) 0x1001502C)
#define PTB_ICR2     (*(volatile unsigned long*) 0x1001512C)
#define PTC_ICR2     (*(volatile unsigned long*) 0x1001522C)
#define PTD_ICR2     (*(volatile unsigned long*) 0x1001532C)
#define PTE_ICR2     (*(volatile unsigned long*) 0x1001542C)
#define PTF_ICR2     (*(volatile unsigned long*) 0x1001552C)
#define PTA_IMR      (*(volatile unsigned long*) 0x10015030)
#define PTB_IMR      (*(volatile unsigned long*) 0x10015130)
#define PTC_IMR      (*(volatile unsigned long*) 0x10015230)
#define PTD_IMR      (*(volatile unsigned long*) 0x10015330)
#define PTE_IMR      (*(volatile unsigned long*) 0x10015430)
#define PTF_IMR      (*(volatile unsigned long*) 0x10015530)
#define PTA_ISR      (*(volatile unsigned long*) 0x10015034)
#define PTB_ISR      (*(volatile unsigned long*) 0x10015134)
#define PTC_ISR      (*(volatile unsigned long*) 0x10015234)
#define PTD_ISR      (*(volatile unsigned long*) 0x10015334)
#define PTE_ISR      (*(volatile unsigned long*) 0x10015434)
#define PTF_ISR      (*(volatile unsigned long*) 0x10015534)
#define PTA_GPR      (*(volatile unsigned long*) 0x10015038)
#define PTB_GPR      (*(volatile unsigned long*) 0x10015138)
#define PTC_GPR      (*(volatile unsigned long*) 0x10015238)
#define PTD_GPR      (*(volatile unsigned long*) 0x10015338)
#define PTE_GPR      (*(volatile unsigned long*) 0x10015438)
#define PTF_GPR      (*(volatile unsigned long*) 0x10015538)
#define PTA_SWR      (*(volatile unsigned long*) 0x1001503C)
#define PTB_SWR      (*(volatile unsigned long*) 0x1001513C)
#define PTC_SWR      (*(volatile unsigned long*) 0x1001523C)
#define PTD_SWR      (*(volatile unsigned long*) 0x1001533C)
#define PTE_SWR      (*(volatile unsigned long*) 0x1001543C)
#define PTF_SWR      (*(volatile unsigned long*) 0x1001553C)
#define PTA_PUEN     (*(volatile unsigned long*) 0x10015040)
#define PTB_PUEN     (*(volatile unsigned long*) 0x10015140)
#define PTC_PUEN     (*(volatile unsigned long*) 0x10015240)
#define PTD_PUEN     (*(volatile unsigned long*) 0x10015340)
#define PTE_PUEN     (*(volatile unsigned long*) 0x10015440)
#define PTF_PUEN     (*(volatile unsigned long*) 0x10015540)
#define PMASK        (*(volatile unsigned long*) 0x10015600)

/****************************************************************************
 *
 ****************************************************************************/
#define INTCNTL      (*(volatile unsigned long*) 0x10040000)
#define NIMASK       (*(volatile unsigned long*) 0x10040004)
#define INTENNUM     (*(volatile unsigned long*) 0x10040008)
#define INTDISNUM    (*(volatile unsigned long*) 0x1004000C)
#define INTENABLEH   (*(volatile unsigned long*) 0x10040010)
#define INTENABLEL   (*(volatile unsigned long*) 0x10040014)
#define INTTYPEH     (*(volatile unsigned long*) 0x10040018)
#define INTTYPEL     (*(volatile unsigned long*) 0x1004001C)
#define NIPRIORITY7  (*(volatile unsigned long*) 0x10040020)
#define NIPRIORITY6  (*(volatile unsigned long*) 0x10040024)
#define NIPRIORITY5  (*(volatile unsigned long*) 0x10040028)
#define NIPRIORITY4  (*(volatile unsigned long*) 0x1004002C)
#define NIPRIORITY3  (*(volatile unsigned long*) 0x10040030)
#define NIPRIORITY2  (*(volatile unsigned long*) 0x10040034)
#define NIPRIORITY1  (*(volatile unsigned long*) 0x10040038)
#define NIPRIORITY0  (*(volatile unsigned long*) 0x1004003C)
#define NIVECSR      (*(volatile unsigned long*) 0x10040040)
#define FIVECSR      (*(volatile unsigned long*) 0x10040044)
#define INTSRCH      (*(volatile unsigned long*) 0x10040048)
#define INTSRCL      (*(volatile unsigned long*) 0x1004004C)
#define INTFRCH      (*(volatile unsigned long*) 0x10040050)
#define INTFRCL      (*(volatile unsigned long*) 0x10040054)
#define NIPNDH       (*(volatile unsigned long*) 0x10040058)
#define NIPNDL       (*(volatile unsigned long*) 0x1004005C)
#define FIPNDH       (*(volatile unsigned long*) 0x10040060)
#define FIPNDL       (*(volatile unsigned long*) 0x10040064)

/****************************************************************************
 *
 ****************************************************************************/
#define CSCR         (*(volatile unsigned long*) 0x10027000)
#define MPCTL0       (*(volatile unsigned long*) 0x10027004)
#define MPCTL1       (*(volatile unsigned long*) 0x10027008)
#define SPCTL0       (*(volatile unsigned long*) 0x1002700C)
#define SPCTL1       (*(volatile unsigned long*) 0x10027010)
#define OSC26MCTL    (*(volatile unsigned long*) 0x10027014)
#define PCDR0        (*(volatile unsigned long*) 0x10027018)
#define PCDR1        (*(volatile unsigned long*) 0x1002701C)
#define PCCR0        (*(volatile unsigned long*) 0x10027020)
#define PCCR1        (*(volatile unsigned long*) 0x10027024)
#define CCSR         (*(volatile unsigned long*) 0x10027028)
#define PMCTL        (*(volatile unsigned long*) 0x1002702C)
#define PMCOUNT      (*(volatile unsigned long*) 0x10027030)
#define WKGDCTL      (*(volatile unsigned long*) 0x10027034)

#endif
