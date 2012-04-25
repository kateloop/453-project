#include <htc.h>
#include <peripheral/usart.h>


/********************************************************************
*    Function Name:  OpenUSART                                      *
*    Return Value:   void                                           *
*    Parameters:     config: bit definitions to configure USART     *
*                    spbrg: baudrate value for register SPBRG1      *
*                    or for SPBRGH:SPBRG for 16-bit baud rate       *
*                    generation for applicable parts                *
*    Description:    This routine first resets the USART regs       *
*                    to the POR  It then configures the       *
*                    USART for interrupts, synch/async, 8/9-bit,    *
*                    sync slave/master and single/          *
*    Notes:          The bit definitions for config can be found    *
*                    in the h                            *
********************************************************************/
#if defined (_AUSART_V1)

void OpenUSART( unsigned char config, unsigned int spbrg)
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;
 
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

  TXIF = 0;

  if(config&0x40)      // Interrupt on receipt
    RCIE_x = 1;
  else
    RCIE_x = 0;

  RCIF = 0;

  if(config&0x80)      // Interrupt on transmission
    TXIE_x = 1;
  else
    TXIE_x = 0;

  SPBRG = spbrg;       // Write baudrate to SPBRG1
  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver
 
	TRISC6 = 0;TRISC7 = 1; 
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISC6 = 1;
}

#elif defined (_EAUSART_V3) || defined (_EAUSART_V4) || defined (_EAUSART_V5) 

void OpenUSART( unsigned char config, unsigned int spbrg)
{
  TXSTA = 0;           // Reset USART registers to POR state
  RCSTA = 0;
 
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
  
    TXIF = 0;
  if(config&0x20)
	 SENDB_x = 1;

  if(config&0x40)      // Interrupt on receipt
    RCIE_x = 1;
  else
    RCIE_x = 0;

  RCIF = 0;

  if(config&0x80)      // Interrupt on transmission
    TXIE_x = 1;
  else
    TXIE_x = 0;

  SPBRG = spbrg;       // Write baudrate to SPBRG1
  SPBRGH = spbrg >> 8; // For 16-bit baud rate generation

  TXEN_x = 1;  // Enable transmitter
  SPEN_x = 1;  // Enable receiver

#if defined (_USART_IO_V1)
	TRISB1 = 0;TRISB4 = 1;
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISB1 = 1;

#elif defined (_USART_IO_V2)
	TRISA2 = 0;TRISA3 = 1;
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISA2 = 1;
#else
	TRISC6 = 0;TRISC7 = 1; 
	if(SYNC_x && !CSRC_x)	//synchronous  slave mode
		TRISC6 = 1;

#endif
	
}

#endif
