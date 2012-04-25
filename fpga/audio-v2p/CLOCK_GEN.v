module CLOCK_GEN (
      // inputs
      SYSTEM_CLOCK,
      // outputs
      system_clock_buffered,
      reset
      );

input			SYSTEM_CLOCK;			// 100MHz LVTTL SYSTEM CLOCK

output 		system_clock_buffered;		// buffered SYSTEM_CLOCK
output		reset;				// reset asserted when DCMs are NOT LOCKED

wire			low  = 1'b0;
wire			high = 1'b1;

// signals associated with the system clock DCM
wire 			system_dcm_reset;
wire 			system_dcm_locked;
wire			system_clock_in;
wire			system_clock_unbuffered;
wire 			system_clock_buffered;

IBUFG SYSTEM_CLOCK_BUF (
.O  (system_clock_in),
.I  (SYSTEM_CLOCK)
);

// instantiate the clock input buffers for the internal clocks
BUFG SYS_CLOCK_BUF (
.O  (system_clock_buffered),
.I  (system_clock_unbuffered)
);


assign reset = !system_dcm_locked;

DCM SYSTEM_DCM (
.CLKFB		(system_clock_buffered),
.CLKIN		(system_clock_in),
.DSSEN		(low),
.PSCLK		(low),
.PSEN			(low),
.PSINCDEC	(low),
.RST			(system_dcm_reset),
.CLK0			(system_clock_unbuffered),
.CLK90		(),
.CLK180		(),
.CLK270		(),
.CLK2X		(),
.CLK2X180	(),
.CLKDV		(),
.CLKFX		(),
.CLKFX180	(),
.LOCKED		(system_dcm_locked),
.PSDONE		(),
.STATUS		()
);

SRL16 RESET_SYSTEM_DCM (
.Q		(system_dcm_reset),
.CLK	(system_clock_in),
.D 	(low),
.A0	(high),
.A1	(high),
.A2	(high),
.A3	(high)
);
defparam RESET_SYSTEM_DCM.INIT = "000F";

endmodule //CLOCK_GEN
