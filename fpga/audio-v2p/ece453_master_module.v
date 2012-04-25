`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		UW-Madison, ECE 453
// Engineer: 		M.G.Morrow
// 
// Create Date:    	09:19:05 03/27/2008 
// Design Name: 	ECE453 TLL5000 master project
// Module Name:    	ece453_master_module 
// Description: 	Second-level module for use with TLL5000 board
//					All student code to be in this module or below
//					Do NOT alter the ports names or directions
//
// Revision: 
// Revision 0.01 - File Created
//
//////////////////////////////////////////////////////////////////////////////////
module ece453_master_module(
	// system reset and clocks
	input 	SYS_RST_N,	
	input 	SYS_CLK, FPGA_CLK_BANK_0, FPGA_CLK_BANK_1, FPGA_CLK_BANK_5, // 24MHz clocks
	input 	FPGA_CLK1, FPGA_CLK2, FPGA_CLK3, //100MHz clocks
	
	// RS-232 level translators
	input 	RS232_RX, RS232_RTS,		
	output	RS232_TX, RS232_CTS,
	// LEDs and switches
	output 	[7:0] LED,		
	inout 	[7:0] DIP_SW,	
	inout 	[4:0] PB,		
	// character LCD module
	output	LCD_BACK, LCD_E, LCD_RS, LCD_RW,
	output	[3:0] LCD_DATA,
	// PS2 mouse and keyboard
	output	PS2_ENABLED,
	inout	PS2_KEYB_CLK, PS2_KEYB_DATA, PS2_MOUSE_CLK, PS2_MOUSE_DATA,
	
	// AC97 codec
	input 	AC97_BIT_CLK, AC97_SDATA_IN,
	output	AC97_SDATA_OUT, AC97_SYNC, AC97_RESET_N, AC97_ID_0, AC97_ID_1,

	// VGA output
	output	[7:0] VGA_B, VGA_G, VGA_R,
	output	VGA_HSYNC, VGA_VSYNC, VGA_PIX_CLK, VGA_BLANK, VGA_SYNC, VGA_PSAVE,
	
	// SPI to SD card socket
	input	SPI_MISO,		// MMC_I04
	output	SPI_MOSI,		// MMC_I00
	output	SPI_SCK, 		// MMC_I05
	output	SPI_SS, 		// MMC_I02

	// flash memory
	output	[22:0] FLASH_ADDR,
	inout	[15:0] FLASH_DATA,
	output	FLASH_NBYTE, FLASH_NCE, FLASH_NOE_E, FLASH_NRESET, FLASH_NWE, FLASH_NWP,
	input	FLASH_NRYBY, 

	// video encoder
	output	[7:0] VE_DATA,
	output	VE_TTX, VE_SCRESET_RTC, VE_RESET, VE_PAL_NTSC,
	input	VE_CLK, VE_TTXREQ, VE_CLAMP, VE_VSO, VE_CSO_HSO,
	inout	VE_HSYNC, VE_FIELD_VSYNC, VE_BLANK,

	// i2c lines to mezzanines
	inout	I2C_SCL, I2C_SDA,

	// video decoder
	input	[7:0] VD_DATA_DEC,
	input	VD_VS_FIELD, VD_HS, VD_INTRQ, VD_SFL, VD_LLC,
	output	VD_NRESET, VD_NPWRDWN,
	
	// Ethernet mac (not included)
	// *********************************************************
	
	// TLL7D4820m prototyping card
	input	[47:0] PIO_IN,
	output	[47:0] PIO_OUT,
	output	PIO_DIR0, PIO_DIR8, PIO_DIR16, PIO_DIR24, PIO_DIR32, PIO_DIR40, 
	output	PIO_OE_B0, PIO_OE_B8, PIO_OE_B16, PIO_OE_B24, PIO_OE_B32, PIO_OE_B40, 
	inout	[67:48] PIO,

	// TLL6219 ARM926
    input	[23:0] ARM_A,
    inout	[31:0] ARM_D,
    input	[3:0] ARM_BE_B,
    output	ARM_DTACK, ARM_IRQ,//armIRQ is how to say that switch pressed
	input	ARM_CS0_B, ARM_CS1_B, ARM_CS2_B, ARM_CS3_B, ARM_CS5_B,
	input	ARM_OE_B, ARM_RW, ARM_RESET_OUT,
	input	ARM_CLK0, CPLD_AS, CPLD_NFIO4, CPLD_NFIO5, 
	input	CPLD_RS1_B, CPLD_WS1_B, CPLD_RS5_B, CPLD_WS5_B, 

	// accessory connector
	output [42:9] ACC_PORT_PIN
);
	// ***************************************************************************
	// place all use code below here *********************************************
	// the code below is only example code - edit as needed **********************
	
	/*
	assign ACC_PORT_PIN[9] = ARM_CLK0;
	assign ACC_PORT_PIN[10] = CPLD_RS5_B;
	assign ACC_PORT_PIN[11] = CPLD_WS5_B;
	assign ACC_PORT_PIN[15:12] = ARM_BE_B[3:0];
	assign ACC_PORT_PIN[20:17] = ARM_A[3:0];
	assign ACC_PORT_PIN[24:21] = ARM_D[3:0];
	assign ACC_PORT_PIN[16] = CPLD_AS;*/
	/*wire read0, write0;
	wire [31:0] counter;
	assign ACC_PORT_PIN[26:25] = {read0, write0};
	assign ACC_PORT_PIN[42:27] = counter;
	
*/
	 assign ARM_DTACK = 1'b1;
	 
	 main FPGAaudio(
				.SYSTEM_CLOCK(FPGA_CLK1),  //should this be the FPGA clock instead?
				.ac97_bit_clock(AC97_BIT_CLK),	
				.ac97_sdata_in(AC97_SDATA_IN),		
				.ac97_sdata_out(AC97_SDATA_OUT),	
				.ac97_synch(AC97_SYNC),			// Defines boundries of AC'97 frames, controls warm reset
				.audio_reset_b(AC97_RESET_N)		// AC'97 codec cold reset
		);
		
    /*assign ACC_PORT_PIN[9] = FPGA_CLK1;
    assign ACC_PORT_PIN[10] = CPLD_RS5_B;
    assign ACC_PORT_PIN[11] = CPLD_WS5_B;
    assign ACC_PORT_PIN[15:12] = ARM_BE_B[3:0];
    assign ACC_PORT_PIN[20:17] = ARM_A[3:0];
    assign ACC_PORT_PIN[24:21] = ARM_D[3:0];
    assign ACC_PORT_PIN[16] = CPLD_AS;*/
	 /*wire [3:0] BE1;
	 wire [23:0] addr1;
	 wire RS1;
	 wire WS1;
	 wire AS1;
	 wire [31:0] data1;*/
    
   endmodule
