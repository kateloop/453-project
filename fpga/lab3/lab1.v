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



module lab1( input [31:0] data_in, output [31:0] data_out, input [7:0] SW, output [7:0] LED, input [23:0] address, input CLK, RST, AS, RS, WS, input [3:0] BE, input [15:0] ACC_PORT_PIN, output reg [15:0] ACC_PORT_PIN_OUT);

	reg [31:0] count, scratch1, scratch2, scratch3, scratch4, read, io_direction, io_write, io_read, RandomNumber;
	reg busy;
	 reg [31:0] random;
	wire [31:0] data;

		
	assign data = data_in;
	
	assign data_out = (~RS) ? read : 32'dz;

	assign LED = SW;
   

			
	always @(posedge CLK, negedge RST) begin
	

	
	
		if (~RST) begin
			busy <= 1'b0;
			count <= 32'd0;
			scratch1 <= 32'd0;
			scratch2 <= 32'd0;
			scratch3 <= 32'd0;
			scratch4 <= 32'd0;
			RandomNumber <= 32'd0;
			io_direction <= 32'd0;
			io_write <= 32'd0;
			io_read <= 32'd0;
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
					24'h000020 : begin
							case (BE) 
								4'b0011 : scratch4[15:0] <= data[15:0];
								4'b0000 : scratch4 <= data[31:0];
								default : scratch4[7:0] <= data[7:0];
							endcase
						end
					24'h000014 : begin
							io_direction[15:0] <= data_in[15:0]; 
					end
					
					24'h000010 : begin
							case (BE) 
								4'b0011 : RandomNumber[15:0] <= data[15:0];
								4'b0000 : RandomNumber <= data[31:0];
								default : RandomNumber[7:0] <= data[7:0];
							endcase				
							random <= (RandomNumber >> 1) ^ (-(RandomNumber & 1) & 32'h80200003);
					end
					24'h000018 : begin
							if (io_direction[0]) begin
								ACC_PORT_PIN_OUT[0] <= data[0];
							end
							if (io_direction[1]) begin
								ACC_PORT_PIN_OUT[1] <= data[1];
							end
							if (io_direction[2]) begin
								ACC_PORT_PIN_OUT[2] <= data[2];
							end
							if (io_direction[3]) begin
								ACC_PORT_PIN_OUT[3] <= data[3];
							end
							if (io_direction[4]) begin
								ACC_PORT_PIN_OUT[4] <= data[4];
							end
							if (io_direction[5]) begin
								ACC_PORT_PIN_OUT[5] <= data[5];
							end
							if (io_direction[6]) begin
								ACC_PORT_PIN_OUT[6] <= data[6];
							end
							if (io_direction[7]) begin
								ACC_PORT_PIN_OUT[7] <= data[7];
							end
							if (io_direction[8]) begin
								ACC_PORT_PIN_OUT[8] <= data[8];
							end
							if (io_direction[9]) begin
								ACC_PORT_PIN_OUT[9] <= data[9];
							end
							if (io_direction[10]) begin
								ACC_PORT_PIN_OUT[10] <= data[10];
							end
							if (io_direction[11]) begin
								ACC_PORT_PIN_OUT[11] <= data[11];
							end
							if (io_direction[12]) begin
								ACC_PORT_PIN_OUT[12] <= data[12];
							end
							if (io_direction[13]) begin
								ACC_PORT_PIN_OUT[13] <= data[13];
							end
							if (io_direction[14]) begin
								ACC_PORT_PIN_OUT[14] <= data[14];
							end
							if (io_direction[15]) begin
								ACC_PORT_PIN_OUT[15] <= data[15];
							end
					end
				endcase
				
			end // end if (~WS && AS)
			 
			else if (~RS && AS) begin
			
				if (~busy) begin
					busy <= 1'b1;
					if ( (address == 24'h000004) || (address == 24'h000008) || (address == 24'h00000C) || (address == 24'h000010) ) begin
						count[15:0] <= count[15:0] + 16'd1;
					if (address == 24'h000010) begin
					random <= (random >> 1) ^ (-(random & 1) & 32'h80200003);
					read <= random;

					end
					end
				end
				
				case (address)
					24'h000000 : read <= count;
					24'h000004 : read <= scratch1;
					24'h000008 : read <= scratch2;
					24'h00000C : read <= scratch3;
					24'h000020 : read <= scratch4;
					24'h000024 : read <= SW;
		
					24'h00001C : begin
						if (~io_direction[0]) begin
							read <= ACC_PORT_PIN[0];
						end
						if (~io_direction[1]) begin
							read <= ACC_PORT_PIN[1];
						end
						if (~io_direction[2]) begin
							read <= ACC_PORT_PIN[2];
						end
						if (~io_direction[3]) begin
							read <= ACC_PORT_PIN[3];
						end
						if (~io_direction[4]) begin
							read <= ACC_PORT_PIN[4];
						end
						if (~io_direction[5]) begin
							read <= ACC_PORT_PIN[5];
						end
						if (~io_direction[6]) begin
							read <= ACC_PORT_PIN[6];
						end
						if (~io_direction[7]) begin
							read <= ACC_PORT_PIN[7];
						end
						if (~io_direction[8]) begin
							read <= ACC_PORT_PIN[8];
						end
						if (~io_direction[9]) begin
							read <= ACC_PORT_PIN[9];
						end
						if (~io_direction[10]) begin
							read <= ACC_PORT_PIN[10];
						end
						if (~io_direction[11]) begin
							read <= ACC_PORT_PIN[11];
						end
						if (~io_direction[12]) begin
							read <= ACC_PORT_PIN[12];
						end
						if (~io_direction[13]) begin
							read <= ACC_PORT_PIN[13];
						end
						if (~io_direction[14]) begin
							read <= ACC_PORT_PIN[14];
						end
						if (~io_direction[15]) begin
							read <= ACC_PORT_PIN[15];
						end
					end
				endcase
				
			end
			
			else if (~AS) begin
				busy <= 1'b0;
			end
			
		
	end // end always block

endmodule
