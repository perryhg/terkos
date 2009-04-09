
module EdgePos(In, Out, Clk);
  
   input  In;
	output Out;
	input  Clk;

	reg PrevIn;
	
	assign Out = ~PrevIn & In;

	always @(posedge Clk)
	   PrevIn <= In;   

endmodule

/*
module EdgeNeg(In, Out, Clk);
  
   input  In;
	output Out;
	input  Clk;

	reg PrevIn;
	
	assign Out = PrevIn & ~In;

	always @(posedge Clk)
	   PrevIn <= In;   

endmodule
*/