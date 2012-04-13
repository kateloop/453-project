/****************************************************************************
 *
 * Copyright (c) 2012, The University of Wisconsin - Madison
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of The University of Wisconsin - Madison nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERISTY OF
 * WISCONSIN - MADISON BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Christopher Karle <ckarle@wisc.edu>
 *
 ****************************************************************************/
#include <stdio.h>
#include <uart.h>
#include "regaddr.h"
#include "noteFreq.h"


// Notes

#define O3BS	0
#define O3C	1	
#define O3CS	3
#define O3DF	4
#define O3D	5
#define O3DS	6
#define O3EF	7
#define O3E	8
#define O3FF	9
#define O3F	10
#define O3FS	11
#define O3GF	12
#define O3G	13
#define O3GS	14
#define O3AF	15
#define O3A	16
#define O3AS	17
#define O3BF	18
#define O3B	19
#define O3CF	20
#define O4BS    21
#define O4C     22     
#define O4CS    23
#define O4DF    24
#define O4D     25
#define O4DS    26
#define O4EF    27
#define O4E     28
#define O4FF    29
#define O4F     30
#define O4FS    31
#define O4GF    32
#define O4G     33
#define O4GS    34
#define O4AF    35
#define O4A     36
#define O4AS    37
#define O4BF    38
#define O4B     39
#define O4CF    40
#define O5BS    41
#define O5C     42      
#define O5CS    43
#define O5DF    44
#define O5D     45
#define O5DS    46
#define O5EF    47
#define O5E     48
#define O5FF    49
#define O5F     50
#define O5FS    51
#define O5GF    52
#define O5G     53
#define O5GS    54
#define O5AF    55
#define O5A     56
#define O5AS    57
#define O5BF    58
#define O5B     59
#define O5CF    60

// UART Defines
#define UC 0b1000000000000
#define UD 0b0100000000000
#define UE 0b0010000000000
#define UF 0b0001000000000
#define UG 0b0000100000000
#define UA 0b0000010000000
#define UB 0b0000001000000
#define UL3 0b0000000100000
#define UR3 0b0000000010000
#define UL4 0b0000000001000
#define UR4 0b0000000000100
#define UL5 0b0000000000010
#define UR5 0b0000000000001

#define SINE_LUT_LEN 17
int SINE_LUT[SINE_LUT_LEN] = { 0, 6424, 12785, 19024, 25079, 30893, 36409, 41575, 46340, 50659, 54490, 57797, 60546, 62713, 64276, 65219, 65535};

// variables
int freq;

// <C> <D> <E> <F> <G> <A> <B> <L3> <R3> <L4> <R4> <L5> <R5>
int uart_out;
int uart_in;

/***************************************************************************
 * plays note
 **************************************************************************/
