//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

module InterruptCont(Addr, DataRd, DataWr, En, Rd, Wr, IntStatus, IntReset, Int, Reset, Clk);

   input  [2:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   input  [31:0] IntStatus;
   output [31:0] IntReset;
   output Int;
   input  Reset;
   input  Clk;

   reg  [15:0] DataRd;
   reg  [31:0] IntMask;
   reg  [31:0] IntReset;
   wire [31:0] IntFilt;

   assign IntFilt = IntStatus&IntMask;
   assign Int = IntFilt!=0;

   always @(posedge Clk)
      begin

      if (Reset)
         IntMask <= 0;
      else
         begin
         if (Wr & En)
            begin
            if (Addr==0)
               IntMask[15:0] <= DataWr;
            else if (Addr==1)
               IntMask[31:16] <= DataWr;
            else if (Addr==2)
               IntReset[15:0] <= DataWr;
            else if (Addr==3)
               IntReset[31:16] <= DataWr;
            end
         else
            IntReset <= 0; 
         end
      end

   always @(Addr or IntMask or IntFilt)
      begin
      if (Addr==0) // little endian, lsw
         DataRd = IntMask[15:0];
      else if (Addr==1) // msw
         DataRd = IntMask[31:16];
      else if (Addr==2) // msw
         DataRd = IntFilt[15:0];
      else if (Addr==3) // msw
         DataRd = IntFilt[31:16];
		else
         DataRd = 16'hxxxx;
      end

endmodule
