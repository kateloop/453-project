`timescale 1ns / 1ps

module audio (system_clock, reset, left_in_data, left_out_data, right_in_data, right_out_data, ready,
	      audio_reset_b, ac97_sdata_out, ac97_sdata_in,
         ac97_synch, ac97_bit_clock 
			);

   input system_clock;
   input reset; 
	output [19:0] left_in_data, right_in_data;
   input [19:0] left_out_data, right_out_data;
   output ready;

   //ac97 interface signals
   output audio_reset_b;
   output ac97_sdata_out;
   input ac97_sdata_in;
   output ac97_synch;
   input ac97_bit_clock;
	
   wire [7:0] command_address;
   wire [15:0] command_data;
   wire command_valid;

   reg audio_reset_b;
   reg [9:0] reset_count;
	
	//headphone volume
	wire [4:0] volume_hp;
   assign volume_hp = 4'd22;

   //wait a little before enabling the AC97 codec
   always @(posedge system_clock) begin
      if (reset) begin
         audio_reset_b = 1'b0;
         reset_count = 0;
      end else if (reset_count == 1023)
        audio_reset_b = 1'b1;
      else
        reset_count = reset_count+1;
   end

   ac97 ac97(ready, command_address, command_data, command_valid,
             left_out_data, left_valid,
             right_out_data, right_valid, 
				 left_in_data, right_in_data, 
				 ac97_sdata_out, ac97_sdata_in, ac97_synch, ac97_bit_clock, reset
				 );

   ac97commands cmds(system_clock, ready, command_address, command_data,
                     command_valid, volume_hp);

   //Call the data good
	assign left_valid = 1;
	assign right_valid = 1;

endmodule

// assemble/disassemble AC97 serial frames
module ac97 (ready,
             command_address, command_data, command_valid,
             left_data, left_valid,
             right_data, right_valid,
             left_in_data, right_in_data,
             ac97_sdata_out, ac97_sdata_in, ac97_synch, ac97_bit_clock, reset
				 );

   output ready;
   input [7:0] command_address;
   input [15:0] command_data;
   input command_valid;
   input [19:0] left_data, right_data;
   input left_valid, right_valid;
   output [19:0] left_in_data, right_in_data;

   input ac97_sdata_in;
   input ac97_bit_clock, reset;
   output ac97_sdata_out;
   output ac97_synch;
	
   reg ready;

   reg ac97_sdata_out;
   reg ac97_synch;

   reg [7:0] bit_count;

   reg [19:0] l_cmd_addr;
   reg [19:0] l_cmd_data;
   reg [19:0] l_left_data, l_right_data;
   reg l_cmd_v, l_left_v, l_right_v;
   reg [19:0] left_in_data, right_in_data;



   always @(posedge ac97_bit_clock) begin
      // Generate the sync signal
		if (reset)
		begin
		   ready <= 1'b0;
	      // synthesis attribute init of ready is "0";
	      ac97_sdata_out <= 1'b0;
	      // synthesis attribute init of ac97_sdata_out is "0";
	      ac97_synch <= 1'b0;
	      // synthesis attribute init of ac97_synch is "0";

	      bit_count <= 8'h00;
	      // synthesis attribute init of bit_count is "0000";
	      l_cmd_v <= 1'b0;
	      // synthesis attribute init of l_cmd_v is "0";
	      l_left_v <= 1'b0;
	      // synthesis attribute init of l_left_v is "0";
	      l_right_v <= 1'b0;
	      // synthesis attribute init of l_right_v is "0";
		end
		else
			begin
	      if (bit_count == 255)
	        ac97_synch <= 1'b1;
	      if (bit_count == 15)
	        ac97_synch <= 1'b0;

	      // Generate the ready signal
	      if (bit_count == 128)
	        ready <= 1'b1;
	      if (bit_count == 2)
	        ready <= 1'b0;

	      // Latch user data at the end of each frame. This ensures that the
	      // first frame after reset will be empty.
	      if (bit_count == 255)
	        begin
	           l_cmd_addr <= {command_address, 12'h000};
	           l_cmd_data <= {command_data, 4'h0};
	           l_cmd_v <= command_valid;
	           l_left_data <= left_data;
	           l_left_v <= left_valid;
	           l_right_data <= right_data;
	           l_right_v <= right_valid;
	        end

	      if ((bit_count >= 0) && (bit_count <= 15))
	        // Slot 0: Tags
	        case (bit_count[3:0])
	          4'h0: ac97_sdata_out <= 1'b1;      // Frame valid
	          4'h1: ac97_sdata_out <= l_cmd_v;   // Command address valid
	          4'h2: ac97_sdata_out <= l_cmd_v;   // Command data valid
	          4'h3: ac97_sdata_out <= l_left_v;  // Left data valid
		  4'h4: ac97_sdata_out <= l_right_v; // Right data valid
	          default: ac97_sdata_out <= 1'b0;
	        endcase

	      else if ((bit_count >= 16) && (bit_count <= 35))
	        // Slot 1: Command address (8-bits, left justified)
	        ac97_sdata_out <= l_cmd_v ? l_cmd_addr[35-bit_count] : 1'b0;

	      else if ((bit_count >= 36) && (bit_count <= 55))
	        // Slot 2: Command data (16-bits, left justified)
	        ac97_sdata_out <= l_cmd_v ? l_cmd_data[55-bit_count] : 1'b0;

	      else if ((bit_count >= 56) && (bit_count <= 75))
	        begin
	           // Slot 3: Left channel
	           ac97_sdata_out <= l_left_v ? l_left_data[19] : 1'b0;
	           l_left_data <= { l_left_data[18:0], l_left_data[19] };
	        end
	      else if ((bit_count >= 76) && (bit_count <= 95))
	        // Slot 4: Right channel
	           ac97_sdata_out <= l_right_v ? l_right_data[95-bit_count] : 1'b0;
	      else
	        ac97_sdata_out <= 1'b0;

	      bit_count <= bit_count+1;
		end
   end // always @ (posedge ac97_bit_clock)

   always @(negedge ac97_bit_clock) begin
      if ((bit_count >= 57) && (bit_count <= 76))
        // Slot 3: Left channel
        left_in_data <= { left_in_data[18:0], ac97_sdata_in };
      else if ((bit_count >= 77) && (bit_count <= 96))
        // Slot 4: Right channel
        right_in_data <= { right_in_data[18:0], ac97_sdata_in };
   end

endmodule

// issue initialization commands to AC97
module ac97commands (clock, ready, command_address, command_data,
                     command_valid, volume);

   input clock;
   input ready;
   output [7:0] command_address;
   output [15:0] command_data;
   output command_valid;
	input [4:0] volume;

   reg [23:0] command = 0;
   reg command_valid = 0;

   reg old_ready = 0;
   reg done = 0;
   reg [3:0] state = 0;
   assign command_address = command[23:16];
   assign command_data = command[15:0];

   wire [4:0] vol;
   assign vol = 31-volume;

   always @(posedge clock) begin
      if (ready && (!old_ready))
        state <= state+1;

      case (state)
        0: // Read ID
          begin
             command <= 24'h80_0000;
             command_valid <= 1'b1;
          end
        1: // Read ID
          command <= 24'h80_0000;
		  2: // master volume
          command <= { 8'h02, 16'b0000100000001000};
        3: // headphone volume
          command <= { 8'h04, 3'b000, vol, 3'b000, vol };
		  4: // line in volume
			 command <= 24'h10_0000;
        5: // PCM volume
          command <= 24'h18_0000;
        6: // Record source select
          //command <= {8'h1A, 'h0, select, 'h0, select };
			 command <= 24'h1A_0000;
        7: // Record gain = max
	  		command <= 24'h1C_0F0F;
        9: // set +20db mic gain
          command <= 24'h0E_8048;
        10: // Set beep volume
          command <= 24'h0A_0000;
        11: // General purpose register
          command <= { 8'h20, 16'b0000000000000000};
        default:
          command <= 24'h80_0000;
      endcase // case(state)

      old_ready <= ready;

   end

endmodule