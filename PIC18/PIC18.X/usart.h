#include <htc.h>
#ifndef __USART_H
#define __USART_H
/* PIC18 USART peripheral libraries. */

/* There are three library modules, corresponding to register names:
 *  USART1  (TXSTA1, RCSTA1, etc.)
 *  USART2  (TXSTA2, RCSTA2, etc.)
 *  USART  (TXSTA, RCSTA, etc.)
 *  Each module is defined only for those devices for which the register
 *  names are defined.
 */

 /* Corresponding to each module, there are several routines: 
  *
  * The 'open' routine takes two parameters:
  *   - 'config' is the bitwise 'and' of the appropriate configuration
  *     bit masks (defined below);
  *   - 'spbrg' is the baud rate.
  * The registers associated with the USART module are set according to these
  * parameters; then, the transmitter and receiver are enabled.
  *
  * The 'datardy' routine returns 1 if data has been received, 0 otherwise.
  *
  * The 'read' routine returns the received byte.  It also sets the framing
  * and overrun error status bits (FRAME_ERROR & OVERRUN_ERROR) if necessary; 
  * also, the status receive bit 8 (RX_NINE) is significant if 9-bit mode 
  * is enabled.
  * (See status bit structure definition below).
  *
  * The 'write' routine accepts the byte to transmit.  If 9-bit mode is 
  * enabled, the status trasmit bit 8 (TX_NINE) is also trasmitted.
  *
  * The 'gets' routine accepts a buffer and the buffer length in bytes as
  * parameters.  It fills the buffer with bytes as they are received; it will
  * wait for data if necessary in order to fill the entire buffer.
  *
  * The 'puts' routine accepts a null-terminated byte string.  All bytes
  * are transmitted, including the null character.  It will wait until the
  * USART is not busy in order to transmit all the bytes.
  *
  * The 'putrs' routine is identical to 'puts', except the byte string
  * resides in ROM.
  *
  * The 'close' routine disables the receiver, the transmitter, and the 
  * interrupts for both.
  *
  * The 'busy' routine returns 1 if the transmit shift register is not empty;
  * otherwise, it returns 0.
  *
  * For devices with enhanced USART capability, an additional 'baud'
  * routine is provided.  This routine takes a 'config' parameter, which
  * is a bitwise 'and' of the baud configuration bit masks (see below).
  * The BAUDCON (a.k.a. BAUDCTL) register is configured appropriately.
  */

/* Configuration bit masks to be 'anded' together and passed as the 'config'
 * parameter to the 'open' routine. */
//-----------AND OR MASK-------------------------------------------------
#ifdef USE_OR_MASKS
#define USART_TX_INT_ON   		0b10000000  // Transmit interrupt on
#define USART_TX_INT_OFF  		0b00000000  // Transmit interrupt off
#define USART_TX_INT_MASK		(~USART_TX_INT_ON)

#define USART_RX_INT_ON   		0b01000000  // Receive interrupt on
#define USART_RX_INT_OFF  		0b00000000  // Receive interrupt off
#define USART_RX_INT_MASK		(~USART_RX_INT_ON)

#define USART_ADDEN_ON    		0b00100000  // 
#define USART_ADDEN_OFF   		0b00000000  // 
#define USART_ADDEN_MASK		(~USART_ADDEN_ON)

#define USART_BRGH_HIGH   		0b00010000  // High baud rate
#define USART_BRGH_LOW    		0b00000000  // Low baud rate
#define USART_BRGH_MASK			(~USART_BRGH_HIGH)

#define USART_CONT_RX     		0b00001000  // Continuous reception
#define USART_SINGLE_RX   		0b00000000  // Single reception
#define USART_CONT_RX_MASK		(~USART_CONT_RX) 

#define USART_SYNC_MASTER 		0b00000100  // Synchrounous master mode
#define USART_SYNC_SLAVE  		0b00000000  // Synchrounous slave mode
#define USART_SYNC_MASK			(~USART_SYNC_MASTER)

#define USART_NINE_BIT    		0b00000010  // 9-bit data
#define USART_EIGHT_BIT   		0b00000000  // 8-bit data
#define USART_BIT_MASK 			(~USART_NINE_BIT)

