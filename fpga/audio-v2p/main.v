`timescale 1ns / 1ps

module main(
// inputs
SYSTEM_CLOCK,
ac97_bit_clock,	// Generated by AC'97
ac97_sdata_in,		// Serial data from AC'97
// outputs
ac97_sdata_out,	// Serial data to AC'97
ac97_synch,			// Defines boundries of AC'97 frames, controls warm reset
audio_reset_b,		// AC'97 codec cold reset
LED,
SACNT1
);

input SYSTEM_CLOCK, ac97_bit_clock, ac97_sdata_in;
output ac97_sdata_out, ac97_synch, audio_reset_b;
output [7:0] LED; 
output [31:0] SACNT1;

assign SACNT1 = 8'h0000000D;
assign SRCCR  = 8'h00007000;

wire [19:0] left_in_data, right_in_data, left_out_data, right_out_data;

// instantiate the clock generation module
CLOCK_GEN CLOCK_GEN 
(
SYSTEM_CLOCK,
system_clock_buffered,
reset
);

// Audio module
audio audio0 (
      // Inputs
      .system_clock(system_clock_buffered), 
      .reset(reset), 
      .left_out_data(left_out_data), 
      .right_out_data(right_out_data), 
      .ac97_bit_clock(ac97_bit_clock),
      // Outputs
      .left_in_data(left_in_data),
      .right_in_data(right_in_data),
      .ready(ready),
	   .audio_reset_b(audio_reset_b), 
      .ac97_sdata_out(ac97_sdata_out), 
      .ac97_sdata_in(ac97_sdata_in),
      .ac97_synch(ac97_synch)  
		);

// Processing module: takes input, does something, sends output				  
processing processing0 (
      // inputs
      .clock(system_clock_buffered), 
      .ready(ready), 
      .left_in(left_in_data), 
      .right_in(right_in_data), 
      // outputs
      .left_out(left_out_data),
      .right_out(right_out_data), 
      .LED(LED));

endmodule