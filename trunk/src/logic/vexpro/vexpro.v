//               pwm1   pwm0   measure
// b a           p e -> p e -> p e
// 0 0 coast     0 0    1 0    0 0
// 0 1 forward   0 1    1 0    0 0
// 1 0 reverse   1 1    1 0    0 0
// 1 1 brake     1 1    1 0    0 0   

module MotorLogic(Control, Pwm, Measure, MotorA, MotorB, MotorC);

   input  [1:0]Control;
	input  Pwm;
	input  Measure;
	output MotorA;
	output MotorB;
	output MotorC;

	reg MotorA;
	reg MotorB;
	reg MotorC;

   always @(Control or Pwm or Measure)
	   begin
      if (Measure | Control==2'b00) // fast decay, coast
		   begin
			MotorA = 1'b0;
			MotorB = 1'b0;
			MotorC = 1'b1;
			end
		else if (~Pwm | Control==2'b11) // slow decay, brake
		   begin
			MotorA = 1'b0;
			MotorB = 1'b0;
			end
      else if (Control==2'b01) // forward
		   begin
			MotorA = 1'b1;
			MotorB = 1'b0;
			MotorC = 1'b0;
			end
      else // reverse 
		   begin
			MotorA = 1'b0;
			MotorB = 1'b1;
			MotorC = 1'b0;
			end
      end

endmodule			 

module Vexpro(Addr, Data, RdN, WrN, Dq, CsN, Wait, Int, Clk, 
   Async, Asdo, Arstn, Asdi, AbitClk, Audio,
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
	output Audio;
	
	inout  [52:0] P;
	input  [4:0] PIn;
	input  PClkIn;

	wire   [15:0] DataRd;

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
      .Async(Async), .Asdo(Asdo), .Arstn(Arstn), .Asdi(Asdi), .AbitClk(AbitClk), .AudioOut(Audio),
		.Rd(Rd), .Wr(Wr), .Cs(Cs), .DataRd(DataRd), .Identifier(16'hc002), .Reset(Reset),
		.IntStatus({GpioIntStatus, 7'h00, BemfIntStatus}), 
		.IntReset({GpioIntReset, IntDummy, BemfIntReset}));

	wire BemfEn;
	wire [15:0]BemfDataRd;
   wire [3:0] PwmOut;
   wire [7:0] PwmCont;
	wire [3:0] Active;
	wire [3:0] Measure;
	wire AdcDir;
   wire AdcOut;
	
	assign P[41] = AdcDir ? AdcOut : 1'bz;
	assign BemfEn = Cs & Addr[11:10]==2'b00;
		
   BemfCont4 InstBemfCont4(.Addr(Addr[9:1]), .DataRd(BemfDataRd), .DataWr(Data), .En(BemfEn), 
	   .Rd(Rd), .Wr(Wr), .PwmOut(PwmOut), .PwmCont(PwmCont), .AxisActive(Active), .AxisMeasure(Measure), 
		.AdcIn(P[41]), .AdcOut(AdcOut), .AdcDir(AdcDir), .AdcCs(P[42]), .AdcClk(P[40]), 
		.IntStatus(BemfIntStatus), .IntReset(BemfIntReset), 
		.Reset(Reset), .Clk(Clk)); 
		
	assign P[0] = Active[0];
	assign P[1] = Active[1];
	assign P[2] = Active[2];
	assign P[3] = Active[3];
	assign P[4] = Measure[0];
	assign P[5] = Measure[1];
	assign P[6] = Measure[2];
	assign P[7] = Measure[3];

	// ME0 P13
	// MP0 P18
	// ME1 P22
	// MP1 P27
	// ME2 P29
	// MP2 P25
	// ME3 P20
	// MP3 P15

	MotorLogic Inst0MotorLogic(.Control(PwmCont[1:0]), .Pwm(PwmOut[0]), .Measure(~Active[0]),
		.MotorA(P[28]), .MotorB(P[29]), .MotorC(P[30]));
	MotorLogic Inst1MotorLogic(.Control(PwmCont[3:2]), .Pwm(PwmOut[1]), .Measure(~Active[1]),
		.MotorA(P[31]), .MotorB(P[32]), .MotorC(P[33]));
	MotorLogic Inst2MotorLogic(.Control(PwmCont[5:4]), .Pwm(PwmOut[2]), .Measure(~Active[2]),
		.MotorA(P[34]), .MotorB(P[35]), .MotorC(P[36]));
	MotorLogic Inst3MotorLogic(.Control(PwmCont[7:6]), .Pwm(PwmOut[3]), .Measure(~Active[3]),
		.MotorA(P[37]), .MotorB(P[38]), .MotorC(P[39]));

`ifdef foo
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
	   .SdaOut(P[49]), .SdaIn(PIn[4]), .SclOut(P[47]), .SclIn(), 
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
`endif
	assign DataRd = BemfEn ? BemfDataRd : 16'hxxxx;
		 
endmodule