#define USART_SYNCH_MODE  		0b00000001  // Synchronous mode
#define USART_ASYNCH_MODE 		0b00000000  // Asynchronous mode
#define USART_MODE_MASK			(~USART_SYNCH_MODE) 
//------------AND MASK------------------------------------------------
#else
#define USART_TX_INT_ON   0b11111111  // Transmit interrupt on
#define USART_TX_INT_OFF  0b01111111  // Transmit interrupt off
#define USART_RX_INT_ON   0b11111111  // Receive interrupt on
#define USART_RX_INT_OFF  0b10111111  // Receive interrupt off
#define USART_BRGH_HIGH   0b11111111  // High baud rate
#define USART_BRGH_LOW    0b11101111  // Low baud rate
#define USART_CONT_RX     0b11111111  // Continuous reception
#define USART_SINGLE_RX   0b11110111  // Single reception
#define USART_SYNC_MASTER 0b11111111  // Synchrounous master mode
#define USART_SYNC_SLAVE  0b11111011  // Synchrounous slave mode
#define USART_NINE_BIT    0b11111111  // 9-bit data
#define USART_EIGHT_BIT   0b11111101  // 8-bit data
#define USART_SYNCH_MODE  0b11111111  // Synchronous mode
#define USART_ASYNCH_MODE 0b11111110  // Asynchronous mode
#define USART_ADDEN_ON    0b11111111  // 
#define USART_ADDEN_OFF   0b11011111  // 

#endif

/* These devices have enhanced USARTs. */
#if defined (_EAUSART_V3 ) ||defined (_EAUSART_V4 ) ||defined (_EAUSART_V5 ) ||defined (_EAUSART_V6 ) ||defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 ) ||defined (_EAUSART_V10 ) || defined (_EAUSART_V11)

/* The baud configuration bit masks to be 'anded' together and passed to
 * the 'baud' routine. */
//--------------AND OR MASK------------------------------
#ifdef USE_OR_MASKS
#define BAUD_IDLE_CLK_HIGH  0b00010000  // idle state for clock is a high level
#define BAUD_IDLE_CLK_LOW   0b00000000  // idle state for clock is a low level
#define BAUD_IDLE_CLK_MASK	(~BAUD_IDLE_CLK_HIGH)

#define BAUD_16_BIT_RATE    0b00001000  // 16-bit baud generation rate
#define BAUD_8_BIT_RATE     0b00000000  // 8-bit baud generation rate
#define BAUD_BIT_RATE       (~BAUD_16_BIT_RATE)

#define BAUD_WAKEUP_ON      0b00000010  // RX pin monitored
#define BAUD_WAKEUP_OFF     0b00000000  // RX pin not monitored
#define BAUD_WAKEUP_MASK	(~BAUD_WAKEUP_ON)

#define BAUD_AUTO_ON        0b00000001  // auto baud rate measurement enabled
#define BAUD_AUTO_OFF       0b00000000  // auto baud rate measurement disabled
#define BAUD_AUTO_MASK		(~BAUD_AUTO_ON)
//-----------------AND MASK-------------------------------
#else 
#define BAUD_IDLE_CLK_HIGH  0b11111111  // idle state for clock is a high level
#define BAUD_IDLE_CLK_LOW   0b11101111  // idle state for clock is a low level
#define BAUD_16_BIT_RATE    0b11111111  // 16-bit baud generation rate
#define BAUD_8_BIT_RATE     0b11110111  // 8-bit baud generation rate
#define BAUD_WAKEUP_ON      0b11111111  // RX pin monitored
#define BAUD_WAKEUP_OFF     0b11111101  // RX pin not monitored
#define BAUD_AUTO_ON        0b11111111  // auto baud rate measurement enabled
#define BAUD_AUTO_OFF       0b11111110  // auto baud rate measurement disabled
#endif

#endif


/* Only these devices have two USART modules: USART1 & USART2. */
#if defined (_AUSART_V2 ) ||defined (_EAUSART_V6 ) ||defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 )  || defined (_EAUSART_V10) || defined (_EAUSART_V11)

/* ***** USART1 ***** */

