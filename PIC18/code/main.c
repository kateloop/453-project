/* 
 * File:   main.c
 * Author: Katelyn
 *
 * Created on March 29, 2012, 2:00 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <peripheral/adc.h>
#include <peripheral/sw_uart.h>
#include <peripheral/usart.h>
#include <errno.h>
#include <stddef.h>
#include <pic18f2682.h>
#include <htc.h>
#include "../PIC18.X/delay.h"
#include "../PIC18.X/usart.h"

__CONFIG (1, OSC_IRCIO67);
__CONFIG (2, WDT_OFF);
__CONFIG (3, MCLRE_OFF);
__CONFIG (4, LVP_OFF & DEBUG_OFF);
__CONFIG (5, UNPROTECT);
__CONFIG (6, UNPROTECT);
__CONFIG (7, UNPROTECT);

/*
/ ADC /
#define _ADC_V5
/ CC /
#define _CC_V1
/ PWM /
#define _PWM_V1
/ USART /
#define _EAUSART_V4
/ TIMERS /
#define _TMR_V2
/ EEPROM /
#define _EEP_V3
/ PORT_B /
#define _PTB_V1
*/


////////////////////////////////
//  Function Declarations     //
////////////////////////////////

void ToggleLeds();
void adc_conversion();

//////////////////////////////////
//  defines                     //
//////////////////////////////////
// ADC
#define ADCCHANA   0 // pin 2, AN0
#define ADCCHANB   1 // pin 3, AN1
#define ADCCHANC   2 // pin 4, AN2
#define ADCCHAND   3 // pin 5, AN3
#define ADCCHANE   4 // pin 7, AN4
#define ADCCHANF   5 // pin 25, AN9
#define ADCCHANG   6 // pin 22, AN8
#define NUMADCCHAN 7 // total number of ADC channels being used
// LEDS
#define A_SEL_PORTC  0b00000100    // pin 13, RC2
#define B_SEL_PORTC  0b00001000    // pin 14, RC3
#define C_SEL_PORTA  0b00010000    // pin 6, RA4
#define D_SEL_PORTA  0b01000000    // pin 10, RA6
#define E_SEL_PORTC  0b00000001    // pin 11, RC0
#define F_SEL_PORTC  0b00000010    // pin 12, RC1
#define G_SEL_PORTB  0b10000000    // pin 28, RB7
#define L3_SEL_PORTB 0b01000000    // pin 27, RB6
#define R3_SEL_PORTB 0b00100000    // pin 26, RB5
#define L4_SEL_PORTB 0b00001000    // pin 24, RB3
#define R4_SEL_PORTB 0b00000001    // pin 21, RB0
#define L5_SEL_PORTC 0b00100000    // pin 16, RC5
#define R5_SEL_PORTC 0b00010000    // pin 15, RC4

// PORT A Directions : Input == 1; Output == 0
    // RA7(ONLY INPUT) RA6(ONLY OUTPUT)  AN4 RA4 AN3 AN2 AN1 AN0
#define PORTA_DIR 0b10101111
// PORT B Directions : Input == 1; Output == 0
    // RB7 RB6 RB5 AN9 RB3 RB2 AN8 RB0
#define PORTB_DIR 0b00010010
// PORT C Directions : Input == 1; Output == 0
    // RX TX RC5 RC4 RC3 RC2 RC1 RC0
#define PORTC_DIR 0b10000000


// ADC Config, result in ADRESH and ADRESL registers
// when A/D conversion is complete ADCON0 bit 1 = 0 and ADIF (A/D interrupt flag) is set

// ADCON0 5-2 Analog Channel Select Bits (0-3, 7-10)
//        1   A/D Conversion Status Bit: 1 A/D conversion in progress, 0 A/D idle
//        0   A/D on bit: 1 enabled, 0 disabled
#define ADCON0_INIT  0b00000000 // Initial settings; ADC off and not converting
#define ADCON0_CHANA 0b00100100 // Channel 9 conversion
#define ADCON0_CHANB 0b00100000 // Channel 8 conversion
#define ADCON0_CHANC 0b00000000 // Channel 0 conversion
#define ADCON0_CHAND 0b00000100 // Channel 1 conversion
#define ADCON0_CHANE 0b00001000 // Channel 2 conversion
#define ADCON0_CHANF 0b00001100 // Channel 3 conversion
#define ADCON0_CHANG 0b00010000 // Channel 4 conversion

// ADCON1 5   Voltage Reference Config bit, 0 for AVss
//        4   Voltage Reference Config bit, 0 for AVdd
//        3-0 A/D Port Config Control Bits, 0101 sets all to Analog ports, except
//                                               AN10 to Analog
#define ADCON1_VAL 0b00000101

