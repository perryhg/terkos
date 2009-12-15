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

module GpioInt(Addr, DataRd, DataWr, En, Rd, Wr, Port, IntStatus, IntReset, Reset, Clk);

   input  [2:0] Addr;
	output [15:0] DataRd;
	input  [15:0] DataWr;
	input  En;
	input  Rd;
	input  Wr;
	inout  [15:0] Port;
	output [15:0] IntStatus;
	input  [15:0] IntReset;
	input  Reset;
	input  Clk;

	reg  [15:0] DataRd;
	reg  [15:0] DataDir;
	reg  [15:0] DataOut;
	reg  [15:0] IntStatus;
	reg  [15:0] IntFilt;
	reg  [15:0] IntEdge;
	reg  [15:0] PortSync;
	reg  [15:0] PortSyncPrev;
	reg  [15:0] IntFiltPrev;
	reg  [15:0] IntFiltEdge;
	reg  [7:0] DivCount;
	wire DivClk;
	
	EdgePos InstEdgePos(.In(DivCount[6]), .Out(DivClk), .Clk(Clk));
		
	assign Port[0]  = DataDir[0]  ? DataOut[0]  : 1'bZ;
	assign Port[1]  = DataDir[1]  ? DataOut[1]  : 1'bZ;
	assign Port[2]  = DataDir[2]  ? DataOut[2]  : 1'bZ;
	assign Port[3]  = DataDir[3]  ? DataOut[3]  : 1'bZ;
	assign Port[4]  = DataDir[4]  ? DataOut[4]  : 1'bZ;
	assign Port[5]  = DataDir[5]  ? DataOut[5]  : 1'bZ;
	assign Port[6]  = DataDir[6]  ? DataOut[6]  : 1'bZ;
	assign Port[7]  = DataDir[7]  ? DataOut[7]  : 1'bZ;
	assign Port[8]  = DataDir[8]  ? DataOut[8]  : 1'bZ;
	assign Port[9]  = DataDir[9]  ? DataOut[9]  : 1'bZ;
	assign Port[10] = DataDir[10] ? DataOut[10] : 1'bZ;
	assign Port[11] = DataDir[11] ? DataOut[11] : 1'bZ;
	assign Port[12] = DataDir[12] ? DataOut[12] : 1'bZ;
	assign Port[13] = DataDir[13] ? DataOut[13] : 1'bZ;
	assign Port[14] = DataDir[14] ? DataOut[14] : 1'bZ;
	assign Port[15] = DataDir[15] ? DataOut[15] : 1'bZ;

	always @(posedge Clk)
	   begin
		if (DivClk)
			begin
			PortSync <= Port;
			PortSyncPrev <= PortSync;
			end

		if (Reset)
		   begin
			DataDir <= 0;
			DataOut <= 0;
			IntStatus <= 0;
			IntEdge <= 0;
			end
      else
		   begin
			DivCount <= DivCount + 1;
			IntFilt <= (PortSyncPrev^IntEdge)&(~PortSync^IntEdge);
			IntFiltPrev <= IntFilt;
			IntFiltEdge <= IntFiltPrev & ~IntFilt;
			
			IntStatus <= ~IntReset & (IntStatus | IntFiltEdge);
		
			if (En & Wr)
			   begin
			   if (Addr==0)
				   DataOut <= DataWr;
        		else if (Addr==1)
				   DataDir <= DataWr;
        		else if (Addr==2)
				   IntEdge <= DataWr;
				end
			end
      end

   always @(En or Addr or Port or DataDir or IntEdge or IntFilt)
	   begin
	   if (En & Addr==0)
			DataRd = Port;
	   else if (En & Addr==1)
	      DataRd = DataDir;
      else if (En & Addr==2)
	      DataRd = IntEdge;
		else 
		   DataRd = 16'hxxxx;      
		end
endmodule