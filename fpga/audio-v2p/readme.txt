AC'97 Audio Codec Controller for Digilent XUP-V2P
-------------------------------------------------------------------------------

Description:

This project communicates with the AC'97 Audio Codec on the Digilent XUP-V2P
development board.  It implements codec configuration, audio input, and audio
output capabilities.

The AC'97 codec communicates with the FPGA using a serial protocol.  The FPGA
sends it a series of commands, which control volume, muting,  record source 
selection, etc.  It also sends and receives PCM encoded audio in this format.
The best resource for understanding this is the datasheet for the National
Semiconductor LM4550, which is the codec chip included on the XUP-V2P board. 
The AC'97 specification was developed by Intel, and complete documentation on
the protocol is available at http://www.intel.com/.  The LM4550 datasheet is 
available from http://www.national.com/ and is included for reference in the 
project zip file. 

The commands for adjusting the aforementioned parameters are located in the
ac97.v file, in the ac97commands module.  A complete discussion of these
parameters is out of the scope of this readme, and can be viewed in the LM4550
datasheet.  However, it is extremely important to note several things.  First,
values for volume are given as attenuations, so larger values result in lower
volume.  Second, in general, the uppermost bit of a volume command will mute 
the related channel.

As an example, the project includes a processing module (processing.v), which 
is configured to sample microphone input and output it on the PCM (wave) output 
channel by default.  Since the PCM channel is included in the output mix, the 
input from the microphone can be heard using the headphone output.

The processing module also contains commented code that will output a square
wave.  It is loud and obnoxious.


Included Files:

main.bit - programming file synthesized and implemented using ISE 8.1.02i
audio.ise - ISE 8.1i project file
AUDIO.ucf - constraints file
MAIN.v - Primary module
AC97.v - Contains audio interface module (audio), AC'97 serial commuication
	processor (ac97), and AC'97 command constructor (ac97commands)
PROCESSING.v - A module capable of receiving audio input and sending
	output.
CLOCK_GEN.v - clock divider module
LM4550.pdf - Datasheet for LM4550 AC'97 codec chip


Authors:

This project is based on the XUP-V2P Built in Self Test source code provided by 
Xilinx, Inc. It has been modified by Eric Gallimore and Nathaniel Smith under 
the supervision of Professor Mark Chang at Franklin W. Olin College of 
Engineering (http://www.olin.edu/).

It may contain elements from other sample souce code provided by Xilinx or
others.  If you feel that your work has been used here, please contact the
authors so that you can be credited.

The Xilinx copyright claims for the code provided in the Built in Self Test are 
as follows:
	
	XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
	SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
	XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
	AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
	OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
	IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
	AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
	FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
	WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
	IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
	REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
	INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE.

	(c) Copyright 2004 Xilinx, Inc.
	All rights reserved.


Revisions:

1.0	(2006-04-06)
	AC'97 Audio Codec Controller for Digilent XUP-V2P initial version. 
	Created by Eric Gallimore and Nathaniel Smith
	Licensed under the Creative Commons Attribution 2.5 License.

0.0	(2004-08-12)
	XUP-V2P Built in Self Test
	(c) Copyright 2004 Xilinx, Inc.
	All rights reserved.