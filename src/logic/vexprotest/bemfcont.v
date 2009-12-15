//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v3 (http://www.gnu.org/licenses/gpl-3.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//


module Exchange(DataIn, DataOut, SerialIn, SerialOut, SerialDir, Cs, Start, Busy, DivClk, Clk, Reset);

   input  [3:0] DataIn;
   output [12:0] DataOut;
   input  SerialIn;
   output SerialOut;
	output SerialDir;
   output Cs;
   input  Start;
   output Busy;
   input  DivClk;
   input  Clk;
   input  Reset;

   reg  [2:0] State;
   reg  SerialOut;
   wire PositiveEdge;
   wire NegativeEdge;
   reg  DivClkPrev;
   reg  [3:0] Count;
   reg  [12:0] DataOut;

   assign PositiveEdge = ~DivClkPrev & DivClk; 
   assign NegativeEdge = DivClkPrev & ~DivClk; 
   assign Busy = State!=0;
   assign Cs = State<2;
	assign SerialDir = ~State[2];

   always @(posedge Clk)
      DivClkPrev <= DivClk;

   always @(posedge Clk)
      begin
      if (Reset)
         State <= 0;
      else
         begin
         case (State)
         0: // wait for start signal
            begin
            Count <= 0;
            if (Start) 
               State <= 1;
            end

         1: // wait for NegativeEdge
            begin 
            if (NegativeEdge)
               State <= 2;
            end

         2: // send start bit
            begin
            Count <= 3;
            if (NegativeEdge)
               State <= 3;
            end

         3: // send  bits
            begin
            if (NegativeEdge)
               begin
               Count <= Count - 1;
               if (Count==0)
                  State <= 4;
               end
            end

          4: // wait
             begin
             if (PositiveEdge)
                State <= 5;
             end

          5: // wait
             begin
             Count <= 12;
             if (PositiveEdge)
                State <= 6;
             end

          6: // receive bits
            begin
            if (PositiveEdge)
               begin
               DataOut[Count] <= SerialIn;
               if (Count==0)
                  State <= 0;
               Count <= Count - 1;
               end
            end
          endcase
          end 
      end

   always @(State or DataIn or Count)
      begin
      if (State==2)
         SerialOut = 1'b1;
      else if (State==3)
         SerialOut = DataIn[Count];
      else
         SerialOut = 1'b0;
      end

   endmodule   

 module BemfBlockRam(Addr, DataRd, DataWr, En, Wr, Clk);

   input  [9:0] Addr;
   output [15:0] DataRd;
   input  [15:0] DataWr;
   input  En;
   input  Wr;
   input  Clk;

   RAMB16_S18 Ram0(.WE(Wr), .EN(En), .SSR(1'b0), .ADDR(Addr), .CLK(Clk), .DI(DataWr), .DIP(2'B00), .DO(DataRd));

endmodule

module PwmLogic(PwmReg, PwmOut, ClkDiv, Clk,
   ClkDivOut2, ClkDivOut3, ClkDivOut4, ClkDivOut5);

   parameter NUM_PWM = 1;
  
   input  [NUM_PWM*8-1:0] PwmReg;
   output [NUM_PWM-1:0] PwmOut;
   input  ClkDiv;
   input  Clk;
	output ClkDivOut2;
	output ClkDivOut3;
	output ClkDivOut4;
	output ClkDivOut5;

   reg     [NUM_PWM-1:0] PwmOut;
   reg     [NUM_PWM-1:0] PreOut;
   reg	  [7:0] Counter;
   reg     [7:0] Channel;
	wire    PosClkDiv;
   integer i;
   integer j;

	assign ClkDivOut2 = Counter[2];
	assign ClkDivOut3 = Counter[3];
	assign ClkDivOut4 = Counter[4];
	assign ClkDivOut5 = Counter[5];

	EdgePos InstEdgePos(.In(ClkDiv), .Out(PosClkDiv), .Clk(Clk));
 
   always @(posedge Clk)
      begin
		if (PosClkDiv)
         Counter <= Counter + 1;
      for (i=0; i<NUM_PWM; i=i+1)
         PwmOut[i] <= PreOut[i];
      end

   always @(Counter or PwmReg)
      begin
      for (i=0; i<NUM_PWM; i=i+1)
         begin
         for (j=0; j<8; j=j+1)
            Channel[j] = PwmReg[j+i*8];            
         PreOut[i] = Channel[7:0]>=Counter[7:0];
         end
      end
             
