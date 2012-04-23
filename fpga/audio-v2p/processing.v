`timescale 1ns / 1ps

module processing(clock, ready, left_in, right_in, left_out, right_out);
   input clock;              // system clock

   input ready;                  // 1 when AC'97 is stable and ready
   input [19:0] left_in, right_in;		// PCM data in from AC'97
   output [19:0] left_out, right_out;	// PCM data out to AC'97

	reg [19:0] left_out;
	reg [19:0] right_out;
	
	
	// Produce annoying square wave
	reg [10:0] counter = 0;
	// square wave on left channel
	always @ (posedge clock) begin
		counter <= counter + 1;
		left_out[15] <= counter[10];
	end
	
	
	/*
	// send input back out
   always @ (posedge ready) begin
   		// just received new data from the AC97
	 		left_out <= left_in;
			right_out <= left_in;
   end
	*/

endmodule