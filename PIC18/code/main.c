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
#include "../PIC18.X/uartVals.h"

__CONFIG (1, OSC_IRCIO67);
__CONFIG (2, WDT_OFF);
__CONFIG (3, MCLRE_OFF);
__CONFIG (4, LVP_OFF & DEBUG_OFF);
__CONFIG (5, UNPROTECT);
__CONFIG (6, UNPROTECT);
__CONFIG (7, UNPROTECT);

////////////////////////////////
//  Function Declarations     //
////////////////////////////////

void ToggleLeds();
void adc_conversion();
void testToggle();
unsigned char getNote (unsigned char, unsigned char);
void play (unsigned char);
void getNoteTeachMode (unsigned char);
void playPianoMan ();
void playClair();
void playClocks();

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
#define FOSC 8000000L // 8 MHZ
#define CLK_8MHZ 0b01110010

// Interrupt Defines
#define INTCON_INIT 0b00100000

// Timer0 Defines
#define T0CON_VAL 0b11000000
#define T0CON_16  0b10000000

//note length defines
#define br   2    //break length
#define en  10*8  //eighth note
#define den 15*8  //dotted eighth note
#define qn  20*8  //quarter note
#define dqn 30*8  //dotted quarter note
#define hn  40*8  //half note
#define dhn 50*8  //dotted half note
#define wn  80*8  //whole note


////////////////////////////////////
//      Global Variables          //
////////////////////////////////////
int led_array[13];

// uart
unsigned char uart_out;

unsigned char new_note = 0x00;

// Structure to hold ADC conversion information
typedef struct _ADC_conv {
 unsigned char result;        // result of ADC conversion
 unsigned char adc_channel;   // channel converted (ie what note was played)
}ADC_conv;
ADC_conv conv_result;

unsigned char chanA[3] = {0,0,0};
unsigned char chanB[3] = {0,0,0};
unsigned char chanC[3] = {0,0,0};
unsigned char chanD[3] = {0,0,0};
unsigned char chanE[3] = {0,0,0};
unsigned char chanF[3] = {0,0,0};
unsigned char chanG[3] = {0,0,0};

unsigned char mode = 0x03;

volatile char adc_num;  // Keeps track of which ADC channel is being used

// Inputs from the sensors, if cur is not the same as prev, new sound to be played
unsigned char cur_inputs[7];
unsigned char prev_inputs[7];

int index;

/*
 * main
 */

int main(int argc, char** argv) {

    while (1) {
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
 

            // configure timer0
            //T0CON = T0CON_VAL;
            T0CON = T0CON_16;

            // configure timer0 interrupt
            INTCON = INTCON_INIT;

            // configure UART Receive interrupt
            PIE1 |= 0b00100000;     // enables UART receive interrupts

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
            //LATC   --NNPPPP
            LATC = 0b00001111;

            INTCON |= 0b10000000;   // enables interrupts

        if (mode == 0x00) {
            while (mode == 0x00);
        }

        else if (mode == 0x01) {
            // play Piano Man
            INTCON = 0b10000000;  // turn off timer 0 interrupt
            while (mode == 0x01) {
                playPianoMan();

            }
        }

        else if (mode == 0x02) {
            // play Clocks
            INTCON = 0b10000000;  // turn off timer 0 interrupt
            while (mode == 0x02) {
                playClocks();

            }
        }

        else if (mode == 0x03) {
            // Play Clare De Lune
            INTCON = 0b10000000;  // turn off timer 0 interrupt
            while (mode == 0x03) {
                playClair();
            }
        }

        else {
            mode = 0x00;
        }
    }            // SAMPLE RECEIVE UART CODE: WILL NEED TO DO UART RECEIVE INTERRUPT TO CHANGE MODES

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



    return (EXIT_SUCCESS);
}

void play(unsigned char note) {
    TXREG = note;
    while((PIR1 & 0b00010000) == 0);
}

