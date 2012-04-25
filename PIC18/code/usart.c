
/*A sample project file to be used with HI-TIDE, to demonstrate the functionality of the microcontroller's USART.*/
/*Refer to usart.txt for additional information*/

#include <pic18.h>
#include "usart.h"
#include <stdio.h>
#include "serial.h"

void init(void);
void write(int); 
unsigned char read(int);    
unsigned char getchSpecial(void);


volatile bit UPDATE_REQUIRED ;                                 /*used to indicate when to refresh output data */
volatile bit DATA_RECEIVED;                                      /*used to indicate when new data is received from USART*/
volatile bit TX_INT;                                                       /*used to indicate when transmit interrupt should be enabled*/
volatile char DATA;                                                     /*data received from USART*/

char c;
void main(void)
{
    init();
    while(1)
    {
		if (UPDATE_REQUIRED)
        {
			UPDATE_REQUIRED=0;
			CREN=0;                                                     /*reset USART receiver logic to clear OERR*/
			CREN=1;

            printf("\n USE PUSH BUTTONS TO SWITCH MODES\n----------------------------------------------\n");
            
            if (TX_INT) printf(" USART transmit interrupt enabled \n----------------------------------------------\n");
            else printf(" USART transmit interrupt disabled \n----------------------------------------------\n");

            if(RCIE)                                                
            {
                printf(" USART receive interrupt enabled\n----------------------------------------------\n");
                printf(" Waiting for USART interrupt. Type something. \n----------------------------------------------\n");
            }
            else
            {
                printf(" USART receive interrupt disabled\n----------------------------------------------\n");
                printf(" Polling for input.Type something or press Enter <CR> to exit\n----------------------------------------------\n");

                while((DATA=getchSpecial())!=10)                   /*If USART receive interrupt is disabled the RX port is polled*/
                {
                    if (TX_INT&&(!TXIE)) TXIE=1;              /*if USART transmit interrupt enable use ISR to output characters */
                    else printf("%c",DATA);                     /*otherwise use printf() which calls putch() routine*/
                }
                printf("\n----------------------------------------------\n");
            }
        }
        if (DATA_RECEIVED)                                       /*this code is executed if USART receive interrupt is enabled*/
        {
            if (TX_INT&&(!TXIE))TXIE=1;                         /*if USART transmit interrupt enable use ISR to output characters */       
            else    printf("%c",DATA);                            /*otherwise use printf() which calls putch() routine*/                              
            DATA_RECEIVED=0;                                                                                                                                                          
        }
    }
}

unsigned char getchSpecial() 
{
                                                                                    /*retrieve one byte */
while(!RCIF)    
    {
        if (UPDATE_REQUIRED)	  return 10;        /*if buttons were pressed while waiting for keyboard input*/
    }                                                                              /*exit and return <CR> character*/
    
    return RCREG;                                                        /*set when register is not empty */
}

void init(void)
{
    RBIE=1;                                                                   /*enable PORTB interrupts to */
    GIEH=1;                                                                   /*allow interrupts from PUSH BUTTONS */
    GIEL=1; 

    init_comms();                                                        /*initialize usart in serial.c*/

    TRISB=0xF0;                                                          

    ADCON1=0x0F;                                                     /*configure pins of ADC pins to be digital inputs*/
    UPDATE_REQUIRED=1;								          /*set initial values to variables*/
    DATA_RECEIVED=0;
    TX_INT=0;
}

