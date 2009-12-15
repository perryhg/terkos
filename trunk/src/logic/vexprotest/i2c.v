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

//SdaOutReg SdaDir SdaOut
//    0        0     0
//	   0        1     1
//	   1        0     0
//	   1        1     0

module I2cCont(Addr, DataRd, DataWr, En, Rd, Wr, SdaOut, SdaIn, SclOut, SclIn, 
   // IntStatus, IntReset, 
   Reset, Clk);

   input  [2:0] Addr;
   output [15:0] DataRd;
	input  [15:0] DataWr;
	input  En;
	input  Rd;
	input  Wr;
	output SdaOut;
	input  SdaIn;
	output SclOut;
	input  SclIn;
//	output IntStatus;
//	input  IntReset;
	input  Reset;
	input  Clk;
	
	reg    SdaDir;
	reg    SclReg;
	reg    SdaOutReg;
	reg    [15:0] DataRd;
	
	assign SdaOut = SdaDir & ~SdaOutReg;
	assign SclOut = ~SclReg;
	
   always @(posedge Clk)
      begin
      if (Wr & En)
         begin
         if (Addr[2:0]==0)
			   {SdaDir, SclReg, SdaOutReg} <= DataWr[2:0];
			end
      end

   always @(Addr or SdaDir or SclIn or SdaIn)
      begin
	   if (Addr[2:0]==0)
		   DataRd = {13'h0000, SdaDir, SclIn, SdaIn};
		else 
		   DataRd = 16'hxxxx;
		end
		
endmodule

