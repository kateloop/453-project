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
void testToggle();
unsigned char getNote (unsigned char, unsigned char);
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
#define G_SEL_PORTB  0b11101001    // pin 28, RB7

#define L3_SEL_PORTB 0b00111111    // pin 27, RB6
#define R3_SEL_PORTB 0b01011111    // pin 26, RB5
#define L4_SEL_PORTB 0b01110111    // pin 24, RB3
#define R4_SEL_PORTB 0b01111110    // pin 21, RB0
#define L5_SEL_PORTC 0b11011111    // pin 16, RC5
#define R5_SEL_PORTC 0b11101111    // pin 15, RC4

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
#define ADCON0_CHANA 0b00100000 // Channel 8 conversion
#define ADCON0_CHANB 0b00100100 // Channel 9 conversion
#define ADCON0_CHANC 0b00000000 // Channel 0 conversion
#define ADCON0_CHAND 0b00000100 // Channel 1 conversion
#define ADCON0_CHANE 0b00001000 // Channel 2 conversion
#define ADCON0_CHANF 0b00001100 // Channel 3 conversion
#define ADCON0_CHANG 0b00010000 // Channel 4 conversion
#define ADCON0_EN_OR 0b00000001 //ADC enable OR-mask

// ADCON1 5   Voltage Reference Config bit, 0 for AVss
//        4   Voltage Reference Config bit, 0 for AVdd
//        3-0 A/D Port Config Control Bits, 0101 sets all to Analog ports, except
//                                               AN10 to Analog
#define ADCON1_VAL 0b00000101

// ADCON2 7   A/D Result Format, 1 Right Justified
//        5-3 A/D Acquisition Time Select bits   TODO 0TAD
//        2-0 A/D Conversion Clock Select Bits   TODO Fosc/2
#define ADCON2_VAL 0b10001101    // Right justified, 2 TAD, Fosc/16

// UART Defines
#define UART_RENABLE 0b10010000
#define UART_TENABLE 0b00100100 // 3 - 1/0?
#define UART_BAUD 0b00001000 // TODO interrupt
#define SPBRGH_SPBRG (((FOSC/BAUDRATE)/16) - 1) // SYNC = 0, BRG16 = 1, BRGH = 0
#define BAUDRATE_ACT (FOSC/(16* (SPBRGH_SPBRG + 1)))
#define BAUDRATE 19200UL

// Oscillator Frequency
//#define FOSC 8000000L // 8MHz
#define FOSC 8000000L // 8 MHZ
#define CLK_8MHZ 0b01110010


////////////////////////////////////
//      Global Variables          //
////////////////////////////////////
int led_array[13];

// uart
unsigned char uart_out;

unsigned char new_note = 0x00;
unsigned char old_note = 0x00;


// Structure to hold ADC conversion information
typedef struct _ADC_conv {
 int result;        // result of ADC conversion
 int adc_channel;   // channel converted (ie what note was played)
}ADC_conv;
ADC_conv conv_result;

volatile char adc_num;  // Keeps track of which ADC channel is being used

// Inputs from the sensors, if cur is not the same as prev, new sound to be played
unsigned char cur_inputs[7];
unsigned char prev_inputs[7];

char change_val = 0;

int index;

/*
 * main
 */