void play (int note) 
{
	uart_out = 0b0000000000000;
	switch (note) {
		//////////////////////
		// Octave 3
		////////////////////// 
		case O3BS :
			freq = B3SHARP;
			// B, R3
			uart_out |= UB;
			uart_out |= UR3;
			break;
		case O3C :
			freq = C3;
			// C, L3, R3
			uart_out |= UC;
			uart_out |= UL3;
			uart_out |= UR3;
			break;
		case O3CS :
			freq = C3SHARP;
			// C, R3
			uart_out |= UC;
			uart_out |= UR3;
			break;
		case O3DF :
			freq = D3FLAT;
			// D, L3
			uart_out |= UD;
			uart_out |= UL3;
			break;
		case O3D :
			freq = D3;
			// D, L3, R3
			uart_out |= UD;
			uart_out |= UL3;
			uart_out |= UR3;
			break;
		case O3DS :
			freq = D3SHARP;
			// D, 3R
			uart_out |= UD;
			uart_out |= UR3;
			break;
		case O3EF :
			freq = E3FLAT;
			// E, L3
			uart_out |= UE;
			uart_out |= UL3;
			break;
		case O3E :
			freq = E3;
			// E, L3, R3
			uart_out |= UE;
			uart_out |= UL3;
			uart_out |= UR3;
			break;
		case O3FF :
			freq = F3FLAT;
			// F, L3
			uart_out |= UF;
			uart_out |= UL3;
			break;
		case O3F :
			freq = F3;
			// F, L3, R3
			uart_out |= UF;
			uart_out |= UL3;
			uart_out |= UR3;
			break;
		case O3FS :
			freq = F3SHARP;
			// F, R3
			uart_out |= UF;
			uart_out |= UR3;
			break;	
		case O3GF :
			freq = G3FLAT;
			// G, L3
			uart_out |= UG;
			uart_out |= UL3;
			break;
		case O3G :
			freq = G3;
			// G, L3, R3
			uart_out |= UG;
			uart_out |= UL3;
			uart_out |= UR3;
			break;
		case O3GS :
			freq = G3SHARP;
			// G, R3
			uart_out |= UG;
			uart_out |= UR3;
			break;
		case O3AF :
			freq = A3FLAT;
			// A, L3
			uart_out |= UA;
			uart_out |= UL3;
			break;
		case O3A :
			freq = A3;
			break;
		case O3AS :
			freq = A3SHARP;
			break;
		case O3BF :
			freq = B3FLAT;
			break;
		case O3B :
			freq = B3;
			break;
		case O3CF :
			freq = C3FLAT;
			break;
		///////////////////////////////
		// Octave 4
		///////////////////////////////
		case O4BS :
			freq = B4SHARP;
			break;
		case O4C :
			freq = C4;
			break;
		case O4CS :
			freq = C4SHARP;
			break;
		case O4DF :
			freq = D4FLAT;
			break;
		case O4D :
			freq = D4;
			break;
		case O4DS :
			freq = D4SHARP;
			break;
		case O4EF :
			freq = E4FLAT;
			break;
		case O4E :
			freq = E4;
			break;
		case O4FF :
			freq = F4FLAT;
			break;
		case O4F :
			freq = F4;
			break;
		case O4FS :
			freq = F4SHARP;
			break;
		case O4GF :
			freq = G4FLAT;
			break;
		case O4G :
			freq = G4;
			break;
		case O4GS :
			freq = G4SHARP;
			break;
		case O4AF :
			freq = A4FLAT;
			break;
		case O4A :
			freq = A4;
			break;
		case O4AS :
			freq = A4SHARP;
			break;
		case O4BF :
			freq = B4FLAT;
			break;
		case O4B :
			freq = B4;
			break;
		case O4CF : 
			freq = C4FLAT;
			break;
		////////////////////////////////
		// Octave 5
		//////////////////////////////// 
		case O5BS :
			freq = B5SHARP;
			break;
		case O5C :
			freq = C5;
			break;
		case O5CS :
			freq = C5SHARP;
			break;
		case O5DF :
			freq = D5FLAT;
			break;
		case O5D :
			freq = D5;
			break;
		case O5DS :
			freq = D5SHARP;
			break;
		case O5EF :
			freq = E5FLAT;
			break;
		case O5E :
			freq = E5;
			break;
		case O5FF :
			freq = F5FLAT;
			break;
		case O5F :
			freq = F5;
			break;
		case O5FS :
			freq = F5SHARP;
			break;
		case O5GF :
			freq = G5FLAT;
			break;
		case O5G :
			freq = G5;
			break;
		case O5GS :
			freq = G5SHARP;
			break;
		case O5AF :
			freq = A5FLAT;
			break;
		case O5A :
			freq = A5;
			break;
		case O5AS :
			freq = A5SHARP;
			break;
		case O5BF :
			freq = B5FLAT;
			break;
		case O5B :
			freq = B5;
			break;
		case O5CF :
			freq = C5FLAT;
			break;	
	}

}


/****************************************************************************
 * main function
 ****************************************************************************/
void _main(void* parameter)
{
   printf("TODO");
}
