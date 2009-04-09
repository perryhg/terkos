module GpioInt(Addr, DataRd, DataWr, En, Rd, Wr, PortIn, PortOut, IntStatus, IntReset, Reset, Clk);

   input  [2:0] Addr;
	output [15:0] DataRd;
	input  [15:0] DataWr;
	input  En;
	input  Rd;
	input  Wr;
	input  [7:0] PortIn;
	output [7:0] PortOut;
	output [7:0] IntStatus;
	input  [7:0] IntReset;
	input  Reset;
	input  Clk;

	reg  [15:0] DataRd;
	reg  [7:0] PortOut;
	reg  [7:0] IntStatus;
	reg  [7:0] IntMask;
	reg  [7:0] IntEdge;
	reg  [7:0] PortInSync;
	reg  [7:0] PortInPrev;
	wire [7:0] PortInPos;

	assign PortInPos = (PortInPrev^IntEdge)&(~PortInSync^IntEdge);

	always @(posedge Clk)
	   begin
		PortInSync <= PortIn;
		PortInPrev <= PortInSync;

		if (Reset)
		   begin
			PortOut <= 0;
			IntStatus <= 0;
			IntMask <= 0;
			IntEdge <= 0;
			end
      else
		   begin
			if (IntReset!=0) // reset status bits
			   IntStatus <= IntStatus & ~IntReset;
		   else
			   IntStatus <= (IntStatus | PortInPos)&IntMask;	 // IntMask will reset interrupt
         
			if (En & Wr)
			   begin
			   if (Addr==0)
				   PortOut <= DataWr[15:8];
        		else if (Addr==1)
				   IntMask <= DataWr[7:0];
        		else if (Addr==2)
				   IntEdge <= DataWr[7:0];
				end
			end
      end

   always @(En or Addr or PortIn or PortOut or IntMask or IntEdge or IntStatus)
	   begin
	   if (En & Addr==0)
	      DataRd = {PortOut, PortIn};
      else if (En & Addr==1)
	      DataRd = {8'h00, IntMask};
      else if (En & Addr==2)
	      DataRd = {8'h00, IntEdge};
      else if (En & Addr==3)
	      DataRd = {8'h00, IntStatus};
		else 
		   DataRd = 16'hxxxx;      
		end
endmodule