int main(int argc, char** argv) {
    // Init Config
    OSCCON |= 0b01110010;  //internal oscillator, 8MHz

    // set up GPIO
    TRISA = PORTA_DIR;
    TRISB = PORTB_DIR;
    TRISC = PORTC_DIR;

    // configure ADC
    ADCON0 = ADCON0_INIT;
    ADCON1 = ADCON1_VAL;
    ADCON2 = ADCON2_VAL;
    adc_num = ADCCHANA; // reset the current adc channel to 0
 

    // configure interrupt

// TODO timer interrupt for ADC conversions


//  INTCON |= 0b10000000; //all unmasked interrupts enabled; periph, overflow, external, RB port change, TMR0 ovrflw intrpts disabled
//  INTCON2 = 0b00000000; //TODO no idea about this one
//  INTCON3 = 0b00000000; //TODO no idea about this one

        // UART config
        SPBRGH  = SPBRGH_SPBRG >> 8;
        SPBRG =  SPBRGH_SPBRG;
        BAUDCON = 0b00001000;
        RCSTA = 0b10010000;
        TXSTA = 0b00100000;

        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;


        // SAMPLE RECEIVE UART CODE: WILL NEED TO DO UART RECEIVE INTERRUPT TO CHANGE MODES

    /*    char uout = 0x01;
        while(1) {
            TXREG = uout;
            while((PIR1 & 0b00010000) == 0);
            while((PIR1 & 0b00100000) == 0);
            uout = RCREG;
            if (uout == 'a') {
                uout = 0xaa;
            }
        }*/



        // USE FOLLOWING CODE TO CALIBRATE
    while (1) {
        char uout;
        char hResult;
        char lResult;

        uout = 0xee;
        TXREG = uout;
        while((PIR1 & 0b00010000) == 0);

        
        ADON = 0;   // Turn off ADC

        ADRESH = 0x00; // reset result registers
        ADRESL = 0x00;
        ADCON0 = ADCON0_CHANE;
        // Start conversion and wait for result
        ADON = 1;   // Turn on ADC
        GODONE = 1; // Starts conversion

        // Wait for conversion to finish
        while (GODONE);
        hResult = ADRESH;
        lResult = ADRESL;
        
        uout = hResult;
        TXREG = uout;
        while((PIR1 & 0b00010000) == 0);
        uout = lResult;
        TXREG = uout;
        while((PIR1 & 0b00010000) == 0);

        DelayMs(5000);
        DelayMs(5000);
    }


    




    while (0) {  
        // wait until sensor value changes 
        while (change_val == 0) {
            // spin
            continue;
        }

        // once note changes, send data across UART and light LEDs
        uart_out = cur_inputs[index];
        TXREG = uart_out;
        while((PIR1 & 0b00010000) == 0);
        
        ToggleLeds();

        // Reset Change Val
        change_val = 0;

    }
    
   /*
        testToggle(); 
   */
     return (EXIT_SUCCESS);
}


//////////////////////////////////////////////
//  Function to Toggle LEDs according to    //
//  notes being played.                     //
//////////////////////////////////////////////

