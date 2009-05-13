module Gpio(Addr, DataRd, DataWr, En, Rd, Wr, P, Clk);

   parameter NUM_GPIO = 16;
   
   input  [3:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   inout  [NUM_GPIO-1:0] P;
	input  Clk;   

   reg     [15:0] DataRd;
   reg     [NUM_GPIO-1:0] DdrReg; 
   reg     [NUM_GPIO-1:0] DataReg; 
   integer i;
   integer j;
   integer n;
               
   GpioLogic #(NUM_GPIO) InstGpioLogic(DdrReg, DataReg, P[NUM_GPIO-1:0]);
   
   always @(posedge Clk)
      begin
      for (i=0; i<(NUM_GPIO+15)/16; i=i+1)
         begin
         if ((i+1)*16>NUM_GPIO)
            n = NUM_GPIO - i*16;
         else
            n = 16; 
         if (Addr[3:0]==i & En & Wr)
            begin
            for (j=0; j<n; j=j+1)
               DdrReg[j+i*16] = DataWr[j];
            end
         if (Addr[3:0]==i+(NUM_GPIO+15)/16 && En)
            begin
            for (j=0; j<n; j=j+1)
               DataReg[j+i*16] = DataWr[j];
            end
         end
      end

   always @(Addr or Rd or En or DdrReg or P)
      begin
      DataRd = 16'hxxxx;
      for (i=0; i<(NUM_GPIO+15)/16; i=i+1)
         begin
         if ((i+1)*16>NUM_GPIO)
            n = NUM_GPIO - i*16;
         else
            n = 16; 
         if (Addr[3:0]==i)
            begin
            for (j=0; j<n; j=j+1)
               DataRd[j] = DdrReg[j+i*16];
            for (j=n; j<16; j=j+1)
               DataRd[j] = 1'b0;
            end
         if (Addr[3:0]==i+(NUM_GPIO+15)/16)
            begin
            for (j=0; j<n; j=j+1)
               DataRd[j] = P[j+i*16];
            for (j=n; j<16; j=j+1)
               DataRd[j] = 1'b0;
            end
         end
      end
   
endmodule
    
module GpioLogic(DdrReg, DataReg, P);

   parameter NUM_GPIO = 16;
  
   input  [NUM_GPIO-1:0] DdrReg; 
   input  [NUM_GPIO-1:0] DataReg; 
   inout  [NUM_GPIO-1:0] P;  

   reg     [NUM_GPIO-1:0] Gpio;   
   integer i;
   
   always @(DdrReg or DataReg)
      begin
      for (i=0; i<NUM_GPIO; i=i+1)
         begin
         if (DdrReg[i]==1'b1)
            Gpio[i] = DataReg[i];
         else
            Gpio[i] = 1'bz;
         end
      end

   assign P = Gpio;              

endmodule

module I2CCont(Addr, DataRd, DataWr, En, Rd, Wr, ICDataIn, ICDataOut, ICClk, Clk);

   input  [1:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   input  ICDataIn;
	output ICDataOut;
	output ICClk;
	input  Clk;   

	reg [15:0] DataRd;
	reg ICDataOut;
	reg ICClk;

   always @(posedge Clk)
	   begin
      if (En & Wr)
   	   begin
			if (Addr==0)
			   begin
			   ICDataOut <= DataWr[0];
			   ICClk <= DataWr[1];
				end
         end
       end

	always @(Addr or ICDataIn or ICDataOut or ICClk)
	   begin
		if (Addr==0)
		   DataRd = {13'h0000, ICDataIn, ICClk, ICDataOut};
      else
		   DataRd = 16'hxxxx;
		end

endmodule

module Primary(Addr, Data, RdN, WrN, Dq, CsN, Wait, Int, Clk, 
   Async, Asdo, Arstn, Asdi, AbitClk, AudioOut,
	ICDataOut, ICDataIn, ICClk,
	Rd, Wr, Cs, DataRd, Reset);

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

	output ICDataOut;
	input  ICDataIn;
	output ICClk;

   output Rd;
	output Wr;
	output Cs;
   input  [15:0] DataRd;
	output Reset;

	reg    [15:0] PriDataRd;

   assign Rd = ~RdN;
	assign Wr = ~WrN;
	assign Cs = ~CsN[0];

	assign Data[15:0] = Cs & Rd ? PriDataRd : 16'hzzzz;

	assign Int = 1'b0;
	assign Wait = 1'bz;
 	assign Asdi = 1'b0;

   // power-up reset (synchronous)
   reg  [1:0] ResetCount;
   always @(posedge Clk)
      begin
      if (ResetCount!=2'b11)
         ResetCount <= ResetCount + 1;
      end
   assign Reset = ResetCount!=2'b11;  // active high

	wire I2CEn;
	wire [15:0] I2CDataRd;

	assign I2CEn = Addr[11:4]==8'hfc;
   I2CCont InstI2CCont(.Addr(Addr[2:1]), .DataRd(I2CDataRd), .DataWr(Data), .En(I2CEn), .Rd(Rd), .Wr(Wr), 
	   .ICDataIn(ICDataIn), .ICDataOut(ICDataOut), .ICClk(ICClk), .Clk(Clk));

	always @(I2CDataRd or I2CEn or DataRd)
	   begin
		if (I2CEn)
		   PriDataRd = I2CDataRd;
      else
		   PriDataRd = DataRd;
      end

endmodule


module i2c(Addr, Data, RdN, WrN, Dq, CsN, Wait, Int, Clk, 
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
   reg    [15:0] Test;

   wire   Rd;
	wire   Wr;
	wire   Cs;
	wire   Reset;

	Primary InstPrimary(.Addr(Addr), .Data(Data), .RdN(RdN), .WrN(WrN), .Dq(Dq), 
	   .CsN(CsN), .Wait(Wait), .Int(Int), .Clk(Clk), 
      .Async(Async), .Asdo(Asdo), .Arstn(Arstn), .Asdi(Asdi), .AbitClk(AbitClk), .AudioOut(P[24]),
		.ICDataIn(PIn[4]), .ICDataOut(P[49]), .ICClk(P[47]),
		.Rd(Rd), .Wr(Wr), .Cs(Cs), .DataRd(DataRd), .Reset(Reset));
			 
endmodule
