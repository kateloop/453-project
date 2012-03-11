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
   
   // Register file size
   parameter REG_FILE_SIZE      = 10; // Register file size
   parameter LOG_REG_FILE_SIZE  = 4; // Number of internal select bits

   // Internal regs
   reg [LOG_REG_FILE_SIZE - 1:0] select; // Address decoding   
   reg [31:0]                    registers [REG_FILE_SIZE - 1:0]; // The registers
   reg                           rw_state;                        // 1 if in read or write

   // Internal wires
   wire rs_en, ws_en;
   assign rs_en = ~rs_n && ~rw_state;
   assign ws_en = ~ws_n && ~rw_state;   
   
   // Address locations
   parameter SENSORA = 0;
   parameter SENSORB = 4;
   parameter SENSORC = 8;
   parameter SENSORD = 12;
   parameter SENSORE = 16;
   parameter SENSORF = 20;
   parameter SENSORG = 24;
   parameter LED1    = 28;
   parameter LED2    = 32;
   parameter LED3    = 36;

   // Address decode
   always @(address) begin
      case (address)
        SENSORA:  select = SENSORA;
        SENSORB:  select = SENSORB;
 	SENSORC:  select = SENSORC;
	SENSORD:  select = SENSORD;
	SENSORE:  select = SENSORE;
 	SENSORF:  select = SENSORF;
	SENSORG:  select = SENSORG;
	LED1:     select = LED1;
	LED2:	  select = LED2;
    	LED3:     select = LED3;
      endcase
   end
    
   // Clocking registers
   always @(posedge clk or negedge rst_n) begin
      // Reset
      if (~rst_n) begin
         data_out <= 32'hfee1dead;
         rw_state <= 1'b0;
 	 registers[LED1] <= 32'b0;
	 registers[LED2] <= 32'b0;
	 registers[LED3] <= 32'b0;
      end

      // Chip selected
      else if (as) begin
         // Read
         if (rs_en) begin
            // Set rw_state
            rw_state <= 1'b1;
            
            // Output value contained in register
            if (select == SENSORA) begin
               data_out <= registers[select];    
            end
	    else if (select == SENSORB) begin
	       data_out <= registers[select];
 	    end
	    else if (select == SENSORC) begin
	       data_out <= registers[select];
	    end
	    else if (select == SENSORD) begin
	       data_out <= registers[select];
 	    end
	    else if (select == SENSORE) begin
 	       data_out <= registers[select];
	    end
	    else if (select == SENSORF) begin
 	       data_out <= registers[select];
	    end
	    else if (select == SENSORG) begin
 	       data_out <= registers[select];
	    end
	    else if (select == LED1) begin
 	       data_out <= registers[select];
	    end
	    else if (select == LED2) begin
 	       data_out <= registers[select];
	    end
	    else if (select == LED3) begin
 	       data_out <= registers[select];
  	    end
         end

         // Write
         else if (ws_en) begin
            // Set rw_state
            rw_state <= 1'b1;

            // Turn LEDs on/off
            if (select == LED1) begin
               registers[select] <= data_in;
            end
            else if (select == LED2) begin
               registers[select] <= data_in;
            end
	    else if (select == LED3) begin
	      registers[select] <= data_in;
	    end
         end
      end // if (as)

      // Not selected - reset rw_cycle
      else begin
         rw_state <= 1'b0;
      end
   end 
endmodule // regFile

  
       
   
   
   
   
   
