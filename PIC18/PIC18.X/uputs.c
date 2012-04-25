#include <htc.h>
#include <peripheral/usart.h>

/**********************************************************************
*    Function Name:  putsUSART                                        *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the USART including the null.                 *
**********************************************************************/

#if defined (_AUSART_V1) || defined (_EAUSART_V3) || defined (_EAUSART_V4) || defined (_EAUSART_V5)

void putsUSART( char *data)
{
  do
  {  // Transmit a byte
    while(BusyUSART());
    putcUSART(*data);
  } while( *data++ );
}

#endif

/**********************************************************************
*    Function Name:  puts1USART                                        *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the USART1 including the null.                 *
**********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V10) || defined (_EAUSART_V11)
void puts1USART( char *data)
{
  do
  {  // Transmit a byte
    while(Busy1USART());
    putc1USART(*data);
  } while( *data++ );
}
#endif

/**********************************************************************
*    Function Name:  puts2USART                                        *
*    Return Value:   void                                             *
*    Parameters:     data: pointer to string of data                  *
*    Description:    This routine transmits a string of characters    *
*                    to the USART2 including the null.                 *
**********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V11)

void puts2USART( char *data)
{
  do
  {  // Transmit a byte
    while(Busy2USART());
    putc2USART(*data);
  } while( *data++ );
}

#endif
