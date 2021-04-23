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

   always @(CURRENT_STATE or X)
     begin
 	case(CURRENT_STATE)
       Idle:	
         if (Strobe == 1'b0)
            begin
               LdCtr = 1'b1;
               ReadyEn = 1'b0;
               Rdy = 1'b0;
               W = 1'b0;
               MStrobe = 1'b0;
               MRW = 1'b0;
               WSel = 1'b0;
               RSel = 1'b0;
               NEXT_STATE <=  Idle;
            end 
         else begin
         //set outputs
         //set next state
            end
      Read:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      ReadMiss:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      ReadMem:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      ReadData:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      Write:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      WriteHit:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      WriteMiss:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      WriteMem:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
      WriteData:	
         if (/*something*/)
            begin
         //set outputs
         //set next state
            end 
         else begin
         //set outputs
         //set next state
            end
	  default: 
	    begin
	       NEXT_STATE <=  S0;
	       Z = 1'b0;	     
	    end
	  
	endcase // case (CURRENT_STATE)	
     end // always @ (CURRENT_STATE or X)   

endmodule // fsm



















endmodule /* Control */


