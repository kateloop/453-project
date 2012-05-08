`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:23:05 01/28/2012 
// Design Name: 
// Module Name:    lab1 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module lab1( inout [31:0] data, input [23:0] address, input CLK, RST, AS, RS, WS, input [3:0] BE);

	reg [31:0] count, scratch1, scratch2, scratch3, scratch4, read;
	reg busy;
	
	assign data = (~RS) ? read : 32'dz;

initial begin
			busy <= 1'b0;
			count <= 32'd0;
			scratch1 <= 32'd0;
			scratch2 <= 32'd0;
			scratch3 <= 32'd0;
			scratch4 <= 32'd0;
end
			
	always @(posedge CLK, negedge RST) begin
		
	
		if (~RST) begin
			busy <= 1'b0;
			count <= 32'd0;
			scratch1 <= 32'd0;
			scratch2 <= 32'd0;
			scratch3 <= 32'd0;
			scratch4 <= 32'd0;
		end // end reset
		
		else if (~WS && AS) begin
			
				if (~busy) begin
					busy <= 1'b1;
					if ( (address == 24'h000004) || (address == 24'h000008) || (address == 24'h00000C) || (address == 24'h000010) ) begin
						count[31:16] <= count[31:16] + 16'h0001;
					end
				end
				
	
				case (address) 
					24'h000004 : begin
							case (BE) 
								4'b0011 : scratch1[15:0] <= data[15:0];
								4'b0000 : scratch1 <= data[31:0];
								default : scratch1[7:0] <= data[7:0];
							endcase
						end
					24'h000008 : begin
							case (BE) 
								4'b0011 : scratch2[15:0] <= data[15:0];
								4'b0000 : scratch2 <= data[31:0];
								default : scratch2[7:0] <= data[7:0];
							endcase
						end
					24'h00000C : begin
							case (BE) 
								4'b0011 : scratch3[15:0] <= data[15:0];
								4'b0000 : scratch3 <= data[31:0];
								default : scratch3[7:0] <= data[7:0];
							endcase
						end
					24'h0000010 : begin
							case (BE) 
								4'b0011 : scratch4[15:0] <= data[15:0];
								4'b0000 : scratch4 <= data[31:0];
								default : scratch4[7:0] <= data[7:0];
							endcase
						end
				endcase
				
			end // end if (~WS && AS)
			 
			else if (~RS && AS) begin
			
				if (~busy) begin
					busy <= 1'b1;
					if ( (address == 24'h000004) || (address == 24'h000008) || (address == 24'h00000C) || (address == 24'h000010) ) begin
						count[15:0] <= count[15:0] + 16'd1;
					end
				end
				
				case (address) 
					24'h000000 : read <= count;
					24'h000004 : read <= scratch1;
					24'h000008 : read <= scratch2;
					24'h00000C : read <= scratch3;
					24'h000010 : read <= scratch4;
				endcase
				
			end
			
			else if (~AS) begin
				busy <= 1'b0;
			end
		
	end // end always block

endmodule
