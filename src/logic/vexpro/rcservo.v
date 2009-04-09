module RCServo(Addr, DataRd, DataWr, En, Rd, Wr, P, Clk);

   parameter NUM_SERVO = 2;
   
   input  [4:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   output [NUM_SERVO-1:0] P;   
   input  Clk;

   reg     [15:0] DataRd;
   reg     [NUM_SERVO*8-1:0] PwmReg;
	reg     [4:0] DivCount;
	reg     DivClk;
	reg     Enable;
   integer i;
   integer j;
               
   RCServoLogic #(NUM_SERVO) InstRCServoLogic(PwmReg, P[NUM_SERVO-1:0], DivClk&Enable);
   
	always @(posedge Clk)
	   begin
		if (DivCount==22)
		   begin
			DivCount <= 0;
			DivClk <= 1'b1;
			end
      else
		   begin
			DivCount <= DivCount + 1;
		   DivClk <= 1'b0;
			end
		end

   always @(posedge Clk)
      begin
		if (Wr & En)
		   begin
		   if (Addr[4:0]==5'b11111)
		      Enable <= DataWr[0];
         for (i=0; i<NUM_SERVO/2; i=i+1)
            begin
            if (Addr[4:0]==i)
               begin
               for (j=0; j<16; j=j+1)
                  PwmReg[j+i*16] <= DataWr[j];
               end
            end
         end
      end

   always @(Addr or PwmReg or Enable)
      begin
      DataRd = 16'hxxxx;
		if (Addr[4:0]==5'b11111)
		   DataRd = {14'h0000, Enable};
      for (i=0; i<NUM_SERVO/2; i=i+1)
         begin
         if (Addr[4:0]==i)
            begin
            for (j=0; j<16; j=j+1)
               DataRd[j] = PwmReg[j+i*16];
            end
         end
      end
   
endmodule

   
module RCServoLogic(ChannelReg, ChannelOut, Clk);

   parameter NUM_PWM = 2;
  
   input  [NUM_PWM*8-1:0] ChannelReg;
   output [NUM_PWM-1:0] ChannelOut;
   input  Clk;

   reg  [NUM_PWM-1:0] ChannelOut;
   reg  ClkDiv2;
   reg  ClkDiv4;
   reg  ClkDiv8;
   reg  ClkDiv16;
   reg  ClkDiv32;
   reg  ActiveRegion;
   reg  SubRegion;
   reg  [NUM_PWM-1:0] PreOut;
   reg  [10:0] Counter;
   reg  [7:0] Channel;

   integer i;
   integer j;

   always @(posedge Clk)
      begin
      {ClkDiv32, ClkDiv16, ClkDiv8, ClkDiv4, ClkDiv2} = {ClkDiv32, ClkDiv16, ClkDiv8, ClkDiv4, ClkDiv2} + 1;
      end

   always @(posedge ClkDiv32)
      begin
      Counter <= Counter + 1;
      for (i=0; i<NUM_PWM; i=i+1)
         ChannelOut[i] <= PreOut[i];
      end

   always @(Counter)
      begin
      if (Counter<12'h130)
         ActiveRegion = 1'b1;
      else
         ActiveRegion = 1'b0;
      if (Counter<12'h100)
         SubRegion = 1'b0;
      else
         SubRegion = 1'b1;
      end

   always @(Counter or SubRegion or ActiveRegion or ChannelReg or Channel or i)
      begin
      for (i=0; i<NUM_PWM; i=i+1)
         begin
         for (j=0; j<8; j=j+1)
            Channel[j] = ChannelReg[j+i*8];
         PreOut[i] = ((Channel[7:0]<Counter[7:0]) | SubRegion) & ActiveRegion;
         end
      end
             
endmodule