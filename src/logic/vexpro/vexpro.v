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
			MotorC = 1'b0;
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
	I2CDataOut, I2CDataIn, I2CClkOut, I2CClkIn, 
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
	
	output I2CDataOut;
	input  I2CDataIn;
	output I2CClkOut; 
	input  I2CClkIn; 
	
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
	wire GpioIntStatus;
	wire GpioIntReset;
	wire [13:0] IntDummy;
	reg  TestIntStatus;
	wire TestIntReset;
	Primary InstPrimary(.Addr(Addr), .Data(Data), .RdN(RdN), .WrN(WrN), .Dq(Dq), 
	   .CsN(CsN), .Wait(Wait), .Int(Int), .Clk(Clk), 
      .Async(Async), .Asdo(Asdo), .Arstn(Arstn), .Asdi(Asdi), .AbitClk(AbitClk), .AudioOut(Audio),
		.Rd(Rd), .Wr(Wr), .Cs(Cs), .DataRd(DataRd), .Identifier(16'hc002), .Reset(Reset),
		.IntStatus({14'h0000, GpioIntStatus, BemfIntStatus}), 
		.IntReset({IntDummy, GpioIntReset, BemfIntReset}));

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
		
`ifdef foo
	assign P[0] = Active[0];
	assign P[1] = Active[1];
	assign P[2] = Active[2];
	assign P[3] = Active[3];
	assign P[4] = Measure[0];
	assign P[5] = Measure[1];
	assign P[6] = Measure[2];
	assign P[7] = Measure[3];
`endif

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

   // I2C
	wire I2cEn;
	wire [15:0] I2cDataRd;
   assign I2cEn = Cs & Addr[11:6]==6'b010010;

	I2cCont InstI2cCont(.Addr(Addr[3:1]), .DataRd(I2cDataRd), .DataWr(Data), .En(I2cEn), .Rd(Rd), .Wr(Wr), 
	   .SdaOut(I2CDataOut), .SdaIn(I2CDataIn), .SclOut(I2CClkOut), .SclIn(I2CClkIn), 
//		.IntStatus(I2cIntStatus), .IntReset(I2cIntReset), 
	   .Reset(Reset), .Clk(Clk));

	// RC Servo
	wire RcsEn;
	wire [15:0] RcsDataRd;
   assign RcsEn = Cs & Addr[11:6]==6'b010000;

	RCServo12 InstRCServo(.Addr(Addr[5:1]), .DataRd(RcsDataRd), .DataWr(Data), .En(RcsEn), .Rd(Rd), .Wr(Wr), 
	   .P({P[27], P[26], P[25], P[24], P[23], P[22], P[21], P[20], 
		    P[19], P[18], P[17], P[16]}), 
		.Clk(Clk));

	// GpioInt
	wire GpioEn;
	wire [15:0] GpioDataRd;
   assign GpioEn = Cs & Addr[11:6]==6'b010001;

	GpioInt InstGpioInt(.Addr(Addr[3:1]), .DataRd(GpioDataRd), .DataWr(Data), .En(GpioEn), .Rd(Rd), .Wr(Wr), 
		.Port({P[15], P[14], P[13], P[12], P[11], P[10], P[9], P[8],
		       P[7],  P[6],  P[5],  P[4],  P[3],  P[2],  P[1], P[0]}), 
		.IntStatus(GpioIntStatus), .IntReset(GpioIntReset), .Reset(Reset), .Clk(Clk));


   // LedCont
	wire LedEn;
	wire [15:0] LedDataRd;
   assign LedEn = Cs & Addr[11:6]==6'b010011;

	LedCont InstLedCont(.Addr(Addr[2:1]), .DataRd(LedDataRd), .DataWr(Data), .En(LedEn), .Rd(Rd), .Wr(Wr), 
			.LedGreen({P[47], P[45], P[43]}), .LedRed({P[48], P[46], P[44]}), .Reset(Reset), .Clk(Clk));

	always @(BemfDataRd or BemfEn or
	         RcsDataRd or RcsEn or
				GpioDataRd or GpioEn or
				I2cDataRd or I2cEn or
				LedDataRd or LedEn)
	   begin
		case ({BemfEn, RcsEn, GpioEn, I2cEn, LedEn})
		   5'b10000: DataRd = BemfDataRd;
			5'b01000: DataRd = RcsDataRd;
			5'b00100: DataRd = GpioDataRd;
			5'b00010: DataRd = I2cDataRd;
			5'b00011: DataRd = LedDataRd;
		   default: DataRd = 16'hxxxx;
		endcase
		end
		 
endmodule
