
module LedCont(Addr, DataRd, DataWr, En, Rd, Wr, 
				LedGreen, LedRed, Reset, Clk);

   input  [1:0] Addr;
   output [15:0] DataRd;
	input  [15:0] DataWr;
	input  En;
	input  Rd;
	input  Wr;
	output [2:0] LedGreen;
	output [2:0] LedRed;
	input  Reset;
	input  Clk;
	
	reg    [2:0] LedGreen;
	reg    [2:0] LedRed;
	reg    [15:0] DataRd;
	
	
   always @(posedge Clk)
      begin
		if (Reset)
			begin
			LedGreen <= 3'b000;
			LedRed <= 3'b000;
			end
		else if (Wr & En)
         begin
         if (Addr[1:0]==0)
			   {LedRed, LedGreen} <= DataWr[5:0];
			end
      end

   always @(Addr or LedGreen or LedRed)
      begin
	   if (Addr[1:0]==0)
		   DataRd = {10'h000, LedRed, LedGreen};
		else 
		   DataRd = 16'hxxxx;
		end
		
endmodule

