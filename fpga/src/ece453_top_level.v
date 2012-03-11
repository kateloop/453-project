`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 		UW-Madison, ECE 453
// Engineer: 		M.G.Morrow
// 
// Create Date:    	09:19:05 03/27/2008 
// Design Name: 	ECE453 TLL5000 master project
// Module Name:    	ece453_top_level 
// Description: 	Top-level module for use with TLL5000 board
//					Do NOT make any changes to this module
//					Assumes TLL6219 on mezzanine B and TLL7D4820m on mezzanine A
//
// Revision: 
// Revision 0.01 - File Created
//
//////////////////////////////////////////////////////////////////////////////////
module ece453_top_level(
	// system reset and clocks
	input 	SYS_RST_N,	
	input 	SYS_CLK, FPGA_CLK_BANK_0, FPGA_CLK_BANK_1, FPGA_CLK_BANK_5, // 24MHz clocks
	input 	FPGA_CLK1, FPGA_CLK2, FPGA_CLK3, //100MHz clocks
	
	// RS-232 level translators
	input 	RS232_RX, RS232_RTS,		
	output	RS232_TX, RS232_CTS,
	// LEDs and switches
	output 	[7:0] LED,		
	input 	[7:0] DIP_SW,	
	input 	[4:0] PB,		
	// character LCD module
	output	LCD_BACK, LCD_E, LCD_RS, LCD_RW,
	output	[3:0] LCD_DATA,
	// PS2 mouse and keyboard
	output	PS2_ENABLED,
	inout	PS2_KEYB_CLK, PS2_KEYB_DATA, PS2_MOUSE_CLK, PS2_MOUSE_DATA,
	
	// AC97 codec
	input	AC97_BIT_CLK, AC97_SDATA_IN,
	output	AC97_SDATA_OUT, AC97_SYNC, AC97_RESET_N, AC97_ID_0, AC97_ID_1,

	// VGA output
	output	[7:0] VGA_B, VGA_G, VGA_R,
	output	VGA_HSYNC, VGA_VSYNC, VGA_PIX_CLK, VGA_BLANK, VGA_SYNC, VGA_PSAVE,
	
	// SPI to SD card socket
	input	SPI_MISO,		// MMC_I04
	output	SPI_MOSI,		// MMC_I00
	output	SPI_SCK, 		// MMC_I05
	output	SPI_SS, 		// MMC_I02
	
	// SDRAM (not included)
	// *********************************************************

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
	
	// i2c 
	inout	I2C_SCL, I2C_SDA,
	
	// video decoder
	input	[7:0] VD_DATA_DEC,
	input	VD_VS_FIELD, VD_HS, VD_INTRQ, VD_SFL, VD_LLC,
	output	VD_NRESET, VD_NPWRDWN,
	
	// Ethernet mac (not included)
	// *********************************************************
	
	// mezzanine A prototyping card
	inout	[67:0] MZA_PIO,
	output	MZA_DIR0, MZA_OE0,
	output	MZA_DIR8, MZA_OE8,
	output	MZA_DIR16, MZA_OE16,
	output	MZA_DIR24, MZA_OE24,
	output	MZA_DIR32, MZA_OE32,
	output	MZA_DIR40, MZA_OE40,

	// mezzanine B ARM926
    input	[23:0] MZB_A,
    inout	[31:0] MZB_D,
    input	MZB_CPLD_BE0_B, MZB_CPLD_BE1_B, MZB_CPLD_BE2_B, MZB_CPLD_BE3_B,
	input	MZB_CPLD_CS0_B, MZB_CPLD_CS1_B, MZB_CPLD_CS2_B, MZB_CPLD_CS3_B, MZB_CPLD_CS5_B,
	input	MZB_CPLD_OE_B, MZB_CPLD_RW, MZB_CPLD_RESET_OUT,
	input	MZB_CPLD_CLK0, MZB_CPLD_AS, MZB_CPLD_NFIO4, MZB_CPLD_NFIO5, 
    output	MZB_CPLD_DTACK, MZB_CPLD_IRQ,
	input	MZB_CPLD_RS1_B, MZB_CPLD_WS1_B, MZB_CPLD_RS5_B, MZB_CPLD_WS5_B, 

	// accessory connector
	output [42:9] ACC_PORT_PIN
);

	// prototyping board transceiver control logic
	wire	[47:0] PIO_IN, PIO_OUT;
	assign	PIO_IN = MZA_PIO[47:0]; 
	assign	MZA_PIO[ 7: 0] = (MZA_DIR0  == 1'b1)?PIO_OUT[ 7: 0]:8'bz;
	assign	MZA_PIO[15: 8] = (MZA_DIR8  == 1'b1)?PIO_OUT[15: 8]:8'bz;
	assign	MZA_PIO[23:16] = (MZA_DIR16 == 1'b1)?PIO_OUT[23:16]:8'bz;
	assign	MZA_PIO[31:24] = (MZA_DIR24 == 1'b1)?PIO_OUT[31:24]:8'bz;
	assign	MZA_PIO[39:32] = (MZA_DIR32 == 1'b1)?PIO_OUT[39:32]:8'bz;
	assign	MZA_PIO[47:40] = (MZA_DIR40 == 1'b1)?PIO_OUT[47:40]:8'bz;

	// instantiation of second level module
	ece453_master_module MASTER (
	.SYS_RST_N(SYS_RST_N),	
	.SYS_CLK(SYS_CLK), .FPGA_CLK_BANK_0(FPGA_CLK_BANK_0), .FPGA_CLK_BANK_1(FPGA_CLK_BANK_1), .FPGA_CLK_BANK_5(FPGA_CLK_BANK_5),
	.FPGA_CLK1(FPGA_CLK1), .FPGA_CLK2(FPGA_CLK2), .FPGA_CLK3(FPGA_CLK3),
	
	.RS232_RX(RS232_RX), .RS232_TX(RS232_TX), .RS232_RTS(RS232_RTS), .RS232_CTS(RS232_CTS),
	.LED(LED), .DIP_SW(DIP_SW),	.PB(PB),		
	.LCD_BACK(LCD_BACK), .LCD_E(LCD_E), .LCD_RS(LCD_RS), .LCD_RW(LCD_RW), .LCD_DATA(LCD_DATA),
	.PS2_ENABLED(PS2_ENABLED), .PS2_KEYB_CLK(PS2_KEYB_CLK), .PS2_KEYB_DATA(PS2_KEYB_DATA), 
	.PS2_MOUSE_CLK(PS2_MOUSE_CLK), .PS2_MOUSE_DATA(PS2_MOUSE_DATA),
	.AC97_BIT_CLK(AC97_BIT_CLK), .AC97_SDATA_IN(AC97_SDATA_IN), .AC97_SDATA_OUT(AC97_SDATA_OUT), 
	.AC97_SYNC(AC97_SYNC), .AC97_RESET_N(AC97_RESET_N), .AC97_ID_0(AC97_ID_0), .AC97_ID_1(AC97_ID_1),
	.VGA_B(VGA_B), .VGA_G(VGA_G), .VGA_R(VGA_R),
	.VGA_HSYNC(VGA_HSYNC), .VGA_VSYNC(VGA_VSYNC), .VGA_PIX_CLK(VGA_PIX_CLK), 
	.VGA_BLANK(VGA_BLANK), .VGA_SYNC(VGA_SYNC), .VGA_PSAVE(VGA_PSAVE),
	.SPI_MISO(SPI_MISO), .SPI_MOSI(SPI_MOSI), .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), 
	.FLASH_ADDR(FLASH_ADDR), .FLASH_DATA(FLASH_DATA), .FLASH_NBYTE(FLASH_NBYTE), .FLASH_NCE(FLASH_NCE), .FLASH_NOE_E(FLASH_NOE_E), 
	.FLASH_NRESET(FLASH_NRESET), .FLASH_NRYBY(FLASH_NRYBY), .FLASH_NWE(FLASH_NWE), .FLASH_NWP(FLASH_NWP),
	.VE_DATA(VE_DATA), .VE_TTX(VE_TTX), .VE_SCRESET_RTC(VE_SCRESET_RTC), .VE_RESET(VE_RESET), .VE_PAL_NTSC(VE_PAL_NTSC),
	.VE_CLK(VE_CLK), .VE_TTXREQ(VE_TTXREQ), .VE_CLAMP(VE_CLAMP), .VE_VSO(VE_VSO), .VE_CSO_HSO(VE_CSO_HSO),
	.VE_HSYNC(VE_HSYNC), .VE_FIELD_VSYNC(VE_FIELD_VSYNC), .VE_BLANK(VE_BLANK),
	.I2C_SCL(I2C_SCL), .I2C_SDA(I2C_SDA),
	.VD_DATA_DEC(VD_DATA_DEC), .VD_VS_FIELD(VD_VS_FIELD), .VD_HS(VD_HS), .VD_INTRQ(VD_INTRQ), 
	.VD_SFL(VD_SFL), .VD_LLC(VD_LLC), .VD_NRESET(VD_NRESET), .VD_NPWRDWN(VD_NPWRDWN),
	
	// mezzanine A prototyping card
	.PIO_IN(PIO_IN), .PIO_OUT(PIO_OUT), .PIO(MZA_PIO[67:48]),
	.PIO_DIR0(MZA_DIR0), .PIO_DIR8(MZA_DIR8), .PIO_DIR16(MZA_DIR16), 
	.PIO_DIR24(MZA_DIR24), .PIO_DIR32(MZA_DIR32), .PIO_DIR40(MZA_DIR40), 
	.PIO_OE_B0(MZA_OE0), .PIO_OE_B8(MZA_OE8), .PIO_OE_B16(MZA_OE16),
	.PIO_OE_B24(MZA_OE24), .PIO_OE_B32(MZA_OE32), .PIO_OE_B40(MZA_OE40), 

	// mezzanine B ARM926
    .ARM_A(MZB_A), .ARM_D(MZB_D),
    .ARM_BE_B({MZB_CPLD_BE3_B,MZB_CPLD_BE2_B,MZB_CPLD_BE1_B,MZB_CPLD_BE0_B}),
    .ARM_DTACK(MZB_CPLD_DTACK), .ARM_IRQ(MZB_CPLD_IRQ), 
	.ARM_CS0_B(MZB_CPLD_CS0_B), .ARM_CS1_B(MZB_CPLD_CS1_B), .ARM_CS2_B(MZB_CPLD_CS2_B), 
	.ARM_CS3_B(MZB_CPLD_CS3_B), .ARM_CS5_B(MZB_CPLD_CS5_B), 
	.ARM_OE_B(MZB_CPLD_OE_B), .ARM_RW(MZB_CPLD_RW), .ARM_RESET_OUT(MZB_CPLD_RESET_OUT), 
	.ARM_CLK0(MZB_CPLD_CLK0), .CPLD_AS(MZB_CPLD_AS), .CPLD_NFIO4(MZB_CPLD_NFIO4), .CPLD_NFIO5(MZB_CPLD_NFIO5),
	.CPLD_RS1_B(MZB_CPLD_RS1_B), .CPLD_WS1_B(MZB_CPLD_WS1_B), .CPLD_RS5_B(MZB_CPLD_RS5_B), .CPLD_WS5_B(MZB_CPLD_WS5_B),

	// accessory port connector
	.ACC_PORT_PIN(ACC_PORT_PIN)
	);
				
endmodule
