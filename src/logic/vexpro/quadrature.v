module Quadrature4(Addr, DataRd, DataWr, En, Rd, Wr, A, B, Clk);
   
   input  [2:0] Addr;
   output [15:0] DataRd; 
	input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   input  [3:0] A;   
   input  [3:0] B;   
   input  Clk;

   reg    [15:0] DataRd;
   wire   [15:0] QuadraReg0;
   wire   [15:0] QuadraReg1;
   wire   [15:0] QuadraReg2;
   wire   [15:0] QuadraReg3;
   reg    [5:0] CountDiv;
	
	// 6000 RPM -> 100 rev/s * 1000 ticks/rev = 100000 ticks/s
	// ClkDiv = 100 MHz / 64 = 1.56 MHz or 15x the tick rate
	// noisefilt = 10 ClkDiv periods
	
   QuadratureLogic Inst1QuadratureLogic(.InA(A[0]), .InB(B[0]), .Count(QuadraReg0), .ClkDiv(CountDiv[5]), .Clk(Clk));
   QuadratureLogic Inst2QuadratureLogic(.InA(A[1]), .InB(B[1]), .Count(QuadraReg1), .ClkDiv(CountDiv[5]), .Clk(Clk));
   QuadratureLogic Inst3QuadratureLogic(.InA(A[2]), .InB(B[2]), .Count(QuadraReg2), .ClkDiv(CountDiv[5]), .Clk(Clk));
   QuadratureLogic Inst4QuadratureLogic(.InA(A[3]), .InB(B[3]), .Count(QuadraReg3), .ClkDiv(CountDiv[5]), .Clk(Clk));
      
   always @(posedge Clk)
      begin
		
		CountDiv <= CountDiv + 1;
		
//      if (Wr & En)
//         begin
//         if (Addr[2:0]==0)
      end
		
   always @(Addr or QuadraReg0 or QuadraReg1 or QuadraReg2 or QuadraReg3)
      begin
	   if (Addr[2:0]==0)
		   DataRd = QuadraReg0;
		else if (Addr[2:0]==1)
		   DataRd = QuadraReg1;
		else if (Addr[2:0]==2)
		   DataRd = QuadraReg2;
		else if (Addr[2:0]==3)
		   DataRd = QuadraReg3;
		else 
		   DataRd = 16'hxxxx;
		end

endmodule
   
module NoiseFilter(In, Out, ClkDiv, Clk);

   parameter NUM_REJECT  = 2;
   parameter BITS_FILTER = 2;

   input  In;
   output Out;
	input  ClkDiv;
   input  Clk;

   reg    Out;
   reg    [BITS_FILTER-1:0] FilterCount;
   wire   ClkDivPosEdge;
	
   EdgePos InstEdgePos(.In(ClkDiv), .Out(ClkDivPosEdge), .Clk(Clk));
	
   always @(posedge Clk)
      begin
		if (ClkDivPosEdge==1'b1)
		   begin
         if (In != Out)
            begin           
            if (FilterCount >= NUM_REJECT)
               begin
               FilterCount <= 0;
               Out <= In;
               end
            else
               FilterCount <= FilterCount + 1;
            end
         else
            FilterCount <= 0;
         end  
      end		
   endmodule

module QuadratureLogic(InA, InB, Count, ClkDiv, Clk);

   input   InA;
   input   InB;
   output  [15:0] Count;
	input   ClkDiv;
   input   Clk;
   
   wire    InAp;
   wire    InBp;
   reg     [15:0] Count;
   reg     [1:0] Prev;
  
   NoiseFilter #(10, 4) Inst1NoiseFilter(InA, InAp, ClkDiv, Clk);
   NoiseFilter #(10, 4) Inst2NoiseFilter(InB, InBp, ClkDiv, Clk);

   always @(posedge Clk)
      begin
      Prev <= {InAp, InBp};
      case (Prev)
         2'b00: begin
            if ({InAp, InBp}==2'b01 | {InAp, InBp}==2'b11)
               Count <= Count + 1;
            else if ({InAp, InBp}==2'b10)
               Count <= Count - 1; 
            else 
               Count <= Count; 
            end 
         2'b01: begin
            if ({InAp, InBp}==2'b11)
               Count <= Count + 1;
            else if ({InAp, InBp}==2'b00)
               Count <= Count - 1; 
            else 
               Count <= Count; 
            end 
         2'b10: begin
            if ({InAp, InBp}==2'b00)
               Count <= Count + 1;
            else if ({InAp, InBp}==2'b11)
               Count <= Count - 1; 
            else 
               Count <= Count; 
            end 
         2'b11: begin
            if ({InAp, InBp}==2'b10 | {InAp, InBp}==2'b00)
               Count <= Count + 1;
            else if ({InAp, InBp}==2'b01)
               Count <= Count - 1; 
            else 
               Count <= Count; 
            end 
      endcase 
   end
endmodule
