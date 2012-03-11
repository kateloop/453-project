/* shift_reg.v - basic shift register */
module shift_reg(
                 in,            // Input
                 out,           // Output
                 clk            // Clock
                 );
   // Inputs
   input in;
   input clk;

   // Outputs
   output reg out;

   // Parameters
   parameter SIZE = 3;

   // Internal
   reg    shift_reg [SIZE-1:0];

   // Counters
   integer i;

   // Register Clocking
   always @(posedge clk) begin
      // Pass down old values
      for (i = 0; i < SIZE - 1; i = i + 1) begin
         shift_reg[i] <= shift_reg[i+1];
      end

      // Pipe in new
      shift_reg[SIZE-1] <= in;

      // Pipe out old if value stable
      out <= (shift_reg[0] ^ shift_reg[1]) ? out : shift_reg[0];
   end
endmodule

   
   
