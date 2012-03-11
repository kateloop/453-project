/* 
 * async_reg_file.v - Asynchronous register file with metastabiliy handling 
 *
 * Receiver has much higher clock rate.  Inputs are shift-registered before passing to register file.
 * 
 * */
module async_reg_file(
                      address,
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
   output [31:0] data_out;      // Data output
   output [7:0] led_out;	// LED output
   
   // Register file size
   parameter REG_FILE_SIZE      = 8; // Register file size
   parameter LOG_REG_FILE_SIZE  = 3; // Number of internal select bits

   // Clocked inputs
   wire              rs_clkd;
   wire              ws_clkd;
   wire              as_clkd;

   // Generate connections
   shift_reg  #(4) sr_ws  (.in(ws_n), .out(ws_clkd), .clk(clk));
   shift_reg  #(4) sr_rs  (.in(rs_n), .out(rs_clkd), .clk(clk));
   shift_reg  #(4) sr_as  (.in(as),   .out(as_clkd), .clk(clk));

   // Internal register file
   reg_file rf (.address(address),
                .data_in(data_in),
                .data_out(data_out),
                .ws_n(ws_clkd),
                .rs_n(rs_clkd),
                .be(be),
                .clk(clk),
                .as(as_clkd),
                .rst_n(rst_n),
                .led_out(led_out));
endmodule // async_reg_file

                

   
                      
                      
