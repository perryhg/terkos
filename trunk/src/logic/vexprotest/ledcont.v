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

module Pwm(Counter, Duty, Out, DivClk, Clk);

   input  [7:0] Counter;
	input  [7:0] Duty;
	output Out;
	input  DivClk;
	input  Clk;
	
	reg    Out;
	
   always @(posedge Clk)
	   begin
		if (DivClk)
			begin
			if (Counter==Duty)
			   Out <= 1'b0;
			else if (Counter==0)
			   Out <= 1'b1;
			end
       end
		 
endmodule		 
	
module LedCont(Addr, DataRd, DataWr, En, Rd, Wr, 
				LedGreen, LedRed, Reset, Clk);

   input  [2:0] Addr;
   output [15:0] DataRd;
	input  [15:0] DataWr;
	input  En;
	input  Rd;
	input  Wr;
	output [2:0] LedGreen;
	output [2:0] LedRed;
	input  Reset;
	input  Clk;
	
	wire   DivClk;
	
	reg    [15:0] DataRd;
	reg    [11:0] DivCounter;
	reg    [7:0] PwmCounter;
   reg    [7:0] DutyR0;
   reg    [7:0] DutyR1;
   reg    [7:0] DutyR2;
   reg    [7:0] DutyG0;
   reg    [7:0] DutyG1;
   reg    [7:0] DutyG2;
 

	
	EdgePos InstEdgePos(.In(DivCounter[11]), .Out(DivClk), .Clk(Clk));
	Pwm InstPwmR0(.Counter(PwmCounter), .Duty(DutyR0), .Out(LedRed[0]), .DivClk(DivClk), .Clk(Clk));
	Pwm InstPwmR1(.Counter(PwmCounter), .Duty(DutyR1), .Out(LedRed[1]), .DivClk(DivClk), .Clk(Clk));
	Pwm InstPwmR2(.Counter(PwmCounter), .Duty(DutyR2), .Out(LedRed[2]), .DivClk(DivClk), .Clk(Clk));
	Pwm InstPwmG0(.Counter(PwmCounter), .Duty(DutyG0), .Out(LedGreen[0]), .DivClk(DivClk), .Clk(Clk));
	Pwm InstPwmG1(.Counter(PwmCounter), .Duty(DutyG1), .Out(LedGreen[1]), .DivClk(DivClk), .Clk(Clk));
	Pwm InstPwmG2(.Counter(PwmCounter), .Duty(DutyG2), .Out(LedGreen[2]), .DivClk(DivClk), .Clk(Clk));
	
   always @(posedge Clk)
	   begin
		DivCounter <= DivCounter + 1;
		
		if (DivClk)
		   begin
			PwmCounter <= PwmCounter + 1;
			end
		end	
		
   always @(posedge Clk)
      begin
		if (Reset)
			begin
			DutyR0 <= 0;
			DutyR1 <= 0;
			DutyR2 <= 0;
			DutyG0 <= 0;
			DutyG1 <= 0;
			DutyG2 <= 0;
			end
		else if (Wr & En)
         begin
         if (Addr[2:0]==0)
			   {DutyG0, DutyR0} <= DataWr;
         else if (Addr[2:0]==1)
			   {DutyG1, DutyR1} <= DataWr;
         else if (Addr[2:0]==2)
			   {DutyG2, DutyR2} <= DataWr;
			end
      end

   always @(Addr or DutyG0 or DutyR0 or DutyG1 or DutyR1 or DutyG2 or DutyR2)
      begin
	   if (Addr[2:0]==0)
		   DataRd = {DutyG0, DutyR0};
		else if (Addr[2:0]==1)
		   DataRd = {DutyG1, DutyR1};
		else if (Addr[2:0]==2)
		   DataRd = {DutyG2, DutyR2};
		else	
		   DataRd = 16'hxxxx;
		end
		
endmodule

