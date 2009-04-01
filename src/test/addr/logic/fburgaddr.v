module fburgaddr(Addr, Data, Rd, Wr, Dq, Cs, Wait, Int, Clk, 
   Async, Asdo, Arstn, Asdi, AbitClk);

	input  [11:0] Addr;
	inout  [15:0] Data;
	input  Rd;
	input  Wr;
	input  [1:0] Dq;
	input  [1:0] Cs;
	output Wait;
	output Int;
	input  Clk;

	input  Async;
	input  Asdo;
	input  Arstn;
	output Asdi;
	input  AbitClk;

	reg    [15:0] DataRd;
   reg    [15:0] Test;

	assign Data[15:0] = ~Cs[0] & ~Rd ? DataRd : 16'hzzzz;
			 
   always @(posedge Clk)
	   begin
      if (~Cs[0])
		   begin
			if (~Wr)
			   begin
			   if (Addr[11:0]==2)
			      Test <= Data;
            end
         if (~Rd)
			   begin
				if (Addr[11:0]==0)
				   DataRd <= 16'hbeef;
				else if (Addr[11:0]==2)
				   DataRd <= Test;
            end
         end
		end

	assign Int = 1'b0;
	assign Asdi = 1'b0;
	assign Wait = 1'bz;


endmodule