// N channels enabled by 1
// P channels enabled by 0
void ToggleLeds () {

    // Look at array containing LED Info and toggle LEDs accordingly

    // Octave 3
    if (led_array[4] == 1 && led_array[5] == 1 && led_array[6] == 1) {
        // Octave 3 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[7] == 1) {
        // Octave 3 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[8] == 1) {
        // Octave 3 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001110;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[9] == 1) {
        // Octave 3 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001101;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[10] == 1) {
        // Octave 3 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b01100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[11] == 1) {
        // Octave 3 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001011;
    } else if (led_array[4] == 1 && led_array[5] == 1 && led_array[12] == 1) {
       // Octave 5 3 & R LED on; B
       //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (led_array[4] == 1 && led_array[6] == 1) {
        // Octave 3 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[4] == 1 && led_array[7] == 1) {
        // Octave 3 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[4] == 1 && led_array[8] == 1) {
        // Octave 3 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001110;
    } else if (led_array[4] == 1 && led_array[9] == 1) {
        // Octave 3 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (led_array[4] == 1 && led_array[10] == 1) {
        // Octave 3 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b01000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[4] == 1 && led_array[11] == 1) {
        // Octave 3 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (led_array[4] == 1 && led_array[12] == 1) {
        // Octave 3 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (led_array[5] == 1 && led_array[6] == 1) {
        // Octave 3 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[5] == 1 && led_array[7] == 1) {
        // Octave 3 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[5] == 1 && led_array[8] == 1) {
        // Octave 3 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (led_array[5] == 1 && led_array[9] == 1) {
        // Octave 3 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (led_array[5] == 1 && led_array[10] == 1) {
        // Octave 3 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[5] == 1 && led_array[11] == 1) {
        // Octave 3 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (led_array[5] == 1 && led_array[12] == 1) {
        // Octave 3 Left LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    }

    // Octave 4
    else if (led_array[2] == 1 && led_array[3] == 1 && led_array[6] == 1) {
        // Octave 4 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[7] == 1) {
        // Octave 4 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[8] == 1) {
        // Octave 4 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[9] == 1) {
        // Octave 4 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001101;
    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[10] == 1) {
        // Octave 4 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[11] == 1) {
        // Octave 4 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001011;
    } else if (led_array[2] == 1 && led_array[3] == 1 && led_array[12] == 1) {
       // Octave 4 L & R LED on; B
       //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    } else if (led_array[2] == 1 && led_array[6] == 1) {
        // Octave 4 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[2] == 1 && led_array[7] == 1) {
        // Octave 4 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[2] == 1 && led_array[8] == 1) {
        // Octave 4 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (led_array[2] == 1 && led_array[9] == 1) {
        // Octave 4 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (led_array[2] == 1 && led_array[10] == 1) {
        // Octave 4 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[2] == 1 && led_array[11] == 1) {
        // Octave 4 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (led_array[2] == 1 && led_array[12] == 1) {
        // Octave 4 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (led_array[3] == 1 && led_array[6] == 1) {
        // Octave 4 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[3] == 1 && led_array[7] == 1) {
        // Octave 4 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[3] == 1 && led_array[8] == 1) {
        // Octave 4 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (led_array[3] == 1 && led_array[9] == 1) {
        // Octave 4 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (led_array[3] == 1 && led_array[10] == 1) {
        // Octave 4 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (led_array[3] == 1 && led_array[11] == 1) {
        // Octave 4 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (led_array[3] == 1 && led_array[12] == 1) {
        // Octave 4 Left LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    }

    // Octave 5
    else if (led_array[0] == 1 && led_array[1] == 1 && led_array[6] == 1) {
        // Octave 5 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;

    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[7] == 1) {
        // Octave 5 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;

    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[8] == 1) {
        // Octave 5 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111110;

    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[9] == 1) {
        // Octave 5 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111101;
    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[10] == 1) {
        // Octave 5 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;
    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[11] == 1) {
        // Octave 5 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111011;
    } else if (led_array[0] == 1 && led_array[1] == 1 && led_array[12] == 1) {
       // Octave 5 L & R LED on; B
       //LATA   -P-P----
       LATA = 0b11111111;
       //LATB   PNN-NN--
       LATB = 0b10000000;
       //LATB   --NNPPPP
       LATC = 0b00110111;

    } else if (led_array[0] == 1 && led_array[6] == 1) {
        // Octave 5 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (led_array[0] == 1 && led_array[7] == 1) {
        // Octave 5 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (led_array[0] == 1 && led_array[8] == 1) {
        // Octave 5 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011110;

    } else if (led_array[0] == 1 && led_array[9] == 1) {
        // Octave 5 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011101;

    } else if (led_array[0] == 1 && led_array[10] == 1) {
        // Octave 5 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (led_array[0] == 1 && led_array[11] == 1) {
        // Octave 5 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011011;

    } else if (led_array[0] == 1 && led_array[12] == 1) {
        // Octave 5 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00010111;

    } else if (led_array[1] == 1 && led_array[6] == 1) {
        // Octave 5 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (led_array[1] == 1 && led_array[7] == 1) {
        // Octave 5 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (led_array[1] == 1 && led_array[8] == 1) {
        // Octave 5 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101110;

    } else if (led_array[1] == 1 && led_array[9] == 1) {
        // Octave 5 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101101;

    } else if (led_array[1] == 1 && led_array[10] == 1) {
        // Octave 5 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (led_array[1] == 1 && led_array[11] == 1) {
        // Octave 5 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101011;

    } else if (led_array[1] == 1 && led_array[12] == 1) {
        // Octave 5 Left LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00100111;
    }
}


void testToggle () {

    // int led_array[13];
    // R5, L5, R4, L4, R3, L3, C, D, E, F, G, A, B

    // toggle octave 5 leds
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[0] = 1;
        led_array[1] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[0] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[1] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }

    // toggle octave 4 leds
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[2] = 1;
        led_array[3] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[2] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[3] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }

    // toggle octave 3 LEDs
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[4] = 1;
        led_array[5] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
    // toggle octave 3 LEDs
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[4] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);

    }
    // toggle octave 3 LEDs
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < sizeof(led_array); j++) {
            led_array[j] = 0;
        }
        led_array[5] = 1;
        led_array[i+6] = 1;
        ToggleLeds();
        DelayMs(5000);
    }
}

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
    while (GODONE);
    unsigned char note = getNote(ADRESH, ADRESL);

    // store conversion result
    conv_result.result = note;
    conv_result.adc_channel = adc_num;

    // Increase the number of the ADC for next conversion
    adc_num++;
    if (adc_num >= NUMADCCHAN) {
        adc_num = ADCCHANA;
    }
}


