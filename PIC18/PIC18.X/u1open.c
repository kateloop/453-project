#include <htc.h>
#include <peripheral/usart.h>

#define RC1IE RCIE_x
#define TX1IE TXIE_x


/********************************************************************
*    Function Name:  Open1USART                                      *
*    Return Value:   void                                            *
*    Parameters:     config: bit definitions to configure USART1     *
*                    spbrg: baudrate value for register SPBRG1       *
*    Description:    This routine first resets the USART1 regs       *
*                    to the POR  It then configures the        *
*                    USART1 for interrupts, synch/async, 8/9-bit,    *
*                    sync slave/master and single/           *
*    Notes:          The bit definitions for config can be found     * 
*                    in the h                             *
********************************************************************/
#if defined (_AUSART_V2) 

void Open1USART( unsigned char config, unsigned int spbrg)
{
  TXSTA1 = 0;          // Reset USART registers to POR state
  RCSTA1 = 0;
 
  if(config&0x01)      // Sync or async operation
    SYNC_x = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TX9_x = 1;	
    RX9_x = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    CSRC_x = 1;

  if(config&0x08)      // Continuous or single reception
    CREN_x = 1;
  else
    SREN_x = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    BRGH_x = 1;
  else
    BRGH_x = 0;

  if(config&0x40)      // Interrupt on receipt
    RC1IE = 1;
  else
    RC1IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX1IE = 1;
  else
    TX1IE = 0;

  SPBRG1 = spbrg;       // Write baudrate to SPBRG
  
  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
	TRISC6 = 0;TRISC7 = 1; 
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISC6 = 1;
}

#elif  defined (_EAUSART_V6) || defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V10)

void Open1USART( unsigned char config, unsigned int spbrg)
{
  TXSTA1 = 0;          // Reset USART registers to POR state
  RCSTA1 = 0;
 
  if(config&0x01)      // Sync or async operation
    SYNC_x = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TX9_x = 1;	
    RX9_x = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    CSRC_x = 1;

  if(config&0x08)      // Continuous or single reception
    CREN_x = 1;
  else
    SREN_x = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    BRGH_x = 1;
  
  if(config&0x20)      // SENDB (asychronous mode only)
    SENDB_x= 1;
  
  if(config&0x40)      // Interrupt on receipt
    RC1IE = 1;
  else
    RC1IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX1IE = 1;
  else
    TX1IE = 0;

  SPBRG1 = spbrg;       // Write baudrate to SPBRG1
  SPBRGH1 = spbrg >> 8; // For 16-bit baud rate generation

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
  TRISC6 = 0;
  TRISC7 = 1; 
  if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISC6 = 1;
}

#elif  defined (_EAUSART_V11)

void Open1USART( unsigned char config, unsigned int spbrg)
{
  TXSTA1 = 0;          // Reset USART registers to POR state
  RCSTA1 = 0;
 
  if(config&0x01)      // Sync or async operation
    SYNC_x = 1;

  if(config&0x02)      // 8- or 9-bit mode
  {
    TX9_x = 1;	
    RX9_x = 1;
  }

  if(config&0x04)      // Master or Slave (sync only)
    CSRC_x = 1;

  if(config&0x08)      // Continuous or single reception
    CREN_x = 1;
  else
    SREN_x = 1;

  if(config&0x10)      // Baud rate select (asychronous mode only)
    BRGH_x = 1;
  
  if(config&0x20)      // SENDB (asychronous mode only)
    SENDB_x= 1;
  
  if(config&0x40)      // Interrupt on receipt
    RC1IE = 1;
  else
    RC1IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX1IE = 1;
  else
    TX1IE = 0;

  SPBRG1 = spbrg;       // Write baudrate to SPBRG1
  SPBRGH1 = spbrg >> 8; // For 16-bit baud rate generation

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver  
}
#endif