// ADCON2 7   A/D Result Format, 1 Right Justified
//        5-3 A/D Acquisition Time Select bits   TODO 0TAD
//        2-0 A/D Conversion Clock Select Bits   TODO Fosc/2
#define ADCON2_VAL 0b10000000

// UART Defines
#define UART_RENABLE 0b10010000
#define UART_TENABLE 0b00100100 // 3 - 1/0?
#define UART_BAUD 0b00001000 // TODO interrupt
#define SPBRGH_SPBRG (((FOSC/BAUDRATE)/16) - 1) // SYNC = 0, BRG16 = 1, BRGH = 0
#define BAUDRATE_ACT (FOSC/(16* (SPBRGH_SPBRG + 1)))
#define BAUDRATE 19200UL
#define openParam 0b00011000
#define baudParam 0b0100

// Oscillator Frequency
//#define FOSC 8000000L // 8MHz
#define FOSC 8000000L // 8 MHZ
#define CLK_8MHZ 0b01110010


////////////////////////////////////
//      Global Variables          //
////////////////////////////////////
int led_array[13];

unsigned char uart_out;
unsigned char uart_in;

// Structure to hold ADC conversion information
typedef struct _ADC_conv {
 int result;        // result of ADC conversion
 int adc_channel;   // channel converted (ie what note was played)
}ADC_conv;
ADC_conv conv_result;

volatile char adc_num;  // Keeps track of which ADC channel is being used

// Inputs from the sensors, if cur is not the same as prev, new sound to be played
int cur_inputs[7];
int prev_inputs[7];

char change_val = 0;

int index;

/*
 * main
 */

int main(int argc, char** argv) {
    // Init Config
    OSCCON |= 0b00000010;

    // configure ADC
    ADCON0 = ADCON0_INIT;
    ADCON1 = ADCON1_VAL;
    ADCON2 = ADCON2_VAL;
    ADCON2 = 0b10100000;  // Right Justified, 8TAD, Fosc/2
    adc_num = ADCCHANA; // reset the current adc channel to 0

    // set up GPIO
    TRISA = PORTA_DIR;
    TRISB = PORTB_DIR;
    TRISC = PORTC_DIR;

    // configure interrupt

    
//	INTCON |= 0b10000000; //all unmasked interrupts enabled; periph, overflow, external, RB port change, TMR0 ovrflw intrpts disabled
//	INTCON2 = 0b00000000; //TODO no idea about this one
//	INTCON3 = 0b00000000; //TODO no idea about this one

	 /*TODO testing UART:  The UART pins on the TLL6219 are pins 13-16 on the GPIO connector Chris 		suggests taking these steps:
			1. send a bunch of A's from the PIC and see if they print out on the screen; if they do, you have UART communication!
			2. send relevant (non-character) data from the PIC and enable raw data part of uart code so it prints right 
			3.send data from the Arm and either echo it back and have it print out or use scope
			Chris said he is more than willing to reiterate these steps later.  Also, the scope should looks like it's going crazy when 				we send A's, so that's a good indication of whether or not we have it working. He also said to assume that the problem is 				with the PIC code.....let's hope he's right about that.
	*/
    

    // Turn on ADC and enable interrupts
   // ADON = 1;
   // ADC_INT_ENABLE();
    
        // UART config
        SPBRGH  = SPBRGH_SPBRG >> 8;
        SPBRG =  SPBRGH_SPBRG;
        BAUDCON = 0b00001000;
        RCSTA = 0b10010000;
        TXSTA = 0b00100000;

        while (1)
        {
            
           // LATB = 0b00000000;
          //  DelayMs(20);
            TXREG = 'a';
            while ((PIR1 & 0b00010000) ==0);
           // LATB = 0b00100000;
          //  DelayMs(20);

            while ((PIR1 & 0b00100000) == 0) {
                LATB = 0b00100000;
                DelayMs(20);
                LATB = 0b00000000;
                DelayMs(20);
            }

        //    if (RCREG == 'a') {
        //
        //    }
        }
        /*while (1) {
            unsigned char tx = 'a';
             WriteUSART(tx);
        }*/

          /*   while (1) {
                LATC = 0b01000000;
                DelayMs(20);
                LATC = 0b00000000;
                DelayMs(20);
             }*/
    

        

      /*  while (!RCIF) {
            LATB = 0b00000000;
             DelayMs(20);
             LATB = 0b00100000;
             DelayMs(20);
        }
        unsigned char rv;
        rv = ReadUSART();
*/
      //  while (rv == tx) {
      //      continue;
      //  }
    


    while (0) {
        // once receive data on the uart, stuff into led array
        ToggleLeds();

        // wait until sensor value changes and send out on uart
        while (change_val == 0) {
            // spin
            continue;
        }
        int result = cur_inputs[index];
        char channel = index; // 0 == A, 6 == G
        // TODO send result and channel across UART
        
    }




  //  return (EXIT_SUCCESS);
}


