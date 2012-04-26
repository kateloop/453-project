#include "ssiAddr.h"
#include <stdio.h>
//////////////////////////////////////////////
//          defines                         //
//////////////////////////////////////////////

#define SACNT_VAL 0x00000011	// 48kHz fixed mode
#define SRCCR_VAL 0x00007000	// TODO check value - do we even need to set? no; we don't care about receive
#define SCR_VAL   0x00000013	// TODO check value - do we even need to set? no: bits 3, 4 automatically set and we set the rest w/ masks
#define SFCSR_VAL 0x00000800    // 8 data words in TxFIFO0

#define SSIRST_ANDMASK 0xFFFE		 // SSI Reset
#define SSIEN_ORMASK   0x0001	         // SSI Enable
#define SSITEN_ORMASK  0x0002		 // Transmit Enable
#define WL_ANDMASK     0xFFFF9FFF        // Transmit 20 bits of data
#define DC_ANDMASK     0xFFFFF0FF        // DC == 1 //TODO ?????
#define WRCMD_ORMASK   0x0010		 // OR SACNT Reg to transmit
#define FVOP_ANDMASK   0x0010		 // AND with SACNT Reg to set up fixed variable mode

// pg 596 -- VERY IMPORTANT

void main (int argc, char** argv) {
	// NOTE: We are ONLY writing data for transmission

	// Init SSI
	// Issue SSI Reset (SSIEN bit in SCR is cleared)
	SCR &= SSIRST_ANDMASK;
	// Set SSIEN bit in SCR
	SCR |= SSIEN_ORMASK;
	// Set all other control bits (ie TXDIR, RXDIR, etc)-referring to STCR/SRCR; already set as desired by hardware
	
	// Set TE bits in SCR to enable transmission
	SCR |= SSITEN_ORMASK;


	STCCR &= WL_ANDMASK;
	// Select # time slots in DC bits - STCCR (Words/frame)
        STCCR &= DC_ANDMASK;	
	// Write Data to be transmitted (Tx Data Reg 0 (FIFO))
	// TODO
	
	// FV, TIF, RD, WR, FRDIV in SACNT
	SACNT &= FVOP_ANDMASK;
	
	// Update SACADD, SACDAT, SATAG - I don't think we need to worry about these...they might be for interrupts
	// SACADD - command address
	// SACDAT - command data 
	// SATAG - 	AC97 Tag Register
	
	// Enable AC97 mode AC97EN in SACNT
	SACNT |= SSIEN_ORMASK;

   printf("ending; bits set\n");

}























