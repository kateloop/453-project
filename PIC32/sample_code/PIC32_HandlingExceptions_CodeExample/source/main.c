/*********************************************************************
 * FileName:        main.c
 * Dependencies:
 *
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 * Author:			Darren Wenn
 * Notes:			Demonstrates use of an exception handler
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PIC32/PIC24 Microcontroller is intended
 * and supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PIC32/PIC24 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 ********************************************************************/
 
/********************************************************************
 *
 * Example of a generic exception handler
 * Author: Darren Wenn, Microchip Technology
 * Date: 15-Jan-2008
 * 
 * The main.c has a two possible exceptions that it may cause.
 * The exception.c file will handle all general exceptions and provides code to help
 * in analysing the source of the exception (exception cause and address).
 *
 * OUTPUT
 * The following line should produce Value "0x00000005 == EXCEP_AdES" in variable _excep_code
 * 		*illegal_address = 10; 
 *
 * The following line should produce Value "0x0000000D == EXCEP_Trap" in variable _excep_code
 * 		division_by_zero = 2 / zero_value;
 * 
 ********************************************************************/


#include <p32xxxx.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_2

int main(void)
{
	int* illegal_address;
	int division_by_zero;
	int zero_value = 0;
	
	//illegal_address = (int*) 0xFFFFFFFF;
	*illegal_address = 10;
	//division_by_zero = 2 / zero_value;
	
	return 0;	
}