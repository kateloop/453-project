`timescale 1ns / 1ps

module processing(
      // inputs
      clock, ready, 
      left_in, 
      right_in, 
      // Outputs
      left_out, 
      right_out, 
      LED);

   input clock;              // system clock
   input ready;                  // 1 when AC'97 is stable and ready
   input [19:0] left_in, right_in;		// PCM data in from AC'97

   output [19:0] left_out, right_out;	// PCM data out to AC'97
	output reg [7:0] LED;

	reg [19:0] left_out;
	reg [19:0] right_out;
	
	//assign right_out = left_out;
	//assign LED = 1'b1;

	
	// Produce annoying square wave
	reg [10:0] counter = 0;
	// square wave on left channel
	always @ (posedge clock) begin
		counter <= counter + 1;
		left_out[15] <= counter[10];
      if (counter == 1023)
		LED <= LED << 1;
		if (LED == 8'b0) LED <= 8'b1;
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