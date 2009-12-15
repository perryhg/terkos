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

module RCServo12(Addr, DataRd, DataWr, En, Rd, Wr, P, Reset, Clk);
   
   input  [4:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   output [11:0] P;   
	input  Reset;
   input  Clk;
	
   reg     [15:0] DataRd;
	
	reg     DivClk;
   reg     [8:0]  DivReg;
	reg     [8:0]  DivCounter;
	
   reg     Freq;
	reg     [13:0] FreqReg;
   reg     [13:0] FreqCounter;
	
	reg     [13:0] UpReg0;
	reg     [13:0] UpReg1;
	reg     [13:0] UpReg2;
	reg     [13:0] UpReg3;
	reg     [13:0] UpReg4;
	reg     [13:0] UpReg5;
	reg     [13:0] UpReg6;
	reg     [13:0] UpReg7;
	reg     [13:0] UpReg8;
	reg     [13:0] UpReg9;
	reg     [13:0] UpReg10;
	reg     [13:0] UpReg11;
	
   RCServoLogic InstRCServoLogic0(UpReg0, Freq, P[0], DivClk, Clk);
   RCServoLogic InstRCServoLogic1(UpReg1, Freq, P[1], DivClk, Clk);
   RCServoLogic InstRCServoLogic2(UpReg2, Freq, P[2], DivClk, Clk);
   RCServoLogic InstRCServoLogic3(UpReg3, Freq, P[3], DivClk, Clk);
   RCServoLogic InstRCServoLogic4(UpReg4, Freq, P[4], DivClk, Clk);
   RCServoLogic InstRCServoLogic5(UpReg5, Freq, P[5], DivClk, Clk);
   RCServoLogic InstRCServoLogic6(UpReg6, Freq, P[6], DivClk, Clk);
   RCServoLogic InstRCServoLogic7(UpReg7, Freq, P[7], DivClk, Clk);
   RCServoLogic InstRCServoLogic8(UpReg8, Freq, P[8], DivClk, Clk);
   RCServoLogic InstRCServoLogic9(UpReg9, Freq, P[9], DivClk, Clk);
   RCServoLogic InstRCServoLogic10(UpReg10, Freq, P[10], DivClk, Clk);
   RCServoLogic InstRCServoLogic11(UpReg11, Freq, P[11], DivClk, Clk);
   
	always @(posedge Clk)
	   begin
		if (DivCounter==DivReg) // divide 100mhz by 400 -> 4us clock
		   begin
			DivCounter <= 0;
			DivClk <= 1'b1;
			end
      else
		   begin
			DivCounter <= DivCounter + 1;
			DivClk <= 1'b0;
			end
			
		if (DivClk)
         begin
         if (FreqCounter==FreqReg)
				begin
				FreqCounter <= 0;
				Freq <= 1'b1;
				end
			else
				begin
				FreqCounter <= FreqCounter + 1;
				Freq <= 1'b0;
				end
			end
			
      end 
		
   always @(posedge Clk)
	   begin
 		if (Reset)
			begin
			UpReg0 <= 0;
			UpReg1 <= 0;
			UpReg2 <= 0;
			UpReg3 <= 0;
			UpReg4 <= 0;
			UpReg5 <= 0;
			UpReg6 <= 0;
			UpReg7 <= 0;
			UpReg8 <= 0;
			UpReg9 <= 0;
			UpReg10 <= 0;
			UpReg11 <= 0;
			DivReg <= 0;
			FreqReg <= 0;
			end
      else 
			begin
			if (Wr & En)
				begin
				if (Addr[4:0]==0)
					DivReg <= DataWr[8:0];
				else if (Addr[4:0]==1)
					FreqReg <= DataWr[13:0];
				
				else if (Addr[4:0]==2)
					UpReg0 <= DataWr[13:0];
				else if (Addr[4:0]==3)
					UpReg1 <= DataWr[13:0];
				else if (Addr[4:0]==4)
					UpReg2 <= DataWr[13:0];
				else if (Addr[4:0]==5)
					UpReg3 <= DataWr[13:0];
				else if (Addr[4:0]==6)
					UpReg4 <= DataWr[13:0];
				else if (Addr[4:0]==7)
					UpReg5 <= DataWr[13:0];
				else if (Addr[4:0]==8)
					UpReg6 <= DataWr[13:0];
				else if (Addr[4:0]==9)
					UpReg7 <= DataWr[13:0];
				else if (Addr[4:0]==10)
					UpReg8 <= DataWr[13:0];
				else if (Addr[4:0]==11)
					UpReg9 <= DataWr[13:0];
				else if (Addr[4:0]==12)
					UpReg10 <= DataWr[13:0];
				else if (Addr[4:0]==13)
					UpReg11 <= DataWr[13:0];
				end
			end
	   end
		
   always @(Addr or DivReg or FreqReg or
	   UpReg0 or UpReg1 or UpReg2 or UpReg3 or
		UpReg4 or UpReg5 or UpReg6 or UpReg7 or
		UpReg8 or UpReg9 or UpReg10 or UpReg11)
      begin
		if (Addr[4:0]==0)
		   DataRd = {7'h00, DivReg};
		else if (Addr[4:0]==1)
		   DataRd = {2'b00, FreqReg};
			
		else if (Addr[4:0]==2)
		   DataRd = {2'b00, UpReg0};
		else if (Addr[4:0]==3)
		   DataRd = {2'b00, UpReg1};
		else if (Addr[4:0]==4)
		   DataRd = {2'b00, UpReg2};
		else if (Addr[4:0]==5)
		   DataRd = {2'b00, UpReg3};
		else if (Addr[4:0]==6)
		   DataRd = {2'b00, UpReg4};
		else if (Addr[4:0]==7)
		   DataRd = {2'b00, UpReg5};
		else if (Addr[4:0]==8)
		   DataRd = {2'b00, UpReg6};
		else if (Addr[4:0]==9)
		   DataRd = {2'b00, UpReg7};
		else if (Addr[4:0]==10)
		   DataRd = {2'b00, UpReg8};
		else if (Addr[4:0]==11)
		   DataRd = {2'b00, UpReg9};
		else if (Addr[4:0]==12)
		   DataRd = {2'b00, UpReg10};
		else if (Addr[4:0]==13)
		   DataRd = {2'b00, UpReg11};	
			
		else
		   DataRd = 16'hxxxx;
      end
   
endmodule

   
module RCServoLogic(UpReg, Freq, Out, DivClk, Clk);
  
   input  [13:0] UpReg;
	input  Freq;
	output Out;
	input  DivClk;
	input  Clk;
	

   reg  [13:0] Counter;
	reg  Out;
	
   always @(posedge Clk)
	   begin
		if (DivClk)
		   begin
		   if (Freq)
		      Counter <= 0; // UpReg==0 means pwm is off
		   else
		      Counter <= Counter + 1;
				
		   if (Counter==UpReg)
            Out <= 1'b0;
		   else if (Counter==0)
            Out <= 1'b1;			  
			
         end
      end			
endmodule