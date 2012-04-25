#include <htc.h>
#include <peripheral/usart.h>


/********************************************************************
*    Function Name:  Open2USART                                      *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure USART2     *
*                    spbrg: baudrate value for register SPBRG2      *
*    Description:    This routine first resets the USART2 regs       *
*                    to the POR  It then configures the       *
*                    USART2 for interrupts, synch/async, 8/9-bit,    *
*                    sync slave/master and single/          *
*    Notes:          The bit definitions for config can be found    *
*                    in the h                            *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)
void Open2USART( unsigned char config, unsigned int spbrg)
{
  TXSTA2 = 0;           // Reset USART registers to POR state
  RCSTA2 = 0;
 
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
 
  if(config&0x40)      // Interrupt on receipt
    RC2IE = 1;
  else
    RC2IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX2IE = 1;
  else
    TX2IE = 0;

  SPBRG2 = spbrg;       // Write baudrate to SPBRG2

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
	TRISG1 = 0;TRISG2 = 1; 
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISG1 = 1;
}

#elif defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9)

void Open2USART( unsigned char config, unsigned int spbrg)
{
  TXSTA2 = 0;           // Reset USART registers to POR state
  RCSTA2 = 0;
 
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

  if(config&0x20)      // SENDB(asychronous mode only)
    SENDB_x = 1;
  
  if(config&0x40)      // Interrupt on receipt
    RC2IE = 1;
  else
    RC2IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX2IE = 1;
  else
    TX2IE = 0;

  SPBRG2 = spbrg;       // Write baudrate to SPBRG2
  SPBRGH2 = spbrg >> 8; // For 16-bit baud rate generation

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
  TRISG1 = 0;
  TRISG2 = 1; 
  
  if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISG1 = 1;
}

#elif defined (_EAUSART_V11)

void Open2USART( unsigned char config, unsigned int spbrg)
{
  TXSTA2 = 0;           // Reset USART registers to POR state
  RCSTA2 = 0;
 
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

  if(config&0x20)      // SENDB(asychronous mode only)
    SENDB_x = 1;
  
  if(config&0x40)      // Interrupt on receipt
    RC2IE = 1;
  else
    RC2IE = 0;

  if(config&0x80)      // Interrupt on transmission
    TX2IE = 1;
  else
    TX2IE = 0;

  SPBRG2 = spbrg;       // Write baudrate to SPBRG2
  SPBRGH2 = spbrg >> 8; // For 16-bit baud rate generation

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
}


#endif
