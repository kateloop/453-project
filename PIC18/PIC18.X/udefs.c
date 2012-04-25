#include <htc.h>
#include <peripheral/usart.h>

/* These devices only have a single USART */
#if defined (_AUSART_V1 ) || defined (_EAUSART_V3 ) || defined (_EAUSART_V4 ) || defined (_EAUSART_V5 )
union USART USART_Status;
#endif

/* Only these devices have two USART modules: USART1 & USART2. */
#if defined (_AUSART_V2 ) ||defined (_EAUSART_V6 ) ||defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 ) || defined (_EAUSART_V10) || defined (_EAUSART_V11)
union USART1 USART1_Status;
#endif

/* Only these devices have two USART modules: USART1 & USART2. */
#if defined (_AUSART_V2 ) ||defined (_EAUSART_V6 ) ||defined (_EAUSART_V7 ) ||defined (_EAUSART_V8 ) ||defined (_EAUSART_V9 ) || defined (_EAUSART_V11)
union USART2 USART2_Status;
#endif
