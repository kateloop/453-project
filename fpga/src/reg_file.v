/* regFile.v - Basic register file */
module reg_file(address,
                data_in,
                data_out,
                ws_n,
                rs_n,
                be,
                clk,
                as,
                rst_n,
					 led_out
					 );
   // Inputs
   input [23:0] address;        // Arm address
   input [31:0] data_in;        // Data input
   input        ws_n;           // Write strobe
   input        rs_n;           // Read strobe
   input [3:0]  be;             // Byte enable
   input        clk;            // Clock
   input        as;             // Chip select
   input        rst_n;          // Reset

   // Outputs
   output reg [31:0] data_out;  // Data output
   output reg [7:0] led_out;	// LED output
   
   // Register file size
   parameter REG_FILE_SIZE      = 8; // Register file size
   parameter LOG_REG_FILE_SIZE  = 3; // Number of internal select bits

   // Internal regs
   reg [LOG_REG_FILE_SIZE - 1:0] select; // Address decoding   
   reg [31:0]                    registers [REG_FILE_SIZE - 1:0]; // The registers
   reg                           rw_state;                        // 1 if in read or write

   // Internal wires
   wire                          rs_en, ws_en;
   assign rs_en = ~rs_n && ~rw_state;
   assign ws_en = ~ws_n && ~rw_state;   
   
   // Address locations
   parameter A_RAND = 0;
   parameter LED = 4;

   // Address decode
   always @(address) begin
      case (address)
        A_RAND:  select = A_RAND;
        LED: select = LED;
      endcase
   end


   // PRNG
   function [31:0] next_random;
      input [31:0] random;
      next_random = (random >> 1) ^ (-(random & 1) & 32'h80200003);
   endfunction // if
      
   
   // Clocking registers
   always @(posedge clk or negedge rst_n) begin
      // Reset
      if (~rst_n) begin
         data_out <= 32'hfee1dead;
         rw_state <= 1'b0;
 	 registers[LED] <= 8'b1;
	 led_out <= 8'b0;
      end

      // Chip selected
      else if (as) begin
         // Read
         if (rs_en) begin
            // Set rw_state
            rw_state <= 1'b1;
            
            // Random
            if (select == A_RAND) begin
               data_out <= registers[select];
               registers[select] <= next_random(registers[select]);
            end
         end

         // Write
         else if (ws_en) begin
            // Set rw_state
            rw_state <= 1'b1;

            // Random
            if (select == A_RAND) begin
               registers[select] <= next_random(data_in);
            end
            else if (select == LED) begin
               led_out <= registers[select];
               registers[select] <= (registers[select] << 1'b1);
               if (registers[select] == 8'b10000000) registers[select] <= 8'b01;
            end
         end
      end // if (as)

      // Not selected - reset rw_cycle
      else begin
         rw_state <= 1'b0;
      end
   end 
endmodule // regFile

  
       
   
   
   
   
   