/////////////////////////////////////////
//   Function that determines note
//   to be played and data to send
//   across UART
///////////////////////////////////////////
unsigned char getNote (unsigned char hResult, unsigned char lResult) {
    unsigned char new_note;
    int i = 0;
    for (i = 0; i < 13; i++) {
        led_array[i] = 0;
    }

    if (adc_num == ADCCHANA) {
        if (hResult == 0x00) {
            // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 190 && lResult <= 210) {
                // 5AF
                new_note = 0x51;
                led_array[1] = 1;
                led_array[11] = 1;
            } else if (lResult >= 195 && lResult <= 216) {
                // 5A
                new_note = 0x41;
                led_array[1] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else if (lResult >= 128 && lResult <= 154) {
                // 5AS
                new_note = 0x49;
                led_array[1] = 1;
                led_array[12] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 222 && lResult <= 252) {
                // 4AF
                new_note = 0x31;
                led_array[1] = 1;
                led_array[9] = 1;
            } else if (lResult >= 105 && lResult <= 175) {
                // 4A
                new_note = 0x21;
                led_array[1] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 105 && lResult <= 148) {
                // 4AS
                new_note = 0x29;
                led_array[1] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 15 && lResult <= 108) {
                // 3AF
                new_note = 0x11;
                led_array[1] = 1;
                led_array[7] = 1;
            } else if (lResult >= 116 && lResult <= 183) {
                // 3A
                new_note = 0x01;
                led_array[1] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 192 && lResult <= 227) {
                // 3AS
                new_note = 0x09;
                led_array[1] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }
        }

    } else if (adc_num == ADCCHANB) {
        if (hResult == 0x00) {
            // no note playing
            new_note = 0x00;
        } else if (hResult == 0x01) {
            if (lResult >= 187 && lResult <= 213) {
                // 5BF
                new_note = 0x52;
                led_array[0] = 1;
                led_array[11] = 1;
            } else if (lResult >= 161 && lResult <= 182) {
                // 5B
                new_note = 0x42;
                led_array[0] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else if (lResult >= 120 && lResult <= 150) {
                // 5BS
                new_note = 0x4a;
                led_array[0] = 1;
                led_array[12] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 206 && lResult <= 232) {
                // 4BF
                new_note = 0x32;
                led_array[0] = 1;
                led_array[9] = 1;
            } else if (lResult >= 165 && lResult <= 238) {
                // 4B
                new_note = 0x22;
            } else if (lResult >= 109 && lResult <= 140) {
                // 4BS
                new_note = 0x2a;
                led_array[0] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
                led_array[0] = 1;
                led_array[10] = 1;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 0 && lResult <= 174) {
                // 3BF
                new_note = 0x12;
                led_array[0] = 1;
                led_array[7] = 1;
            } else if (lResult >= 193 && lResult <= 210) {
                // 3B
                new_note = 0x02;
                led_array[0] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 221 && lResult <= 229) {
                // 3BS
                new_note = 0x0a;
                led_array[0] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }
        }

    } else if (adc_num == ADCCHANC) {
        if (hResult == 0x00) {
           // No note playing
           new_note = 0x00;
        } else if (hResult == 0x01) {
           if (lResult >= 218 && lResult <= 242) {
               // 5CF
               new_note = 0x53;
               led_array[6] = 1;
               led_array[11] = 1;
           } else if (lResult >= 167 && lResult <= 200) {
                // 5C
                new_note = 0x43;
                led_array[6] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
           } else if (lResult >= 38 && lResult <= 160) {
                // 5CS
                new_note = 0x4b;
                led_array[6] = 1;
               led_array[12] = 1;
           } else {
                // no note playing
                new_note = 0x00;
           }

        } else if (hResult == 0x02) {
            if (lResult >= 201 && lResult <= 235) {
                // 4CF
                new_note = 0x33;
                led_array[6] = 1;
                led_array[9] = 1;
            } else if (lResult >= 124 && lResult <= 183) {
                // 4C
                new_note = 0x23;
                led_array[6] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 70 && lResult <= 110) {
                // 4CS
                new_note = 0x2b;
                led_array[6] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 60 && lResult <= 87) {
                // 3CF
                new_note = 0x13;
                led_array[6] = 1;
                led_array[7] = 1;
            } else if (lResult >= 138 && lResult <= 200) {
                // 3C
                new_note = 0x03;
                led_array[6] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 212 && lResult <= 232) {
                // 3CS
                new_note = 0x0b;
                led_array[6] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }
        }

    } else if (adc_num == ADCCHAND) {
        if (hResult == 0x00) {
            // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 200 && lResult <= 230) {
                // 5DF
                new_note = 0x54;
                led_array[5] = 1;
                led_array[11] = 1;
            } else if (lResult >= 130 && lResult <= 180) {
                // 5D
                new_note = 0x44;
                led_array[5] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else if (lResult >= 90 && lResult <= 120) {
                // 5DS
                led_array[5] = 1;
                led_array[12] = 1;
                new_note = 0x4c;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 203 && lResult <= 255) {
                // 4DF
                new_note = 0x34;
                led_array[5] = 1;
                led_array[9] = 1;
            } else if (lResult >= 130 && lResult <= 167) {
                // 4D
                new_note = 0x24;
                led_array[5] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 87 && lResult <= 124) {
                // 4DS
                new_note = 0x2c;
                led_array[5] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 60 && lResult <= 160) {
                // 3DF
                new_note = 0x14;
                led_array[5] = 1;
                led_array[7] = 1;
            } else if (lResult >= 145 && lResult <= 205) {
                // 3D
                new_note = 0x04;
                led_array[5] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 227 && lResult <= 239) {
                // 3DS
                new_note = 0x0c;
                led_array[5] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        }

    } else if (adc_num == ADCCHANE) {
        if (hResult == 0x00) {
            // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 5EF
                new_note = 0x55;
                led_array[4] = 1;
                led_array[11] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5E
                new_note = 0x45;
                led_array[4] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5ES
                new_note = 0x4d;
                led_array[4] = 1;
                led_array[12] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 4EF
                new_note = 0x35;
                led_array[4] = 1;
                led_array[9] = 1;

            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4E
                new_note = 0x25;
                led_array[4] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4ES
                new_note = 0x2d;
                led_array[4] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 3EF
                new_note = 0x15;
                led_array[4] = 1;
                led_array[7] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3E
                new_note = 0x05;
                led_array[4] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3ES
                new_note = 0x0d;
                led_array[4] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        }

    } else if (adc_num == ADCCHANF) {
        if (hResult == 0x00) {
           // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 5FF
                new_note = 0x56;
                led_array[3] = 1;
                led_array[11] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5F
                new_note = 0x46;
                led_array[3] = 1;
                led_array[12] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5FS
                new_note = 0x4e;
                led_array[3] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 4FF
                new_note = 0x36;
                led_array[3] = 1;
                led_array[9] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4F
                new_note = 0x26;
                led_array[3] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4FS
                new_note = 0x2e;
                led_array[3] = 1;
                led_array[10] = 1;
            } else {
               // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 3FF
                new_note = 0x16;
                led_array[3] = 1;
                led_array[7] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3F
                new_note = 0x06;
                led_array[3] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3FS
                new_note = 0x0e;
                led_array[3] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        }

    } else if (adc_num == ADCCHANG) {
        if (hResult == 0x00) {
           // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 5GF
                new_note = 0x57;
                led_array[2] = 1;
                led_array[11] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5G
                new_note = 0x47;
                led_array[2] = 1;
                led_array[11] = 1;
                led_array[12] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 5GS
                new_note = 0x4f;
                led_array[2] = 1;
                led_array[12] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 4GF
                new_note = 0x37;
                led_array[2] = 1;
                led_array[9] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4G
                new_note = 0x27;
                led_array[2] = 1;
                led_array[9] = 1;
                led_array[10] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 4GS
                new_note = 0x2f;
                led_array[2] = 1;
                led_array[10] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 0x00 && lResult <= 0x00) {
                // 3GF
                new_note = 0x17;
                led_array[2] = 1;
                led_array[7] = 1;

            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3G
                new_note = 0x07;
                led_array[2] = 1;
                led_array[7] = 1;
                led_array[8] = 1;
            } else if (lResult >= 0x00 && lResult <= 0x00) {
                // 3GS
                new_note = 0x0f;
                led_array[2] = 1;
                led_array[8] = 1;
            } else {
                // no note playing
                new_note = 0x00;
            }

        }

    }
    return new_note;
}