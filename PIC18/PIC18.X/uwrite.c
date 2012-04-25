#include <htc.h>
#include <peripheral/usart.h>


/********************************************************************
*    Function Name:  WriteUSART                                     *
*    Return Value:   none                                           *
*    Parameters:     data: data to transmit                         *
*    Description:    This routine transmits a byte out the USART.   *
********************************************************************/
#if defined (_AUSART_V1) || defined (_EAUSART_V3) || defined (_EAUSART_V4) || defined (_EAUSART_V5)

void WriteUSART(char data)
{
  if(TX9_x)  // 9-bit mode?
    TX9D = USART_Status.TX_NINE;       // Set the TX9D bit according to the USART Tx 9th bit in status reg

  TXREG = data;      // Write the data byte to the USART
}

#endif

/********************************************************************
*    Function Name:  Write1USART                                     *
*    Return Value:   none                                           *
*    Parameters:     data: data to transmit                         *
*    Description:    This routine transmits a byte out the USART1.   *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V10) || defined (_EAUSART_V11) 

void Write1USART(char data)
{
  if(TX91)  // 9-bit mode?
  {
    TX9D1 = 0;       // Set the TX9D bit according to the
    if(USART1_Status.TX_NINE)  // USART1 Tx 9th bit in status reg
      TX9D1 = 1;
  }

  TXREG1 = data;      // Write the data byte to the USART2
}
#endif
/********************************************************************
*    Function Name:  Write2USART                                     *
*    Return Value:   none                                           *
*    Parameters:     data: data to transmit                         *
*    Description:    This routine transmits a byte out the USART2.   *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V11)

void Write2USART(char data)
{
  if(TX92)  // 9-bit mode?
  {
    TX9D2 = 0;       // Set the TX9D bit according to the
    if(USART2_Status.TX_NINE)  // USART2 Tx 9th bit in status reg
      TX9D2 = 1;
  }

  TXREG2 = data;      // Write the data byte to the USART2
}

#endif
