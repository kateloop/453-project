//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
module tll5000_register_interface
(
   input clock,
   input reset,
   input[23:0] address,
   input[31:0] dataIn,
   input[3:0] bEnable,
   input aStrobe,
   input rStrobe,
   input wStrobe,
   output[31:0] dataOut,
   output dataValid,
   input[4:0] pbIn,
   input[7:0] swIn,
   output[7:0] ledOut,
   output irqOut
);

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
reg[31:0] data;
reg valid;
reg enable0;
wire rEnable0;
wire wEnable0;
wire rEnable;
wire wEnable;

reg[15:0] rCount;
reg[15:0] wCount;
reg[31:0] scratch[3:0];
integer i;

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
assign rEnable0 = aStrobe && rStrobe;
assign wEnable0 = aStrobe && wStrobe && bEnable;

assign rEnable = enable0 && rEnable0;
assign wEnable = enable0 && wEnable0;

assign ledOut = ledState;
assign irqOut = (irqMask & irqStatus) ? 1 : 0;
assign dataOut = rEnable0 ? data : 32'hzzzzzzzz;
assign dataValid = valid;

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
task scratchReg;
   input[3:0] index;
begin
   if (rEnable)
   begin
      data <= scratch[index];
      rCount <= rCount + 1;
   end
   
   if (wEnable)
   begin
      case (bEnable)
         4'b0001: scratch[index][7:0] <= dataIn[7:0];
         4'b0011: scratch[index][15:0] <= dataIn[15:0];
         4'b1111: scratch[index] <= dataIn;
      endcase
      wCount <= wCount + 1;
   end
end
endtask

//////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////
always @ (posedge clock or posedge reset)
begin
   if (reset)
   begin
      data <= 32'd0;
      valid <= 1;
      enable0 <= 1;

      rCount <= 0;
      wCount <= 0;

      for (i = 0; i < 4; i = i + 1)
         scratch[i] <= 0;
	end
   else
   begin
      if (rEnable || wEnable)
         enable0 <= 0;
      else if (aStrobe == 0)
         enable0 <= 1;

      case (address)

         24'h000000:
         begin
            if (rEnable)
               data <= {wCount, rCount};
         end

         24'h000004:
            scratchReg(0);

         24'h000008:
            scratchReg(1);

         24'h00000C:
            scratchReg(2);

         24'h000010:
            scratchReg(3);

      endcase
   end
end

endmodule
