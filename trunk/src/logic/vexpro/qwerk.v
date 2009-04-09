//               pwm1   pwm0   measure
// b a           p e -> p e -> p e
// 0 0 coast     0 0    1 0    0 0
// 0 1 forward   0 1    1 0    0 0
// 1 0 reverse   1 1    1 0    0 0
// 1 1 reserved  1 1    1 0    0 0   

module MotorLogic(Control, Pwm, Measure, Enable, Phase);

   input  [1:0]Control;
	input  Pwm;
	input  Measure;
	output Enable;
	output Phase;

	reg Enable;
	reg Phase;

   always @(Control or Pwm or Measure)
	   begin
      if (Measure | Control==2'b00) // fast decay, coast
		   begin
			Enable = 1'b0;
			Phase = 1'b0;
			end
		else if (~Pwm) // slow decay, brake
		   begin
			Enable = 1'b0;
			Phase = 1'b1;
			end
      else if (Control==2'b01) // forward
		   begin
			Enable = 1'b1;
			Phase = 1'b1;
			end
      else // reverse 
		   begin
			Enable = 1'b1;
			Phase = 1'b0;
			end
      end

endmodule			 

module Qwerk(Addr, Data, RdN, WrN, Dq, CsN, Wait, Int, Clk, 
   Async, Asdo, Arstn, Asdi, AbitClk, 
	P, PIn, PClkIn);

	input  [11:0] Addr;
	inout  [15:0] Data;
	input  RdN;
	input  WrN;
	input  [1:0] Dq;
	input  [1:0] CsN;
	output Wait;
	output Int;
	input  Clk;

	input  Async;
	input  Asdo;
	input  Arstn;
	output Asdi;
	input  AbitClk;

	inout  [52:0] P;
	input  [4:0] PIn;
	input  PClkIn;

	reg    [15:0] DataRd;

   wire   Rd;
	wire   Wr;
	wire   Cs;
	wire   Reset;

	wire BemfIntStatus;
	wire BemfIntReset;
	wire [7:0] GpioIntStatus;
	wire [7:0] GpioIntReset;
	wire [6:0] IntDummy;
	reg  TestIntStatus;
	wire TestIntReset;
	Primary InstPrimary(.Addr(Addr), .Data(Data), .RdN(RdN), .WrN(WrN), .Dq(Dq), 
	   .CsN(CsN), .Wait(Wait), .Int(Int), .Clk(Clk), 
      .Async(Async), .Asdo(Asdo), .Arstn(Arstn), .Asdi(Asdi), .AbitClk(AbitClk), .AudioOut(P[24]),
		.Rd(Rd), .Wr(Wr), .Cs(Cs), .DataRd(DataRd), .Identifier(16'ha001), .Reset(Reset),
		.IntStatus({GpioIntStatus, 7'h00, BemfIntStatus}), 
		.IntReset({GpioIntReset, IntDummy, BemfIntReset}));

	wire BemfEn;
	wire [15:0]BemfDataRd;
   wire [3:0] PwmOut;
   wire [7:0] PwmCont;
	wire [3:0] Active;
	assign BemfEn = Cs & Addr[11:10]==2'b00;

   BemfCont4 InstBemfCont4(.Addr(Addr[9:1]), .DataRd(BemfDataRd), .DataWr(Data), .En(BemfEn), 
	   .Rd(Rd), .Wr(Wr), .PwmOut(PwmOut), .PwmCont(PwmCont), .AxisActive(Active), 
		.AdcIn(P[42]), .AdcCs(P[45]), .AdcClk(P[40]), .AdcMux({P[36], P[34]}), .AdcSel(P[31]), .AdcDiv(P[38]), 
		.IntStatus(BemfIntStatus), .IntReset(BemfIntReset), 
		.Reset(Reset), .Clk(Clk)); // .Measure0(P[12]

	//assign P[8] = Active[0];

	// ME0 P13
	// MP0 P18
	// ME1 P22
	// MP1 P27
	// ME2 P29
	// MP2 P25
	// ME3 P20
	// MP3 P15

	MotorLogic Inst0MotorLogic(.Control(PwmCont[1:0]), .Pwm(PwmOut[0]), .Measure(~Active[0]),
		.Enable(P[13]), .Phase(P[18]));
	MotorLogic Inst1MotorLogic(.Control(PwmCont[3:2]), .Pwm(PwmOut[1]), .Measure(~Active[1]),
		.Enable(P[22]), .Phase(P[27]));
	MotorLogic Inst2MotorLogic(.Control(PwmCont[5:4]), .Pwm(PwmOut[2]), .Measure(~Active[2]),
		.Enable(P[29]), .Phase(P[25]));
	MotorLogic Inst3MotorLogic(.Control(PwmCont[7:6]), .Pwm(PwmOut[3]), .Measure(~Active[3]),
		.Enable(P[20]), .Phase(P[15]));

	// RC Servo
	wire RcsEn;
	wire [15:0] RcsDataRd;
   assign RcsEn = Cs & Addr[11:6]==6'b010000;

	RCServo #(16) InstRCServo(.Addr(Addr[5:1]), .DataRd(RcsDataRd), .DataWr(Data), .En(RcsEn), .Rd(Rd), .Wr(Wr), 
	   .P({P[51], P[52], P[50], P[48], P[46], P[44], P[43], P[41], 
		    P[39], P[37], P[35], P[33], P[32], P[30], P[28], P[26]}), 
		.Clk(Clk));

	// GpioInt
	wire GpioEn;
	wire [15:0] GpioDataRd;
   assign GpioEn = Cs & Addr[11:6]==6'b010001;

	GpioInt InstGpioInt(.Addr(Addr[3:1]), .DataRd(GpioDataRd), .DataWr(Data), .En(GpioEn), .Rd(Rd), .Wr(Wr), 
	   .PortIn({PIn[0], P[4], P[2], P[0], P[7], P[5], P[3], P[1]}), 
		.PortOut({P[11], P[10], P[9], P[6], P[16], P[14], P[12], P[8]}), 
		.IntStatus(GpioIntStatus), .IntReset(GpioIntReset), .Reset(Reset), .Clk(Clk));

   // I2C
	wire I2cEn;
	wire [15:0] I2cDataRd;
   assign I2cEn = Cs & Addr[11:6]==6'b010010;

	I2cCont InstI2cCont(.Addr(Addr[3:1]), .DataRd(I2cDataRd), .DataWr(Data), .En(I2cEn), .Rd(Rd), .Wr(Wr), 
	   .SdaOut(P[49]), .SdaIn(PIn[4]), .Scl(P[47]), 
//		.IntStatus(I2cIntStatus), .IntReset(I2cIntReset), 
	   .Reset(Reset), .Clk(Clk));

	always @(BemfDataRd or BemfEn or
	         RcsDataRd or RcsEn or
				GpioDataRd or GpioEn or
				I2cDataRd or I2cEn)
	   begin
		case ({BemfEn, RcsEn, GpioEn, I2cEn})
		   4'b1000: DataRd = BemfDataRd;
			4'b0100: DataRd = RcsDataRd;
			4'b0010: DataRd = GpioDataRd;
			4'b0001: DataRd = I2cDataRd;
		   default: DataRd = 16'hxxxx;
		endcase
		end
		 
endmodule