//////////////////////////////////////////////
//  Function to Toggle LEDs according to    //
//  notes being played.                     //
//////////////////////////////////////////////

void ToggleLeds () {
    // Reset All Ports to 0 to turn off all LEDs
    LATA = 0;
    LATB = 0;
    LATC = 0;

    // Look at array containing LED Info and toggle LEDs accordingly
    if (led_array[0] == 1) {
        // Octtave 5 Right LED is on
        LATC |= R5_SEL_PORTC;
    }
    if (led_array[1] == 1) {
        // Octave 5 Left LED is on
        LATC |= L5_SEL_PORTC;
    }
    if (led_array[2] == 1) {
        // Octave 4 Right LED is on
        LATB |= R4_SEL_PORTB;
    }
    if (led_array[3] == 1) {
        // Octave 4 Left LED is on
        LATB |= L4_SEL_PORTB;
    }
    if (led_array[4] == 1) {
        // Octave 3 Right LED is on
        LATB |= R3_SEL_PORTB;
    }
    if (led_array[5] == 1) {
        // Octave 3 Left LED is on
        LATB |= L3_SEL_PORTB;
    }
    if (led_array[6] == 1) {
        // BSel LED is on
        LATC |= B_SEL_PORTC;
    }
    if (led_array[7] == 1) {
        // ASel LED is on
        LATC |= A_SEL_PORTC;
    }
    if (led_array[8] == 1) {
        // GSel LED is on
        LATB |= G_SEL_PORTB;
    }
    if (led_array[9] == 1) {
        // FSel LED is on
        LATC |= F_SEL_PORTC;
    }
    if (led_array[10] == 1) {
        // ESel LED is on
        LATC |= E_SEL_PORTC;
    }
    if (led_array[11] == 1) {
        // DSel LED is on
        LATA |= D_SEL_PORTA;
    }
    if (led_array[12] == 1) {
        // CSel LED is on
        LATA |= C_SEL_PORTA;
    }
}

/*
 * Does an ADC conversion
 */
/*void adc_conversion (int adc_channel) {

    ADC_INT_DISABLE (); // disable interrupts
    
    // ADIE = 0;  // Mask interrupt
     //ADIF = 0;  // Reset ADC interrupt bit
    
    // Reenable interrupts
    ADC_INT_ENABLE();
}*/


// ISR that processes ADC data from 7 inputs in a sequential fashion

ISR () {
    // TODO Disable Interrupts

    adc_conversion ();

    cur_inputs[conv_result.adc_channel] = conv_result.result;

    if (cur_inputs[conv_result.adc_channel] != prev_inputs[conv_result.adc_channel]) {
        change_val = 1;
        prev_inputs[conv_result.adc_channel] = cur_inputs[conv_result.adc_channel];
        index = conv_result.adc_channel;
    } else {
        change_val = 0;
    }

    // TODO Reenable Interrupts
    
}


//////////////////////////////////////////////
// Function that processes ADC data from    //
// 7 inputs in a sequential fashion         //
//////////////////////////////////////////////

void adc_conversion() {

    ADON = 0;   // Turn off ADC

    ADRESH = 0; // reset result registers
    ADRESL = 0;

    // set ADCON0 register, start conversion, write result in new
    switch (adc_num) {
        case ADCCHANA :
            ADCON0 = ADCON0_CHANA;
            break;
        case ADCCHANB :
            ADCON0 = ADCON0_CHANB;
            break;
        case ADCCHANC :
            ADCON0 = ADCON0_CHANC;
            break;
        case ADCCHAND :
            ADCON0 = ADCON0_CHAND;
            break;
        case ADCCHANE :
            ADCON0 = ADCON0_CHANE;
            break;
        case ADCCHANF :
            ADCON0 = ADCON0_CHANF;
            break;
        case ADCCHANG :
            ADCON0 = ADCON0_CHANG;
            break;
    }


    // Start conversion and wait for result
    ADON = 1;   // Turn on ADC
    GODONE = 1; // Starts conversion

    // Wait for conversion to finish
    while (GODONE) {
        ADON = 0;
    }

    // store conversion result
    conv_result.result = ADRES;
    conv_result.adc_channel = adc_num;

    // Increase the number of the ADC for next conversion
    adc_num++;
    if (adc_num >= NUMADCCHAN) {
        adc_num = ADCCHANA;
    }
}

