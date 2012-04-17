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
#include <../samples/std/delay/delay.h>

/*
/ ADC /
#define _ADC_V5
/ CC /
#define _CC_V1
/ PWM /
#define _PWM_V1
/ USART /
#define _EAUSART_V4
/ SPI /
#define _SPI_V1
/ I2C /
#define _I2C_V1
/ CAN /
#define _ECAN_V2
/ TIMERS /
#define _TMR_V2
/ EEPROM /
#define _EEP_V3
/ PORT_B /
#define _PTB_V1
*/


//////////////////////////////////
//  defines
//////////////////////////////////
// ADC
#define ADCCHANA 0 // pin 2, AN0
#define ADCCHANB 1 // pin 3, AN1
#define ADCCHANC 2 // pin 4, AN2
#define ADCCHAND 3 // pin 5, AN3
#define ADCCHANE 4 // pin 7, AN4
#define ADCCHANF 5 // pin 25, AN9
#define ADCCHANG 6 // pin 22, AN8

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

// 1 -> INPUT, 0 -> OUTPUT
// PORT A Directions
    // RA7(ONLY INPUT) RA6(ONLY OUTPUT)  AN4 RA4 AN3 AN2 AN1 AN0
#define PORTA_DIR 0b10101111
// PORT B Directions
    // RB7 RB6 RB5 AN9 RB3 RB2 AN8 RB0
#define PORTB_DIR 0b00010010
// PORT C Directions
    // RX TX RC5 RC4 RC3 RC2 RC1 RC0
#define PORTC_DIR 0b10000000

// TODO convert one more GPIO to output??

// ADC Config, result in ADRESH and ADRESL registers
// when A/D conversion is complete ADCON0 bit 1 = 0 and ADIF (A/D interrupt flag) is set

// ADCON0 5-2 Analog Channel Select Bits (0-3, 7-10)
//        1   A/D Conversion Status Bit: 1 A/D conversion in progress, 0 A/D idle
//        0   A/D on bit: 1 enabled, 0 disabled
#define ADCON0_INIT  0b00000000 // Initial settings; ADC off and not converting
#define ADCON0_CHANA 0b00000000 // Channel 0 conversion
#define ADCON0_CHANB 0b00000100 // Channel 1 conversion
#define ADCON0_CHANC 0b00001000 // Channel 2 conversion
#define ADCON0_CHAND 0b00001100 // Channel 3 conversion
#define ADCON0_CHANE 0b00100000 // Channel 8 conversion
#define ADCON0_CHANF 0b00100100 // Channel 9 conversion
#define ADCON0_CHANG 0b00101000 // Channel 10 conversion

// ADCON1 5   Voltage Reference Config bit, 0 for AVss
//        4   Voltage Reference Config bit, 0 for AVdd
//        3-0 A/D Port Config Control Bits, 0101 sets all to Analog ports, except
//                                               AN10 to Analog
#define ADCON1_VAL 0b00000101

// ADCON2 7   A/D Result Format, 1 Right Justified
//        5-3 A/D Acquisition Time Select bits   TODO
//        2-0 A/D Conversion Clock Select Bits   TODO
#define ADCON2_VAL 0b10000000


/******************************
 * Global Variables
 *****************************/
int led_array[13];

// Inputs from the sensors, if cur is not the same as prev, new sound to be played
int cur_inputs[7];
int prev_inputs[7];

// Structure to hold ADC conversion information
//typedef struct ADC_conv {
// int result;        // result of ADC conversion
// int adc_channel;   // channel converted (ie what note was played)
//};
//ADC_conv conv_result;

/*
 * main
 */
