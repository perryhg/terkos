module Primary(Addr, Data, RdN, WrN, Dq, CsN, Wait, Int, Clk, 
   Async, Asdo, Arstn, Asdi, AbitClk, AudioOut,
	Rd, Wr, Cs, DataRd, Identifier, Reset, 
	IntStatus, IntReset);

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
	output AudioOut;

   output Rd;	
	output Wr;
	output Cs;
   input  [15:0] DataRd;
	input  [15:0] Identifier;
	output Reset;

   input  [31:0] IntStatus;
   output [31:0] IntReset;

   reg [15:0] TempDataRd;

   assign Rd = ~RdN;
	assign Wr = ~WrN;
	assign Cs = ~CsN[0];

	assign Data[15:0] = Cs & Rd ? TempDataRd : 16'hzzzz;

	assign Wait = 1'bz;

   // Power-up reset (synchronous)
   reg  [1:0] ResetCount;
   always @(posedge Clk)
      begin
      if (ResetCount!=2'b11)
         ResetCount <= ResetCount + 1;
      end
   assign Reset = ResetCount!=2'b11;  // active high

	// Interrupt controller
   wire IntEn;
   assign IntEn = Cs & Addr[11:4]==8'hff;
   wire [15:0] IntDataRd;
   InterruptCont InstInterruptCont(.Addr(Addr[3:1]), .DataRd(IntDataRd), .DataWr(Data), 
      .En(IntEn), .Rd(Rd), .Wr(Wr), .IntStatus(IntStatus), .IntReset(IntReset), .Int(Int), .Reset(Reset), .Clk(Clk));

 	// Audio DAC
	wire AudioEn = Cs & Addr[11:4]==8'hfe;
	wire [15:0] AudioDataRd;
	AudioDAC InstAudioDAC(.Addr(Addr[4:1]), .DataRd(AudioDataRd), .DataWr(Data),
		.En(AudioEn), .Rd(Rd), .Wr(Wr), .Reset(Reset),
		.Async(Async), .Asdo(Asdo), .Arstn(Arstn), .Asdi(Asdi),
		.AbitClk(AbitClk), .Out(AudioOut), .Clk(Clk));

	wire IdentEn = Cs & Addr[11:4]==8'hfd;

   // Combine data
   always @(DataRd or IntEn or IntDataRd or AudioEn or AudioDataRd or IdentEn or Identifier)
      begin
		case ({IntEn, AudioEn, IdentEn})
		   3'b100:  TempDataRd = IntDataRd;
			3'b010:  TempDataRd = AudioDataRd;
			3'b001:  TempDataRd = Identifier;
			default: TempDataRd = DataRd;
		endcase
      end

endmodule
