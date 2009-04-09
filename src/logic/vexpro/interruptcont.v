module InterruptCont(Addr, DataRd, DataWr, En, Rd, Wr, IntStatus, IntReset, Int, Reset, Clk);
   
   input  Addr;
   output [15:0] DataRd; 
   input  [15:0] DataWr;
   input  En;
   input  Rd;
   input  Wr;
   input  [15:0] IntStatus;
   output [15:0] IntReset;
   output Int;
   input  Reset;
   input  Clk;

   reg  [15:0] DataRd;
   reg  [15:0] IntMask;
   reg  [15:0] IntReset;
   wire [15:0] IntFilt;

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
               IntMask <= DataWr;
            else if (Addr==1)
               IntReset <= DataWr;
            end
         else
            IntReset <= 0; 
         end
      end

   always @(Addr or IntMask or IntFilt)
      begin
      if (Addr==0)
         DataRd = IntMask;
      else if (Addr==1)
         DataRd = IntFilt;
      else
         DataRd = 16'hxxxx;
      end

endmodule
