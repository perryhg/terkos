module Gpio(Addr, DataRd, DataWr, En, Rd, Wr, P, Clk);

   parameter NUM_GPIO = 16;
   
   input  [3:0] Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   inout  [NUM_GPIO-1:0] P;
	input  Clk;   

   reg     [15:0] DataRd;
   reg     [NUM_GPIO-1:0] DdrReg; 
   reg     [NUM_GPIO-1:0] DataReg; 
   integer i;
   integer j;
   integer n;
               
   GpioLogic #(NUM_GPIO) InstGpioLogic(DdrReg, DataReg, P[NUM_GPIO-1:0]);
   
   always @(posedge Clk)
      begin
      for (i=0; i<(NUM_GPIO+15)/16; i=i+1)
         begin
         if ((i+1)*16>NUM_GPIO)
            n = NUM_GPIO - i*16;
         else
            n = 16; 
         if (Addr[3:0]==i & En & Wr)
            begin
            for (j=0; j<n; j=j+1)
               DdrReg[j+i*16] = DataWr[j];
            end
         if (Addr[3:0]==i+(NUM_GPIO+15)/16 && En)
            begin
            for (j=0; j<n; j=j+1)
               DataReg[j+i*16] = DataWr[j];
            end
         end
      end

   always @(Addr or Rd or En or DdrReg or P)
      begin
      DataRd = 16'hxxxx;
      for (i=0; i<(NUM_GPIO+15)/16; i=i+1)
         begin
         if ((i+1)*16>NUM_GPIO)
            n = NUM_GPIO - i*16;
         else
            n = 16; 
         if (Addr[3:0]==i)
            begin
            for (j=0; j<n; j=j+1)
               DataRd[j] = DdrReg[j+i*16];
            for (j=n; j<16; j=j+1)
               DataRd[j] = 1'b0;
            end
         if (Addr[3:0]==i+(NUM_GPIO+15)/16)
            begin
            for (j=0; j<n; j=j+1)
               DataRd[j] = P[j+i*16];
            for (j=n; j<16; j=j+1)
               DataRd[j] = 1'b0;
            end
         end
      end
   
endmodule
    
module GpioLogic(DdrReg, DataReg, P);

   parameter NUM_GPIO = 16;
  
   input  [NUM_GPIO-1:0] DdrReg; 
   input  [NUM_GPIO-1:0] DataReg; 
   inout  [NUM_GPIO-1:0] P;  

   reg     [NUM_GPIO-1:0] Gpio;   
   integer i;
   
   always @(DdrReg or DataReg)
      begin
      for (i=0; i<NUM_GPIO; i=i+1)
         begin
         if (DdrReg[i]==1'b1)
            Gpio[i] = DataReg[i];
         else
            Gpio[i] = 1'bz;
         end
      end

   assign P = Gpio;              
endmodule