int main(int argc, char** argv) {
    // initial setup
    OSCCON = OSC_IRCIO67;
    OSCTUNE &= 0b11100000;
   // WDTCON = WDT_ON;
    

    // UART configuration  TODO
  //  RCSTA |= 0b10000000;

    // configure ADC
    ADCON0 = ADCON0_INIT;
    ADCON1 = ADCON1_VAL;
    ADCON2 = ADCON2_VAL;

    
    // set up GPIO
    TRISA = PORTA_DIR;
    TRISB = PORTB_DIR;
    TRISC = PORTC_DIR;

    // Turn on ADC and enable interrupts
   // ADON = 1;
   // ADC_INT_ENABLE();

    // TEST: TURN ON 
    LATB = 0x00100000;
    PORTB = 0x00100000;

    while (1) {    // spin
//        if (GODONE == 0) {
            // ADC Conversion Complete; Toggle LEDs accordingly
  //          ToggleLeds();
  //      }

        // TODO anything else ?

        /*
        LATB = 0x00100000;
        PORTB = 0x00100000;

        DelayMs (1000);


        
        LATB = 0x00000000;
        PORTB = 0x00000000;
        
        DelayMs (1000);*/
        
    }
    return (EXIT_SUCCESS);
}

/*
 * Toggles LEDs
 */
void ToggleLeds () {
    // Reset All Ports to 0 to turn off all LEDs
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;

    // Look at array containing LED Info and toggle LEDs accordingly
    if (led_array[0] == 1) {
        // Octtave 5 Right LED is on
        PORTC |= R5_SEL_PORTC;
    }
    if (led_array[1] == 1) {
        // Octave 5 Left LED is on
        PORTC |= L5_SEL_PORTC;
    }
    if (led_array[2] == 1) {
        // Octave 4 Right LED is on
        PORTB |= R4_SEL_PORTB;
    }
    if (led_array[3] == 1) {
        // Octave 4 Left LED is on
        PORTB |= L4_SEL_PORTB;
    }
    if (led_array[4] == 1) {
        // Octave 3 Right LED is on
        PORTB |= R3_SEL_PORTB;
    }
    if (led_array[5] == 1) {
        // Octave 3 Left LED is on
        PORTB |= L3_SEL_PORTB;
    }
    if (led_array[6] == 1) {
        // BSel LED is on
        PORTC |= B_SEL_PORTC;
    }
    if (led_array[7] == 1) {
        // ASel LED is on
        PORTC |= A_SEL_PORTC;
    }
    if (led_array[8] == 1) {
        // GSel LED is on
        PORTB |= G_SEL_PORTB;
    }
    if (led_array[9] == 1) {
        // FSel LED is on
        PORTC |= F_SEL_PORTC;
    }
    if (led_array[10] == 1) {
        // ESel LED is on
        PORTC |= E_SEL_PORTC;
    }
    if (led_array[11] == 1) {
        // DSel LED is on
        PORTA |= D_SEL_PORTA;
    }
    if (led_array[12] == 1) {
        // CSel LED is on
        PORTA |= C_SEL_PORTA;
    }
}

/*
 * Does an ADC conversion
 */
/*void adc_conversion (int adc_channel) {

    ADC_INT_DISABLE (); // disable interrupts
    ADON = 0;   // Turn of ADC

    // determine ADC_channel to do conversion on and configure ADCON0 Register
    switch (adc_channel) {
        case ADCCHANA:
            ADCON0 |= ADCON0_CHANA;
            break;
        case ADCCHANB:
            ADCON0 |= ADCON0_CHANB;
            break;
        case ADCCHANC :
            ADCON0 |= ADCON0_CHANC;
            break;
        case ADCCHAND :
            ADCON0 |= ADCON0_CHAND;
            break;
        case ADCCHANE :
            ADCON0 |= ADCON0_CHANE;
            break;
        case ADCCHANF :
            ADCON0 |= ADCON0_CHANF;
            break;
        case ADCCHANG :
            ADCON0 |= ADCON0_CHANG;
            break;
    }

    // reset result registers
    ADRESH = 0;
    ADRESL = 0;

    
    // ADIE = 0;  // Mask interrupt
     //ADIF = 0;  // Reset ADC interrupt bit
     
    ADON = 1;   // Turn on ADC
    GODONE = 1; // Starts conversion

    // Wait for conversion to finish
    while (GODONE) {
        ADON = 0;
    }

    conv_result.result = ADRES;
    conv_result.adc_channel = adc_channel;

    // Reenable interrupts
    ADC_INT_ENABLE();
}*/

