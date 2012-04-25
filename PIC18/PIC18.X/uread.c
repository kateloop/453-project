#include <htc.h>
#include <peripheral/usart.h>


/********************************************************************
*    Function Name:  ReadUSART                                      *
*    Return Value:   char: received data                            *
*    Parameters:     void                                           *
*    Description:    This routine reads the data from the USART     *
*                    and records the status flags for that byte     *
*                    in USART_Status (Framing and Overrun).         *
********************************************************************/
#if defined (_AUSART_V1) || defined (_EAUSART_V3) || defined (_EAUSART_V4) || defined (_EAUSART_V5)

char ReadUSART(void)		//this function can be removed by macro #define ReadUSART RCREG
{
  USART_Status.val &= 0xf2;          // Clear previous status flags

  if(RX9_x)                  // If 9-bit mode
    USART_Status.RX_NINE = RX9D;        // Clear the receive bit 9 for USART according to the RX9D bit

  if(FERR)                 // If a framing error occured
    USART_Status.FRAME_ERROR = 1;    // Set the status bit

  if(OERR)                 // If an overrun error occured
    USART_Status.OVERRUN_ERROR = 1;  // Set the status bit

  return (RCREG);                     // Return the received data
}

#endif

/********************************************************************
*    Function Name:  Read1USART                                      *
*    Return Value:   char: received data                            *
*    Parameters:     void                                           *
*    Description:    This routine reads the data from the USART1     *
*                    and records the status flags for that byte     *
*                    in USART1_Status (Framing and Overrun).         *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V10) || defined (_EAUSART_V11)

char Read1USART(void)
{
  USART1_Status.val &= 0xf2;          // Clear previous status flags

  if(RX91)                  // If 9-bit mode
    USART1_Status.RX_NINE = RX9D1;        // Clear the recieve bit 9 for USART1 according to RX9D1

  if(FERR1)                 // If a framing error occured
    USART1_Status.FRAME_ERROR = 1;    // Set the status bit

  if(OERR1)                 // If an overrun error occured
    USART1_Status.OVERRUN_ERROR = 1;  // Set the status bit

  return (RCREG1);                     // Return the received data
}

#endif

/********************************************************************
*    Function Name:  Read2USART                                      *
*    Return Value:   char: received data                            *
*    Parameters:     void                                           *
*    Description:    This routine reads the data from the USART2     *
*                    and records the status flags for that byte     *
*                    in USART2_Status (Framing and Overrun).         *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V11)

char Read2USART(void)
{
  USART2_Status.val &= 0xf2;          // Clear previous status flags

  if(RX92)                  // If 9-bit mode
    USART2_Status.RX_NINE = RX9D2;        // Clear the recieve bit 9 for USART2 according to the RX9D bit

  if(FERR2)                 // If a framing error occured
    USART2_Status.FRAME_ERROR = 1;    // Set the status bit

  if(OERR2)                 // If an overrun error occured
    USART2_Status.OVERRUN_ERROR = 1;  // Set the status bit

  return (RCREG2);                     // Return the received data
}

#endif