// ISR that processes ADC data from 7 inputs in a sequential fashion

void interrupt my_isr(void) {
    // Disable Interrupts
    INTCON &= 0b01111111;   // disables interrupts

    if (TMR0IE && TMR0IF && TMR0IP) {
        adc_conversion ();

       if (conv_result.adc_channel == ADCCHANA) {
            if ((chanA[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanA[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHANB) {
            if ((chanB[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanB[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHANC) {
            if ((chanC[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanC[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHAND) {
            if ((chanD[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanD[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHANE) {
            if ((chanE[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanE[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHANF) {
            if ((chanF[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {
            }
            chanF[0] = conv_result.result;
        } else if (conv_result.adc_channel == ADCCHANG) {
            if ((chanG[0] & conv_result.result) == conv_result.result) {
                TXREG = conv_result.result;
                while((PIR1 & 0b00010000) == 0);
                ToggleLeds();
            } else {

            }
            chanG[0] = conv_result.result;
        }
        // Clear flag for timer 0
        TMR0IF = 0;
    }
    // UART Interrupt
    else if (RCIF) {
        mode = RCREG;
        RCIF = 0;
    }

    // Reenable Interrupts
    INTCON |= 0b10000000;   // enables interrupts

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
    if (conv_result.adc_channel == 2 && conv_result.result == 0x03) {
        // Octave 3 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x04) {
        // Octave 3 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x05) {
        // Octave 3 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001110;
    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x06) {
        // Octave 3 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001101;
    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x07) {
        // Octave 3 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b01100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x01) {
        // Octave 3 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00001011;
    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x02) {
       // Octave 3 L & R LED on; B
       //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11100000;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x0b) {
        // Octave 3 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x0c) {
        // Octave 3 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x0d) {
        // Octave 3 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001110;
    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x16) {
        // Octave 3 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x17) {
        // Octave 3 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b01000000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x09) {
        // Octave 3 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x0a) {
        // Octave 3 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b11000000;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x13) {
        // Octave 3 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x14) {
        // Octave 3 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x15) {
        // Octave 3 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x0e) {
        // Octave 3 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x0f) {
        // Octave 3 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00100000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x11) {
        // Octave 3 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x12) {
        // Octave 3 Left LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10100000;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    }

    // Octave 4
    else if (conv_result.adc_channel == 2 && conv_result.result == 0x23) {
        // Octave 4 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x24) {
        // Octave 4 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x25) {
        // Octave 4 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x26) {
        // Octave 4 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001101;
    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x27) {
        // Octave 4 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00001100;
        //LATB   --NNPPPP
        LATC = 0b00001111;
    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x21) {
        // Octave 4 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00001011;
    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x22) {
       // Octave 4 L & R LED on; B
       //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001100;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x2b) {
        // Octave 4 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x2c) {
        // Octave 4 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x2d) {
        // Octave 4 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x2e) {
        // Octave 4 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x2f) {
        // Octave 4 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000100;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x29) {
        // Octave 4 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x2a) {
        // Octave 4 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000100;
        //LATB   --NNPPPP
        LATC = 0b00000111;

    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x33) {
        // Octave 4 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x34) {
        // Octave 4 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x35) {
        // Octave 4 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x36) {
        // Octave 4 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x37) {
        // Octave 4 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00001000;
        //LATB   --NNPPPP
        LATC = 0b00001111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x31) {
        // Octave 4 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00001011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x32) {
        // Octave 4 Left LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10001000;
        //LATB   --NNPPPP
        LATC = 0b00000111;
    }

    // Octave 5
    else if (conv_result.adc_channel == 2 && conv_result.result == 0x43) {
        // Octave 5 L & R LED on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x44) {
        // Octave 5 L & R LED on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x45) {
        // Octave 5 L & R LED on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x46) {
        // Octave 5 L & R LED on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111101;
    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x47) {
        // Octave 5 L & R LED on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00111111;
    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x41) {
        // Octave 5 L & R LED on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00111011;
    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x42) {
       // Octave 5 L & R LED on; B
       //LATA   -P-P----
       LATA = 0b11111111;
       //LATB   PNN-NN--
       LATB = 0b10000000;
       //LATB   --NNPPPP
       LATC = 0b00110111;

    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x4b) {
        // Octave 5 Right LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x4c) {
        // Octave 5 Right LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x4d) {
        // Octave 5 Right LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x4e) {
        // Octave 5 Right LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x4f) {
        // Octave 5 Right LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00011111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x49) {
        // Octave 5 Right LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00011011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x4a) {
        // Octave 5 Right LED is on; B
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00010111;

    } else if (conv_result.adc_channel == 2 && conv_result.result == 0x53) {
        // Octave 5 Left LED is on; C
        //LATA   -P-P----
        LATA = 0b11101111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (conv_result.adc_channel == 3 && conv_result.result == 0x54) {
        // Octave 5 Left LED is on; D
        //LATA   -P-P----
        LATA = 0b10111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (conv_result.adc_channel == 4 && conv_result.result == 0x55) {
        // Octave 5 Left LED is on; E
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101110;

    } else if (conv_result.adc_channel == 5 && conv_result.result == 0x56) {
        // Octave 5 Left LED is on; F
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101101;

    } else if (conv_result.adc_channel == 6 && conv_result.result == 0x57) {
        // Octave 5 Left LED is on; G
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b00000000;
        //LATB   --NNPPPP
        LATC = 0b00101111;

    } else if (conv_result.adc_channel == 0 && conv_result.result == 0x51) {
        // Octave 5 Left LED is on; A
        //LATA   -P-P----
        LATA = 0b11111111;
        //LATB   PNN-NN--
        LATB = 0b10000000;
        //LATB   --NNPPPP
        LATC = 0b00101011;

    } else if (conv_result.adc_channel == 1 && conv_result.result == 0x52) {
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
    unsigned char hResult = ADRESH;
    unsigned char lResult = ADRESL;
    unsigned char note = getNote(hResult, lResult);

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

    if (adc_num == ADCCHANA) {
        if (hResult == 0x00) {
            // no note playing
            new_note = 0x00;

        } else if (hResult == 0x01) {
            if (lResult >= 190 && lResult <= 210) {
                // 5AF
                new_note = 0x51;
            } else if (lResult >= 195 && lResult <= 216) {
                // 5A
                new_note = 0x41;
            } else if (lResult >= 128 && lResult <= 154) {
                // 5AS
                new_note = 0x49;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 222 && lResult <= 252) {
                // 4AF
                new_note = 0x31;
            } else if (lResult >= 105 && lResult <= 175) {
                // 4A
                new_note = 0x21;
            } else if (lResult >= 105 && lResult <= 148) {
                // 4AS
                new_note = 0x29;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 15 && lResult <= 108) {
                // 3AF
                new_note = 0x11;
            } else if (lResult >= 116 && lResult <= 183) {
                // 3A
                new_note = 0x01;
            } else if (lResult >= 192 && lResult <= 227) {
                // 3AS
                new_note = 0x09;
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
            } else if (lResult >= 161 && lResult <= 182) {
                // 5B
                new_note = 0x42;
            } else if (lResult >= 120 && lResult <= 150) {
                // 5BS
                new_note = 0x4a;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 206 && lResult <= 232) {
                // 4BF
                new_note = 0x32;
            } else if (lResult >= 165 && lResult <= 238) {
                // 4B
                new_note = 0x22;
            } else if (lResult >= 109 && lResult <= 140) {
                // 4BS
                new_note = 0x2a;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 0 && lResult <= 174) {
                // 3BF
                new_note = 0x12;
            } else if (lResult >= 193 && lResult <= 210) {
                // 3B
                new_note = 0x02;
            } else if (lResult >= 221 && lResult <= 229) {
                // 3BS
                new_note = 0x0a;
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
           } else if (lResult >= 167 && lResult <= 200) {
                // 5C
                new_note = 0x43;
           } else if (lResult >= 38 && lResult <= 160) {
                // 5CS
                new_note = 0x4b;
           } else {
                // no note playing
                new_note = 0x00;
           }

        } else if (hResult == 0x02) {
            if (lResult >= 201 && lResult <= 235) {
                // 4CF
                new_note = 0x33;
            } else if (lResult >= 124 && lResult <= 183) {
                // 4C
                new_note = 0x23;
            } else if (lResult >= 70 && lResult <= 110) {
                // 4CS
                new_note = 0x2b;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 60 && lResult <= 87) {
                // 3CF
                new_note = 0x13;
            } else if (lResult >= 138 && lResult <= 200) {
                // 3C
                new_note = 0x03;
            } else if (lResult >= 212 && lResult <= 232) {
                // 3CS
                new_note = 0x0b;
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
            } else if (lResult >= 130 && lResult <= 180) {
                // 5D
                new_note = 0x44;
            } else if (lResult >= 90 && lResult <= 120) {
                // 5DS
                new_note = 0x4c;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 203 && lResult <= 255) {
                // 4DF
                new_note = 0x34;
            } else if (lResult >= 130 && lResult <= 167) {
                // 4D
                new_note = 0x24;
            } else if (lResult >= 87 && lResult <= 124) {
                // 4DS
                new_note = 0x2c;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 60 && lResult <= 160) {
                // 3DF
                new_note = 0x14;
            } else if (lResult >= 145 && lResult <= 205) {
                // 3D
                new_note = 0x04;
            } else if (lResult >= 227 && lResult <= 239) {
                // 3DS
                new_note = 0x0c;
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
            if (lResult >= 8 && lResult <= 69) {
                // 5EF
                new_note = 0x55;
            } else if (lResult >= 96 && lResult <= 210) {
                // 5E
                new_note = 0x45;
            } else if (lResult >= 214 && lResult <= 227) {
                // 5ES
                new_note = 0x4d;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 217 && lResult <= 255) {
                // 4EF
                new_note = 0x35;
            } else if (lResult >= 154 && lResult <= 202) {
                // 4E
                new_note = 0x25;
            } else if (lResult >= 115 && lResult <= 145) {
                // 4ES
                new_note = 0x2d;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 227 && lResult <= 240) {
                // 3EF
                new_note = 0x15;
            } else if (lResult >= 180 && lResult <= 210) {
                // 3E
                new_note = 0x05;
            } else if (lResult >= 90 && lResult <= 165) {
                // 3ES
                new_note = 0x0d;
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
            if (lResult >= 148 && lResult <= 174) {
                // 5FF
                new_note = 0x56;
            } else if (lResult >= 89 && lResult <= 120) {
                // 5F
                new_note = 0x46;
            } else if (lResult >= 20 && lResult <= 72) {
                // 5FS
                new_note = 0x4e;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 223 && lResult <= 256) {
                // 4FF
                new_note = 0x36;
            } else if (lResult >= 165 && lResult <= 207) {
                // 4F
                new_note = 0x26;
            } else if (lResult >= 132 && lResult <= 160) {
                // 4FS
                new_note = 0x2e;
            } else {
               // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 148 && lResult <= 174) {
                // 3FF
                new_note = 0x16;
            } else if (lResult >= 89 && lResult <= 120) {
                // 3F
                new_note = 0x06;
            } else if (lResult >= 20 && lResult <= 72) {
                // 3FS
                new_note = 0x0e;
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
            if (lResult >= 87 && lResult <= 160) {
                // 5GF
                new_note = 0x57;
            } else if (lResult >= 175 && lResult <= 215) {
                // 5G
                new_note = 0x47;
            } else if (lResult >= 224 && lResult <= 228) {
                // 5GS
                new_note = 0x4f;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x02) {
            if (lResult >= 200 && lResult <= 229) {
                // 4GF
                new_note = 0x37;
            } else if (lResult >= 139 && lResult <= 192) {
                // 4G
                new_note = 0x27;
            } else if (lResult >= 107 && lResult <= 140) {
                // 4GS
                new_note = 0x2f;
            } else {
                // no note playing
                new_note = 0x00;
            }

        } else if (hResult == 0x03) {
            if (lResult >= 165 && lResult <= 190) {
                // 3GF
                new_note = 0x17;
            } else if (lResult >= 130 && lResult <= 160) {
                // 3G
                new_note = 0x07;
            } else if (lResult >= 107 && lResult <= 125) {
                // 3GS
                new_note = 0x0f;
            } else {
                // no note playing
                new_note = 0x00;
            }

        }

    }
    return new_note;
}

    //sets channel, plays note, toggles LEDs
    void getNoteTeachMode(unsigned char uNote){
        if (uNote == UA3){
            conv_result.result = UA3;
            conv_result.adc_channel = 0;
            play(UA3);
            ToggleLeds();
        }
        else if (uNote == UA3F){
            conv_result.result = UA3F;
            conv_result.adc_channel = 0;
            play(UA3F);
            ToggleLeds();
        }
        else if (uNote == UA3S){
            conv_result.result = UA3S;
            conv_result.adc_channel = 0;
            play(UA3S);
            ToggleLeds();
        }
        else if (uNote == UB3){
            conv_result.result = UB3;
            conv_result.adc_channel = 1;
            play(UB3);
            ToggleLeds();
        }
        else if (uNote == UB3F){
            conv_result.result = UB3F;
            conv_result.adc_channel = 1;
            play(UB3F);
            ToggleLeds();
        }
        else if (uNote == UB3S){
            conv_result.result = UB3S;
            conv_result.adc_channel = 1;
            play(UB3S);
            ToggleLeds();
        }
        else if (uNote == UC3){
            conv_result.result = UC3;
            conv_result.adc_channel = 2;
            play(UC3);
            ToggleLeds();
        }
        else if (uNote == UC3F){
            conv_result.result = UC3F;
            conv_result.adc_channel = 2;
            play(UC3F);
            ToggleLeds();
        }
        else if (uNote == UC3S){
            conv_result.result = UC3S;
            conv_result.adc_channel = 2;
            play(UC3S);
            ToggleLeds();
        }
        else if (uNote == UD3){
            conv_result.result = UD3;
            conv_result.adc_channel = 3;
            play(UD3);
            ToggleLeds();
        }
        else if (uNote == UD3F){
            conv_result.result = UD3F;
            conv_result.adc_channel = 3;
            play(UD3F);
            ToggleLeds();
        }
        else if (uNote == UD3S){
            conv_result.result = UD3S;
            conv_result.adc_channel = 3;
            play(UD3S);
            ToggleLeds();
        }
        else if (uNote == UE3){
            conv_result.result = UE3;
            conv_result.adc_channel = 4;
            play(UE3);
            ToggleLeds();
        }
        else if (uNote == UE3F){
            conv_result.result = UE3F;
            conv_result.adc_channel = 4;
            play(UE3F);
            ToggleLeds();
        }
        else if (uNote == UE3S){
            conv_result.result = UE3S;
            conv_result.adc_channel = 4;
            play(UE3S);
            ToggleLeds();
        }
        else if (uNote == UF3){
            conv_result.result = UF3;
            conv_result.adc_channel = 5;
            play(UF3);
            ToggleLeds();
        }
        else if (uNote == UF3F){
            conv_result.result = UF3F;
            conv_result.adc_channel = 5;
            play(UF3F);
            ToggleLeds();
        }
        else if (uNote == UF3S){
            conv_result.result = UF3S;
            conv_result.adc_channel = 5;
            play(UF3S);
            ToggleLeds();
        }
        else if (uNote == UG3){
            conv_result.result = UG3;
            conv_result.adc_channel = 6;
            play(UG3);
            ToggleLeds();
        }
        else if (uNote == UG3F){
            conv_result.result = UG3F;
            conv_result.adc_channel = 6;
            play(UG3F);
            ToggleLeds();
        }
        else if (uNote == UG3S){
            conv_result.result = UG3S;
            conv_result.adc_channel = 6;
            play(UG3S);
            ToggleLeds();
        }
        else if (uNote == UA4){
            conv_result.result = UA4;
            conv_result.adc_channel = 0;
            play(UA4);
            ToggleLeds();
        }
        else if (uNote == UA4F){
            conv_result.result = UA4F;
            conv_result.adc_channel = 0;
            play(UA4F);
            ToggleLeds();
        }
        else if (uNote == UA4S){
            conv_result.result = UA4S;
            conv_result.adc_channel = 0;
            play(UA4S);
            ToggleLeds();
        }
        else if (uNote == UB4){
            conv_result.result = UB4;
            conv_result.adc_channel = 1;
            play(UB4);
            ToggleLeds();
        }
        else if (uNote == UB4F){
            conv_result.result = UB4F;
            conv_result.adc_channel = 1;
            play(UB4F);
            ToggleLeds();
        }
        else if (uNote == UB4S){
            conv_result.result = UB4S;
            conv_result.adc_channel = 1;
            play(UB4S);
            ToggleLeds();
        }
        else if (uNote == UC4){
            conv_result.result = UC4;
            conv_result.adc_channel = 2;
            play(UC4);
            ToggleLeds();
        }
        else if (uNote == UC4F){
            conv_result.result = UC4F;
            conv_result.adc_channel = 2;
            play(UC4F);
            ToggleLeds();
        }
        else if (uNote == UC4S){
            conv_result.result = UC4S;
            conv_result.adc_channel = 2;
            play(UC4S);
            ToggleLeds();
        }
        else if (uNote == UD4){
            conv_result.result = UD4;
            conv_result.adc_channel = 3;
            play(UD4);
            ToggleLeds();
        }
        else if (uNote == UD4F){
            conv_result.result = UD4F;
            conv_result.adc_channel = 3;
            play(UD4F);
            ToggleLeds();
        }
        else if (uNote == UD4S){
            conv_result.result = UD4S;
            conv_result.adc_channel = 3;
            play(UD4S);
            ToggleLeds();
        }
        else if (uNote == UE4){
            conv_result.result = UE4;
            conv_result.adc_channel = 4;
            play(UE4);
            ToggleLeds();
        }
        else if (uNote == UE4F){
            conv_result.result = UE4F;
            conv_result.adc_channel = 4;
            play(UE4F);
            ToggleLeds();
        }
        else if (uNote == UE4S){
            conv_result.result = UE4S;
            conv_result.adc_channel = 4;
            play(UE4S);
            ToggleLeds();
        }
        else if (uNote == UF4){
            conv_result.result = UF4;
            conv_result.adc_channel = 5;
            play(UF4);
            ToggleLeds();
        }
        else if (uNote == UF4F){
            conv_result.result = UF4F;
            conv_result.adc_channel = 5;
            play(UF4F);
            ToggleLeds();
        }
        else if (uNote == UF4S){
            conv_result.result = UF4S;
            conv_result.adc_channel = 5;
            play(UF4S);
            ToggleLeds();
        }
        else if (uNote == UG4){
            conv_result.result = UG4;
            conv_result.adc_channel = 6;
            play(UG4);
            ToggleLeds();
        }
        else if (uNote == UG4F){
            conv_result.result = UG4F;
            conv_result.adc_channel = 6;
            play(UG4F);
            ToggleLeds();
        }
        else if (uNote == UG4S){
            conv_result.result = UG4S;
            conv_result.adc_channel = 6;
            play(UG4S);
            ToggleLeds();
        }
        else if (uNote == UA5){
            conv_result.result = UA5;
            conv_result.adc_channel = 0;
            play(UA5);
            ToggleLeds();
        }
        else if (uNote == UA5F){
            conv_result.result = UA5F;
            conv_result.adc_channel = 0;
            play(UA5F);
            ToggleLeds();
        }
        else if (uNote == UA5S){
            conv_result.result = UA5S;
            conv_result.adc_channel = 0;
            play(UA5S);
            ToggleLeds();
        }
        else if (uNote == UB5){
            conv_result.result = UB5;
            conv_result.adc_channel = 1;
            play(UB5);
            ToggleLeds();
        }
        else if (uNote == UB5F){
            conv_result.result = UB5F;
            conv_result.adc_channel = 1;
            play(UB5F);
            ToggleLeds();
        }
        else if (uNote == UB5S){
            conv_result.result = UB5S;
            conv_result.adc_channel = 1;
            play(UB5S);
            ToggleLeds();
        }
        else if (uNote == UC5){
            conv_result.result = UC5;
            conv_result.adc_channel = 2;
            play(UC5);
            ToggleLeds();
        }
        else if (uNote == UC5F){
            conv_result.result = UC5F;
            conv_result.adc_channel = 2;
            play(UC5F);
            ToggleLeds();
        }
        else if (uNote == UC5S){
            conv_result.result = UC5S;
            conv_result.adc_channel = 2;
            play(UC5S);
            ToggleLeds();
        }
        else if (uNote == UD5){
            conv_result.result = UD5;
            conv_result.adc_channel = 3;
            play(UD5);
            ToggleLeds();
        }
        else if (uNote == UD5F){
            conv_result.result = UD5F;
            conv_result.adc_channel = 3;
            play(UD5F);
            ToggleLeds();
        }
        else if (uNote == UD5S){
            conv_result.result = UD5S;
            conv_result.adc_channel = 3;
            play(UD5S);
            ToggleLeds();
        }
        else if (uNote == UE5){
            conv_result.result = UE5;
            conv_result.adc_channel = 4;
            play(UE5);
            ToggleLeds();
        }
        else if (uNote == UE5F){
            conv_result.result = UE5F;
            conv_result.adc_channel = 4;
            play(UE5F);
            ToggleLeds();
        }
        else if (uNote == UE5S){
            conv_result.result = UE5S;
            conv_result.adc_channel = 4;
            play(UE5S);
            ToggleLeds();
        }
        else if (uNote == UF5){
            conv_result.result = UF5;
            conv_result.adc_channel = 5;
            play(UF5);
            ToggleLeds();
        }
        else if (uNote == UF5F){
            conv_result.result = UF5F;
            conv_result.adc_channel = 5;
            play(UF5F);
            ToggleLeds();
        }
        else if (uNote == UF5S){
            conv_result.result = UF5S;
            conv_result.adc_channel = 5;
            play(UF5S);
            ToggleLeds();
        }
        else if (uNote == UG5){
            conv_result.result = UG5;
            conv_result.adc_channel = 6;
            play(UG5);
            ToggleLeds();
        }
        else if (uNote == UG5F){
            conv_result.result = UG5F;
            conv_result.adc_channel = 6;
            play(UG5F);
            ToggleLeds();
        }
        else if (uNote == UG5S){
            conv_result.result = UG5S;
            conv_result.adc_channel = 6;
            play(UG5S);
            ToggleLeds();
        }

    }

    //playing Piano Man by Billy Joel
    void playPianoMan(){
        //verse
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UF4);
        DelayMs(dqn);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UC4);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(hn);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(en);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC4);
        DelayMs(en);
        getNoteTeachMode(UD4);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UD4);
        DelayMs(wn);
        getNoteTeachMode(0);
        DelayMs(qn);
        getNoteTeachMode(UG4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG4);
      DelayMs(wn);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UF4);
        DelayMs(qn);
        getNoteTeachMode(UE4);
        DelayMs(en);
        getNoteTeachMode(UC4);
        DelayMs(en);
        DelayMs(dhn);
        getNoteTeachMode(UA3);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA3);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA3);
      DelayMs(hn);
        getNoteTeachMode(UF4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF4);
      DelayMs(qn);
        getNoteTeachMode(UE4);
        DelayMs(qn);
        getNoteTeachMode(UC4);
        DelayMs(qn);
        getNoteTeachMode(UC4);
        DelayMs(dhn);
        play(0);
        DelayMs(wn);
        DelayMs(hn);
        DelayMs(dqn);
        getNoteTeachMode(UG5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(wn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(hn);
        getNoteTeachMode(UD5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(wn);
        getNoteTeachMode(0);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(hn);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UE5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UE5);
      DelayMs(hn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(wn);
        DelayMs(hn);
        //chorus
        //"Da-da's"
        getNoteTeachMode(UA4);
        DelayMs(hn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(dhn);
        getNoteTeachMode(UB4);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4);
        DelayMs(en);
        getNoteTeachMode(UA4);
        DelayMs(hn);
        DelayMs(en);
        getNoteTeachMode(0);
        DelayMs(wn);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UA4);
      DelayMs(en);
        DelayMs(hn);
        getNoteTeachMode(UB4);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4);
        DelayMs(wn);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(en);
        getNoteTeachMode(UG4);
        DelayMs(en);
        DelayMs(dhn);
        getNoteTeachMode(0);
        DelayMs(wn);
        DelayMs(wn);
        //"Sing us a song, you're the piano man..."
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(dqn);
        DelayMs(dhn);
        getNoteTeachMode(UA4);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(dqn);
        getNoteTeachMode(0);
        DelayMs(br);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(hn);
        getNoteTeachMode(UD5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UD5);
      DelayMs(wn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UG5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UG5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(dqn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF5);
      DelayMs(en);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UF5);
      DelayMs(en);
        getNoteTeachMode(UE5);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(hn);
        getNoteTeachMode(0);
        DelayMs(hn);
        getNoteTeachMode(UG4);
        DelayMs(qn);
        getNoteTeachMode(UA4);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(qn);
        getNoteTeachMode(UF5);
        DelayMs(qn);
        getNoteTeachMode(UE5);
        DelayMs(qn);
        getNoteTeachMode(UC5);
        DelayMs(qn);
        getNoteTeachMode(0);
      DelayMs(br);
      getNoteTeachMode(UC5);
      DelayMs(wn);
        DelayMs(hn);
    }

   void playClocks(){
        while(1){
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UG4);
            DelayMs(en);
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UG4);
            DelayMs(en);
            getNoteTeachMode(UE5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UD5F);
            DelayMs(en);
            getNoteTeachMode(UB4F);
            DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
            getNoteTeachMode(UF4);
            DelayMs(en);
            getNoteTeachMode(UC5);
            DelayMs(en);
            getNoteTeachMode(UA4F);
            DelayMs(en);
        }
}

    void playClair(){
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UA5F);
        DelayMs(en);
        DelayMs(dqn);
        getNoteTeachMode(UF5);
        DelayMs(hn);
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(en);
        getNoteTeachMode(UE5F);
        DelayMs(dhn);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en);
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en);
        getNoteTeachMode(UF5);
        DelayMs(den);
        getNoteTeachMode(UD5F);
        DelayMs(den);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UD5F);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(dhn);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UC5);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UE5F);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UB4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(qn);
        getNoteTeachMode(UG4F);
        DelayMs(en);
        getNoteTeachMode(UA4F);
        DelayMs(en);
        getNoteTeachMode(UG4F);
        DelayMs(dqn);

    }