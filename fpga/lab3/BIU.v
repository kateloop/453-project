module BIU( input [31:0] ARM_D, input [3:0] BE, input [23:0] address, input AS, RS, WS, RST, CLK, output reg c_AS, c_RS, 
				c_WS, output reg [23:0] c_address, output reg [31:0] c_ARM_D, output reg [3:0] c_BE);

	reg [3:0] be0, be1;
	reg rs0, rs1, ws0, ws1, as0, as1; 
	reg [31:0] data0, data1;
	reg [23:0] address0, address1;

	always @(posedge CLK, negedge RST) begin
	
		if (~RST) begin
			address0 <= 32'd0;
			address <= 32'd0;
			rs0 <= 1'b0;
			rs1 <= 1'b0;
			ws0 <= 1'b0;
			ws1 <= 1'b0;
			as0 <= 24'd0;
			as1 <= 24'd0;
			data0 <= 32'd0;
			data1 <= 32'd0;
		end
		
		else begin
		
		address0 <= address;
		address1 <= address0;
		if (address1 == address0) begin
			c_address <= address1;
		end
		
		be0 <= BE;
		be1 <= be0;
		if (be0 == be1) begin
			c_BE <= be1;
		end
		
		data0 <= ARM_D;
		data1 <= data0;
		if(data0 == data1) begin
			c_ARM_D <= data1;
		end
		
		as0 <= AS;
		as1 <= as0;
		if(as0 == as1) begin
			c_AS <= as1;
		end
		
		rs0 <= RS;
		rs1 <= rs0;
		if(rs0 == rs1) begin
			c_RS <= rs1;
		end
		
		ws0 <= WS;
		ws1 <= ws0;
		if(ws0 == ws1) begin
			c_WS <= ws1;
		end
		
		end
		
	end

endmodule
