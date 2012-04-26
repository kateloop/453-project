///////////////////////////////////////////////
//         SSI Register Addresses            //
///////////////////////////////////////////////

#ifndef SSIADDR_H
#define SSIADDR_H

/* ssi1 Addresses */
#define STX0   *(volatile unsigned long*) 0x10010000  // SSI Transmit Data Reg 0
#define STX1   *(volatile unsigned long*) 0x10010004  // SSI Transmit Data Reg 1
#define SRX0   *(volatile unsigned long*) 0x10010008  // SSI Receive Data Reg 0
#define SRX1   *(volatile unsigned long*) 0x1001000c  // SSI Receive Data Reg 1
#define SCR    *(volatile unsigned long*) 0x10010010  // SSI Control Reg
#define SISR   *(volatile unsigned long*) 0x10010014  // SSI Interrupt Status Reg; read-only	
#define SIER   *(volatile unsigned long*) 0x10010018  // SSI Interrupt Enable Reg
#define STCR   *(volatile unsigned long*) 0x1001001c  // SSI Transmit Config Reg
#define SRCR   *(volatile unsigned long*) 0x10010020  // SSI Receive Config Reg
#define STCCR  *(volatile unsigned long*) 0x10010024  // SSI Transmit Clk Control Reg
#define SRCCR  *(volatile unsigned long*) 0x10010028  // SSI Receive Clk Control Reg
#define SFCSR  *(volatile unsigned long*) 0x1001002c  // SSI FIFO Control/Status Reg
#define STR    *(volatile unsigned long*) 0x10010030  // SSI Test Reg
#define SOR    *(volatile unsigned long*) 0x10010034  // SSI Option Reg
#define SACNT  *(volatile unsigned long*) 0x10010038  // SSI AC97 Control Reg
#define SACADD *(volatile unsigned long*) 0x1001003c  // SSI AC97 Command Addr Reg
#define SACDAT *(volatile unsigned long*) 0x10010040  // SSI AC97 Command Data Reg
#define SATAG  *(volatile unsigned long*) 0x10010044  // SSI AC97 Tag Reg
#define STMSK  *(volatile unsigned long*) 0x10010048  // SSI Transmit Time Slot Mask Reg
#define SRMSK  *(volatile unsigned long*) 0x1001004c  // SSI Receive Time Slot Mask Reg

#endif



























         