/* status bits */
union USART1
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};
extern union USART1 USART1_Status;

void Open1USART (unsigned char config, unsigned int spbrg);

#define DataRdy1USART( ) (RC1IF)

char Read1USART (void);
void Write1USART (char data);
void gets1USART (char *buffer, unsigned char len);
void puts1USART (char *data);
#define putrs1USART puts1USART
#define getc1USART Read1USART
#define putc1USART Write1USART
#define Close1USART( ) RCSTA1&=0x4f,TXEN_x=0,PIE1&=0xcf
#define Busy1USART( )  (!TRMT_x)
#endif

/* ***** USART2 ***** */
#if defined (_AUSART_V2 ) ||defined (_EAUSART_V6 ) ||defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 ) || defined (_EAUSART_V11)
/* status bits */
union USART2
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};
extern union USART2 USART2_Status;
void Open2USART (unsigned char config, unsigned int spbrg);
#define DataRdy2USART( ) (RC2IF)
char Read2USART (void);
void Write2USART (char data);
void gets2USART (char *buffer, unsigned char len);
void puts2USART (char *data);
#define putrs2USART puts2USART
#define getc2USART Read2USART
#define putc2USART Write2USART
#define Close2USART( ) RCSTA2&=0x4f,TXEN_x=0,PIE3&=0xcf
#define Busy2USART( ) (!TRMT_x)

#endif



#if defined (_AUSART_V1 ) || defined (_EAUSART_V3 ) || defined (_EAUSART_V4 ) || defined (_EAUSART_V5 )
/* ***** USART (TXSTA, RCSTA, etc.) ***** */
/* status bits */
union USART
{
  unsigned char val;
  struct
  {
    unsigned RX_NINE:1;         // Receive Bit 8 if 9-bit mode is enabled
    unsigned TX_NINE:1;         // Transmit Bit 8 if 9-bit mode is enabled
    unsigned FRAME_ERROR:1;     // Framing Error for USART
    unsigned OVERRUN_ERROR:1;   // Overrun Error for USART
    unsigned fill:4;
  };
};
extern union USART USART_Status;
void OpenUSART (unsigned char config, unsigned spbrg);
#define DataRdyUSART( ) (RCIF)
char ReadUSART (void);
void WriteUSART (char data);
void getsUSART (char *buffer, unsigned char len);
void putsUSART (char *data);
#define putrsUSART putsUSART
#define getcUSART ReadUSART
#define putcUSART WriteUSART
#define CloseUSART( ) RCSTA&=0x4f,TXEN_x=0,PIE1&=0xcf
#define BusyUSART( ) (!TRMT_x)

#endif

#if defined (_EAUSART_V4 ) ||defined (_EAUSART_V3 ) ||defined (_EAUSART_V5 )
#define baudUSART(x) (BAUDCON= (x) )
#endif


#if defined (_EAUSART_V6 )  || defined (_EAUSART_V10)
#define baud1USART(x) (BAUDCON1= (x) )
#endif

#if defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 ) || defined (_EAUSART_V11)
#define baud1USART(x) (BAUDCON1= (x) )
#define baud2USART(x) (BAUDCON2= (x) )
#endif

/*Macros for backward compatibility*/
#if !defined (USE_OR_MASKS)

#define BAUD_IDLE_RX_PIN_STATE_HIGH    0b11011111  // idle state for RX pin is high level
#define BAUD_IDLE_RX_PIN_STATE_LOW     0b11111111  // idle state for RX pin is low level
#define BAUD_IDLE_TX_PIN_STATE_HIGH    0b11101111  // idle state for TX pin is high level
#define BAUD_IDLE_TX_PIN_STATE_LOW     0b11111111  // idle state for TX pin is low level

#endif

//-----------usart pins mux-------------


#if defined(__18F1220) || defined(__18F1320) 

#define _USART_IO_V1

#elif defined(__18F1230) || defined(__18F1330) 

#define _USART_IO_V2

#endif

#if defined(__18F1220) || defined(__18F1320)||\
	defined(__18F13K50) || defined(__18F14K50)||\
	defined(__18LF13K50) || defined(__18LF14K50)
#define BAUDCON BAUDCTL
#endif

#endif
