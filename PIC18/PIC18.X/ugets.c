#include <htc.h>
#include <peripheral/usart.h>

/********************************************************************
*    Function Name:  getsUSART                                      *
*    Return Value:   void                                           *
*    Parameters:     buffer: pointer to string                      *
*                    len: length of characters to receive           *
*    Description:    This routine receives a string of characters   *
*                    from the USART of length specified by len.     *
********************************************************************/
#if defined (_AUSART_V1) || defined (_EAUSART_V3) || defined (_EAUSART_V4) || defined (_EAUSART_V5)

void getsUSART(char *buffer, unsigned char len)
{
  char i;    // Length counter

  for(i=0;i<len;i++)  // Only retrieve len characters
  {
    while(!DataRdyUSART());// Wait for data to be received

    *buffer = getcUSART();    // Get a character from the USART
                           // and save in the string
    buffer++;              // Increment the string pointer
  }
}

#endif

/********************************************************************
*    Function Name:  gets1USART                                      *
*    Return Value:   void                                           *
*    Parameters:     buffer: pointer to string                      *
*                    len: length of characters to receive           *
*    Description:    This routine receives a string of characters   *
*                    from the USART of length specified by len.     *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V10) || defined (_EAUSART_V11)

void gets1USART(char *buffer, unsigned char len)
{
  char i;    // Length counter

  for(i=0;i<len;i++)  // Only retrieve len characters
  {
    while(!DataRdy1USART());// Wait for data to be received

    *buffer = getc1USART();    // Get a character from the USART
                           // and save in the string
    buffer++;              // Increment the string pointer
  }
}

#endif

/********************************************************************
*    Function Name:  gets2USART                                      *
*    Return Value:   void                                           *
*    Parameters:     buffer: pointer to string                      *
*                    len: length of characters to receive           *
*    Description:    This routine receives a string of characters   *
*                    from the USART of length specified by len.     *
********************************************************************/
#if defined (_AUSART_V2) || defined (_EAUSART_V6)|| defined (_EAUSART_V7) || defined (_EAUSART_V8) || defined (_EAUSART_V9) || defined (_EAUSART_V11)

void gets2USART(char *buffer, unsigned char len)
{
  char i;    // Length counter
  
  for(i=0;i<len;i++)  // Only retrieve len characters
  {
    while(!DataRdy2USART());// Wait for data to be received

    *buffer = getc2USART();    // Get a character from the USART
                           // and save in the string
    buffer++;              // Increment the string pointer
  }
}

#endif