endmodule

module BemfCont4(Addr, DataRd, DataWr, En, Rd, Wr, PwmOut, PwmCont, AxisActive, AxisMeasure,
   AdcIn, AdcOut, AdcDir, AdcCs, AdcClk,  
	IntStatus, IntReset, Reset, Clk);

   input  [8:0] Addr;
   output [15:0] DataRd;
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   output [3:0] PwmOut;
   output [7:0] PwmCont;
	output [3:0] AxisActive;
	output [3:0] AxisMeasure;
   input  AdcIn;
	output AdcOut;
	output AdcDir;
   output AdcCs;
   output AdcClk;
	output IntStatus;
	input  IntReset;
   input  Reset;
   input  Clk;

   reg  DataRd;
   reg  [7:0] AdcDivCount;
   reg  [7:0] ActiveInterval;
   reg  [7:0] WaitInterval;
   reg  [3:0] BemfReadings;
   reg  [31:0] PwmReg;
   wire [3:0] PrePwmOut;
   reg  [3:0] PwmOut;
   wire PwmClkDiv;
   reg  [7:0] PwmCont;
   reg  [7:0] Count;
   reg  [1:0] Axis;
   wire PwmClkDivPosEdge;
	reg  IntStatus;

   reg  AdcClk;
   wire AdcStart;
	wire [3:0] AdcDataIn;
   wire [12:0] AdcDataOut;
   wire AdcBusy;
   wire RamWr;
   reg  [3:0] AdcState;
   reg  [7:0] RamCount;
   wire [15:0] RamDataRd;
   wire [7:0] RamAddr;
   reg  [3:0] BemfDisable;
   wire Active;
	reg  [7:0] ClkDiv;

	assign AdcDataIn = {1'b0, Axis, 1'b0};
   assign AdcStart = AdcState==2;
   assign RamWr = AdcState==4;
   assign Active = AdcState==0;
   assign RamAddr = Active ? Addr[7:0] : RamCount;
   assign AxisActive[0] = Active | Axis!=0 | BemfDisable[0];
   assign AxisActive[1] = Active | Axis!=1 | BemfDisable[1];
   assign AxisActive[2] = Active | Axis!=2 | BemfDisable[2];
   assign AxisActive[3] = Active | Axis!=3 | BemfDisable[3];
   assign AxisMeasure[0] = AdcState>1 & Axis==0;
   assign AxisMeasure[1] = AdcState>1 & Axis==1;
   assign AxisMeasure[2] = AdcState>1 & Axis==2;
   assign AxisMeasure[3] = AdcState>1 & Axis==3;

   EdgePos InstEdgePos(.In(PwmClkDiv), .Out(PwmClkDivPosEdge), .Clk(Clk));
   PwmLogic #(4) InstPwmLogic(.PwmReg(PwmReg), .PwmOut(PrePwmOut), .ClkDiv(ClkDiv[7]), .Clk(Clk), .ClkDivOut3(PwmClkDiv));

   Exchange InstExchange(.DataIn(AdcDataIn), .DataOut(AdcDataOut), .SerialIn(AdcIn), .SerialOut(AdcOut), .SerialDir(AdcDir),  
	   .Cs(AdcCs), .Start(AdcStart), .Busy(AdcBusy), .DivClk(AdcClk), .Clk(Clk), .Reset(Reset));

   BemfBlockRam InstBlockRam(.Addr({2'b00, RamAddr}), .DataRd(RamDataRd), .DataWr({AdcDataOut, 3'b000}), //.DataWr({2'b00, AdcState, Axis, Count}),
      .En(1'b1), .Wr(RamWr), .Clk(Clk));
  
	always @(posedge Clk)
	   ClkDiv <= ClkDiv + 1;

   always @(posedge Clk)
      begin
      PwmOut <= PrePwmOut; // & (AxisActive | BemfDisable);
      end

   always @(posedge Clk)
      begin
      if (AdcDivCount==40)
         begin
         AdcDivCount <= 0;
         AdcClk <= ~AdcClk;
         end
      else
         AdcDivCount <= AdcDivCount + 1;
      end

   always @(posedge Clk)
      begin
      if (Reset)
	      begin
         AdcState <= 0;
	      Count <= 0;
			IntStatus <= 1'b0;
	      end
      else
         begin

			if (IntReset)
			   IntStatus <= 1'b0;
         else if  (AdcState==5 & Count=={BemfReadings, 2'b11} & Axis==3)
			   IntStatus <= 1'b1;

         case (AdcState)
         0: // Active
            begin
            Axis <= 0;
            if (Count==ActiveInterval)
               begin
               RamCount <= 0;
               AdcState <= 1;
					Count <= 0;
               end
            else if (PwmClkDivPosEdge)
               Count <= Count + 1;
            end

			1: // wait for wait period to end
            begin
            if (Count==WaitInterval)
               begin
               Count <= 0;
               AdcState <= 2;
               end
            else if (PwmClkDivPosEdge)
               Count <= Count + 1;
            end
				
         2: // assert start
            begin
            AdcState <= 3;
            end

         3: // wait for adc exchange
            begin
            if (~AdcBusy)
               AdcState <= 4;
            end 

         4: // write to memory
            begin
            AdcState <= 5;
            end

         5: // increment
            begin
            Count <= Count + 1;
            RamCount <= RamCount + 1;
            if (Count=={BemfReadings, 2'b11})
               begin
               Count <= 0;
		  	      if (Axis==3)
                  AdcState <= 0;
               else
			         begin
                  Axis <= Axis + 1;
                  AdcState <= 1;
			         end
               end
            else
               AdcState <= 2;
            end

         endcase
         end
      end

   always @(posedge Clk)
      begin
      if (Wr & En)
         begin
         if (Addr[2:0]==0)
            begin
            PwmReg[7:0] <= DataWr[7:0];
            PwmCont[1:0] <= DataWr[9:8];
				BemfDisable[0] <= DataWr[10];
            end
         else if (Addr[2:0]==1)
            begin
            PwmReg[15:8] <= DataWr[7:0];
            PwmCont[3:2] <= DataWr[9:8];
				BemfDisable[1] <= DataWr[10];
            end
         else if (Addr[2:0]==2)
            begin
            PwmReg[23:16] <= DataWr[7:0];
            PwmCont[5:4] <= DataWr[9:8];
				BemfDisable[2] <= DataWr[10];
            end
         else if (Addr[2:0]==3)
            begin
            PwmReg[31:24] <= DataWr[7:0];
            PwmCont[7:6] <= DataWr[9:8];
				BemfDisable[3] <= DataWr[10];
            end
         else if (Addr[2:0]==4)
            begin
            ActiveInterval <= DataWr[7:0];
            BemfReadings <= DataWr[11:8];
            end
         else if (Addr[2:0]==5)
			   begin
            WaitInterval <= DataWr[7:0];
				end	
         end
      end

   always @(Addr or AdcBusy or PwmReg or RamCount or RamDataRd or 
      BemfReadings or WaitInterval or ActiveInterval or 
		PwmCont or Active or BemfDisable)
      begin
      if (Addr[8])
         DataRd = RamDataRd;
      else
	      begin
	      if (Addr[2:0]==0)
            DataRd = {5'h00, BemfDisable[0], PwmCont[1:0], PwmReg[7:0]};
         else if (Addr[2:0]==1)
            DataRd = {5'h00, BemfDisable[1], PwmCont[3:2], PwmReg[15:8]};
         else if (Addr[2:0]==2)
            DataRd = {5'h00, BemfDisable[2], PwmCont[5:4], PwmReg[23:16]};
         else if (Addr[2:0]==3)
            DataRd = {5'h00, BemfDisable[3], PwmCont[7:6], PwmReg[31:24]};
         else if (Addr[2:0]==4)
            DataRd = {4'h0, BemfReadings, ActiveInterval}; 
         else if (Addr[2:0]==5)
            DataRd = {8'h00, WaitInterval};
         else if (Addr[2:0]==6)
            DataRd = {15'h0000, Active}; 
         else
            DataRd = 16'hxxxx;
         end
      end

endmodule

