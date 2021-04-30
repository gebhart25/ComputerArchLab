module CacheControl(input Strobe,
                    input DRW,//R/W
                    input M,
                    input V,
                    input clk,
                    input reset,
                    output DReady,
                    output W,
                    output MStrobe,
                    output MRW,
                    output RSel,
                    output WSel);

   logic [7:0] WSCLoadVal;   
   logic       CtrSig;
   logic       ReadyEn;
   logic       LdCtr;
   logic       Ready;   
   logic [7:0]  OutputLogic;
  
   /*output logic DReady;
   output logic W;
   output logic MStrobe;
   output logic MRW;
   output logic RSel;
   output logic WSelc;*/

   assign DReady = (ReadyEn & M && V && ~DRW) || Ready;
   assign {LdCtr, ReadyEn, Ready, W, MStrobe, MRW, RSel, WSel} = OutputLogic;

   parameter [3:0] Idle      = 4'b0000,
                   Read      = 4'b0001,
                   ReadMiss  = 4'b0010,
                   ReadMem   = 4'b0011,
                   ReadData  = 4'b0100,
                   Write     = 4'b0101,
                   WriteHit  = 4'b0110,
                   WriteMiss = 4'b0111,
                   WriteMem  = 4'b1000,
                   WriteData = 4'b1001; 

   logic [3:0] CURRENT_STATE;
   logic [3:0] NEXT_STATE;

   // wait state = 100 cycles
   assign WSCLoadVal = 8'h4;   
   wait_state WaitStateCtr (LdCtr, WSCLoadVal, CtrSig, clk);

   // Insert FSM Here

   always @(posedge clk)
     begin
	if (reset == 1'b1)	
	  CURRENT_STATE <=  Idle;
	else
	  CURRENT_STATE <=  NEXT_STATE;
     end

   always @(CURRENT_STATE or Strobe or DRW or M or V or CtrSig)
     begin
 	case(CURRENT_STATE)
      Idle:	
         if (Strobe == 1'b0)
            begin
               ////LdCtr = 1'b1;
              // //ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*assign DReady = 1'b0;
               assign W = 1'b0;
               assign MStrobe = 1'b0;
               assign MRW = 1'b0;
               assign WSel = 1'b0;
               assign RSel = 1'b0;*/
               NEXT_STATE <=  Idle;
            end 
         else if (Strobe == 1'b1 && DRW == 1)
            begin
               ////LdCtr = 1'b1;
              // //ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  Write;
            end 
         else //((Strobe == 1'b1 && DRW == 0))
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  Read;
            end
      Read:	
         if (M == 1'b0 && V == 1'b0)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b1;
               OutputLogic = 8'b11000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  ReadMiss;
            end 
         else if (M == 1'b0 && V == 1'b1)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b1;
               OutputLogic = 8'b11000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  ReadMiss;
            end 
         else if (M == 1'b1 && V == 1'b0)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b1;
               OutputLogic = 8'b11000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  ReadMiss;
            end 
         else//(M == 1'b1 && V == 1'b1)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b1;
               OutputLogic = 8'b11000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  Idle;
            end
      ReadMiss:	
         begin
            ////LdCtr = 1'b1;
            ////ReadyEn = 1'b0;
            OutputLogic = 8'b10001000;
            /*DReady = 1'b0;
            W = 1'b0;
            MStrobe = 1'b1;
            MRW = 1'b0;
            WSel = 1'b0;
            RSel = 1'b0;*/
            NEXT_STATE <=  ReadMem;
         end 
      ReadMem:	
         if (CtrSig == 1'b1)
            begin
               ////LdCtr = 1'b0;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b00000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  ReadData;
            end 
         else //(CtrSig == 1'b0)
            begin
               ////LdCtr = 1'b0;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b00000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  ReadMem;
            end
      ReadData:	//RDY =1
         begin
            ////LdCtr = 1'b0;
            ////ReadyEn = 1'b0;
            OutputLogic = 8'b00110110;
            /*DReady = 1'b1;
            W = 1'b1;
            MStrobe = 1'b0;
            MRW = 1'b1;
            WSel = 1'b1;
            RSel = 1'b0;*/
            NEXT_STATE <=  ReadData;
         end
      Write:	
         if (M == 1'b0 && V == 1'b0)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
              /* DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteMiss;
            end 
         else if (M == 1'b0 && V == 1'b1)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteMiss;
            end 
         else if (M == 1'b0 && V == 1'b1)
            begin
               ////LdCtr = 1'b1;
               ////ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteMiss;
            end 
         else//(M == 1'b1 && V == 1'b1)
            begin
               //LdCtr = 1'b1;
               //ReadyEn = 1'b0;
               OutputLogic = 8'b10000000;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteHit;
            end
         
      WriteMiss:	
         begin
            //LdCtr = 1'b1;
            //ReadyEn = 1'b0;
            OutputLogic = 8'b10001100;
            /*DReady = 1'b0;
            W = 1'b0;
            MStrobe = 1'b1;
            MRW = 1'b1;
            WSel = 1'b0;
            RSel = 1'b0;*/
            NEXT_STATE <=  WriteMem;
         end
         
      WriteHit:	
         begin
            //LdCtr = 1'b1;
            //ReadyEn = 1'b0;
            OutputLogic = 8'b10001100;
            /*DReady = 1'b0;
            W = 1'b0;
            MStrobe = 1'b1;
            MRW = 1'b1;
            WSel = 1'b0;
            RSel = 1'b0;*/
            NEXT_STATE <=  WriteMem;
         end

      WriteMem:	
         if (CtrSig == 1'b1)
            begin
               //LdCtr = 1'b0;
               //ReadyEn = 1'b0;
               OutputLogic = 8'b00000100;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b1;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteData;
            end 
         else //(CtrSig == 1'b1)
            begin
               //LdCtr = 1'b0;
               //ReadyEn = 1'b0;
               OutputLogic = 8'b00000100;
               /*DReady = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b1;
               WSel = 1'b0;
               RSel = 1'b0;*/
               NEXT_STATE <=  WriteMem;
            end
      WriteData:	//RDY = 1
         begin
            //LdCtr = 1'b0;
            //ReadyEn = 1'b0;
            OutputLogic = 8'b00110101;
            /*DReady = 1'b1;
            W = 1'b1;
            MStrobe = 1'b0;
            MRW = 1'b1;
            WSel = 1'b0;
            RSel = 1'b1;*/
            NEXT_STATE <=  Idle;
         end 
	  default: 
	    begin
	       NEXT_STATE <=  Idle;
	    end
	  
	endcase // case (CURRENT_STATE)	
     end // always @ (CURRENT_STATE or X)   

endmodule // fsm



















//endmodule /* Control */


