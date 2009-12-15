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

// todo
// Change 12 bit to 10 bit rendering
// replace volume with 18x10 lookup
// Add registers for turning off/on speaker
// Add registers for different formats, unsigned? 8 bit? 16 bit? 24 bit? 32 bit?
// cleanup-- make everything synchronous, remove extra bits that aren't used


module AudioDAC(Async, Asdo, Arstn, Asdi, AbitClk, Out, Reset, Clk,
	Addr, DataRd, DataWr, En, Rd, Wr);
	// 2nd line of AudioDAC is for debugging
	
	// Original IO
	input  Async;
	input  Asdo;
	input  Arstn;
	output Asdi;
	input  AbitClk;
	output Out;
	input  Clk;
	input  Reset;
	
	// Original Regs
	reg [11:0] DivCount;
	reg [11:0] TimeoutCount;
	reg [11:0] LeftAudioData;
	reg [11:0] RightAudioData;
	reg [11:0] RightInputReg;
	reg [11:0] LeftInputReg;
	reg AsyncEdgeDetect;
	reg WaveOut;
	reg Out;
	reg [15:0] MixedAudioData;
	reg [15:0] AmpAudioData;
	reg [11:0] MixedCompare;
	reg [15:0] MixedPrev;
	reg [1:0] Mode;
	reg Oldsign;
	
   reg AbitClkSync;
	reg AsyncSync;
	reg AsdoSync;
	reg AbitClkEdgeDetect;
	reg [3:0] BitCount;
	
	// Debugging IO
	input [3:0] Addr;
	output [15:0] DataRd;
	input [15:0] DataWr;
	input En;
	input Rd;
	input Wr;
	
    // Tone Generator Reg
	reg [7:0] VolumeAcc;
	reg [7:0] VolumeData;
	reg VolumeOut;
	reg [20:0] FreqAcc;
	reg [15:0] FreqData;
	reg FreqOut;
	reg [15:0] DataRd;
		
	// Asdi = TO Audio Sound Input. Since we're not digitizing, we'll tie this low.
	assign Asdi = 1'b0;
	// Asdo = TO Audio Sound Output. This is our input.
	// Async = LRCK on spec sheet. (frame latch)
	// ARSTn = Sampling rate (not used for output)
	//	Abitclk = data IO clock
	
	//----------------End Declarations

//  IO Handler
   always @(posedge Clk)
   begin
      if (Reset)
         begin
			VolumeData <= 8'h20;
			Mode <= 2'b10;
			FreqData <= 16'h0000;
		   end
		else if (En & Wr)
	   	begin
			if (Addr == 0)  
			   Mode <= DataWr[1:0];
			else if (Addr == 1)
				VolumeData <= DataWr;
			else if (Addr == 2)
				FreqData <= DataWr;
		end
	end
	
   always @(posedge Clk)
   begin 
       AbitClkSync <= AbitClk;
       AsyncSync <= Async;
       AsdoSync <= Asdo;
   end
	
	always @(Addr or VolumeData or FreqData or Mode)
   begin
		if (Addr == 0)
			DataRd = {14'h0000, Mode};
		else if (Addr == 1)
			DataRd = {8'h00, VolumeData};
		else if (Addr == 2)
			DataRd = FreqData;
		else 
			DataRd = 16'hxxxx;
	 end
	//End debugging stuff

	// Audio Data - Data Shift Register (For some reason, it's left justified, 32 bit frames, with channel data in the top 16 bits)
	always @(posedge Clk)
	   begin
		AsyncEdgeDetect <= AsyncSync;
		AbitClkEdgeDetect <= AbitClkSync;
		// Audio Data - Frame sync
		if ({AsyncEdgeDetect, AsyncSync} == 2'b01)
			begin
			BitCount <= 0;
			RightAudioData <= RightInputReg;
			end
		else if ({AsyncEdgeDetect, AsyncSync} == 2'b10)
		   begin
			BitCount <= 0;
			LeftAudioData <= LeftInputReg;
			end
		else if (~AbitClkEdgeDetect&AbitClkSync)
		   begin
			if (BitCount < 13)
			   begin
				if (AsyncSync)
					RightInputReg <= {RightInputReg[10:0], AsdoSync};
				else
					LeftInputReg <= {LeftInputReg[10:0], AsdoSync};
				BitCount <= BitCount + 1;
				end
			end
	   end

	// PWM
	always @(posedge Clk)
	   begin
		if (Reset)
		   begin
			DivCount <= 0;
			WaveOut <= 0;
			MixedAudioData = 0;
			AmpAudioData = 0;
			Out <= 0;
			TimeoutCount <= 0;
		   end
		else
		   begin
			DivCount <= DivCount + 1;
			
			if (DivCount==0 && VolumeData!=0)
			   begin
				MixedPrev <= MixedAudioData;
				if (MixedPrev != MixedAudioData)
					TimeoutCount <= 0;
				else if (TimeoutCount != 12'hfff)
					TimeoutCount <= TimeoutCount + 1;
					
				WaveOut <= 1'b1;
				MixedAudioData = {LeftAudioData[11],  LeftAudioData[11], LeftAudioData[11], LeftAudioData[11], LeftAudioData[11], LeftAudioData[11:1]} +
					{RightAudioData[11],  RightAudioData[11], RightAudioData[11], RightAudioData[11], RightAudioData[11], RightAudioData[11:1]};
				Oldsign = MixedAudioData[11];
				AmpAudioData = MixedAudioData * VolumeData;
			   end
			else if (DivCount >= MixedCompare)
				WaveOut <= 1'b0; 
		   end
			
		if (Mode == 2'b01) 
			Out <= VolumeOut & FreqOut;
		else if (Mode == 2'b10)
		   begin 
			if (TimeoutCount != 12'hfff)
			   Out <= WaveOut;
			else
				Out <= 1'b0;
			end
		else
			Out <= 1'b0;
	end	
   
	always @(Oldsign or AmpAudioData)
	   begin
 		if ({Oldsign, AmpAudioData[15]} == 2'b01)
			MixedCompare = 12'hfff;
		else if ({Oldsign, AmpAudioData[15]} == 2'b10)
			MixedCompare = 12'h000;
		else
		   MixedCompare = AmpAudioData[15:4] + 12'h800;
		end
		
	always @(posedge Clk)
		begin
		if (Reset)
			begin
			VolumeAcc <= 0;
			VolumeOut <= 0;
			end
		else
			begin
			VolumeAcc <= VolumeAcc + 1;
			if (VolumeAcc == VolumeData)
				VolumeOut <= 1'b0;
			else if (VolumeAcc == 8'h00)
				VolumeOut <= 1'b1;		
			end		
		end
	
// Tone PWM - Frequency
	always @(posedge Clk)
	begin
		if (Reset)
		begin
			FreqAcc <= 0;
			FreqOut <= 0;
		end
		else
		begin
			if (FreqData == FreqAcc[20:5])
			begin
				FreqOut <= ~FreqOut;
				FreqAcc <= 0;
			end
			else
				FreqAcc <= FreqAcc + 1;
		end
	end
// End ToneGenerator

// Output is the OR of Tone and Wave outputs so that either can be used when the other isn't active.
endmodule

