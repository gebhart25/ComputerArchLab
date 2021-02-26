/***************************************************************/
/*                                                             */
/*   ARMv4-32 Instruction Level Simulator                      */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_
#define N_CUR ( (CURRENT_STATE.CPSR>>31) & 0x00000001 )
#define Z_CUR ( (CURRENT_STATE.CPSR>>30) & 0x00000001 )
#define C_CUR ( (CURRENT_STATE.CPSR>>29) & 0x00000001 )
#define V_CUR ( (CURRENT_STATE.CPSR>>28) & 0x00000001 )
#define N_NXT ( (NEXT_STATE.CPSR>>31) & 0x00000001 )
#define Z_NXT ( (NEXT_STATE.CPSR>>30) & 0x00000001 )
#define C_NXT ( (NEXT_STATE.CPSR>>29) & 0x00000001 )
#define V_NXT ( (NEXT_STATE.CPSR>>28) & 0x00000001 )

#define N_N 0x80000000
#define Z_N 0x40000000
#define C_N 0x20000000
#define V_N 0x10000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int ADD (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >>> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int AND (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;	//cur: output
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int EOR (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;	//cur: output
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] ^ (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int ORR (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;	//cur: output
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] | 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] | 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] | (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int SUB (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int BIC (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;	//cur: output
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      !((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  !(CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      !((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & !(Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int ADC (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5)+ (CURRENT_STATE.CPSR & C_N >> 29);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) + (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate))) + (CURRENT_STATE.CPSR & C_N >> 29);
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int SBC (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5) - (CURRENT_STATE.CPSR & C_N >> 29);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5))) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs]))) - (CURRENT_STATE.CPSR & C_N >> 29);
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate))) - (CURRENT_STATE.CPSR & C_N >> 29);
  }
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int LSR (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
 
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    
    
    cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5);

  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int LSL (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
 
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    
    
    cur = CURRENT_STATE.REGS[Rn] << 
	  (CURRENT_STATE.REGS[Rm] << shamt5);


  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int ASR (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    
    cur = CURRENT_STATE.REGS[Rn] >>> 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int CMN (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] + (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  //NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int CMP (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] - 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] - 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] - (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  //NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int TST (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] &
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] & 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] & 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] & (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int TEQ (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  if(I == 0) {
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << shamt5);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> shamt5);
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] ^ 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] ^ 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      
  }
  if (I == 1) {
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = CURRENT_STATE.REGS[Rn] ^ (Imm>>2*rotate|(Imm<<(32-2*rotate)));
  }
  
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int MVN (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;	//cur: output
  
  
	cur = !CURRENT_STATE.REGS[Rn]
    
  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int MOV (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
  
    int rotate = Operand2 >> 8;
    int Imm = Operand2 & 0x000000FF;
    cur = (Imm>>2*rotate|(Imm<<(32-2*rotate)));

  
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}
//????
int ROR (int Rd, int Rn, int Operand2, int I, int S, int CC) {

  int cur = 0;
    int sh = (Operand2 & 0x00000060) >> 5;
    int shamt5 = (Operand2 & 0x00000F80) >> 7;
    int bit4 = (Operand2 & 0x00000010) >> 4;
    int Rm = Operand2 & 0x0000000F;
    int Rs = (Operand2 & 0x00000F00) >> 8;
    if (bit4 == 0) 
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn]  >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5) | (CURRENT_STATE.REGS[Rn] << (32 - (CURRENT_STATE.REGS[Rm] << shamt5)));
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5) | (CURRENT_STATE.REGS[Rn] << (32 - (CURRENT_STATE.REGS[Rm] << shamt5)));
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5) | (CURRENT_STATE.REGS[Rn] << (32 - (CURRENT_STATE.REGS[Rm] << shamt5)));
    	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] >> 
	  (CURRENT_STATE.REGS[Rm] << shamt5) | (CURRENT_STATE.REGS[Rn] << (32 - (CURRENT_STATE.REGS[Rm] << shamt5))); 
	      ((CURRENT_STATE.REGS[Rm] >> shamt5) |
               (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
	  break;
      }     
    else
      switch (sh) {
      case 0: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] << CURRENT_STATE.REGS[Rs]);
	  break;
      case 1: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 2: cur = CURRENT_STATE.REGS[Rn] + 
	  (CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]);
	  break;
      case 3: cur = CURRENT_STATE.REGS[Rn] + 
	      ((CURRENT_STATE.REGS[Rm] >> CURRENT_STATE.REGS[Rs]) |
               (CURRENT_STATE.REGS[Rm] << (32 - CURRENT_STATE.REGS[Rs])));
	  break;
      }      

  NEXT_STATE.REGS[Rd] = cur;
  if (S == 1) {
    if (cur < 0)
      NEXT_STATE.CPSR |= N_N;
    if (cur == 0)
      NEXT_STATE.CPSR |= Z_N;
  }	
  return 0;

}

int B (int imm24) {

    NEXT_STATE.PC = (CURRENT_STATE.PC + 8) + imm24 << 2;
  
  return 0;

}

int BL (int imm24) {

    NEXT_STATE.REGS[14] = (CURRENT_STATE.PC + 8)-4;
    NEXT_STATE.PC = (CURRENT_STATE.PC + 8) + imm24 << 2;
  
  return 0;
}

int LDR (int p, int w, int I, int Rd, int Rn, int Operand2){
  int sh = (Operand2 & 0x00000060) >> 5;
  int shamt5 = (Operand2 & 0x00000F80) >> 7;
  int bit4 = (Operand2 & 0x00000010) >> 4;
  int Rm = Operand2 & 0x0000000F;
  if(I=0)
  {
    if(p==1 && w==0)
    {
      NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
    }
    if(p==0 && w==0)
    {
        NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn]);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    if(p==0 && w==1)
    {
        //do nothing
        return 0;
    }
    if(p==1 && w==1)
    {
        NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    
  }
  if(I=1)
  {
    switch (sh) {
        case 0: op2 = (CURRENT_STATE.REGS[Rm] << shamt5);
      break;
        case 1: op2 = (CURRENT_STATE.REGS[Rm] >> shamt5);
      break;
        case 2: op2 = (CURRENT_STATE.REGS[Rm] >>> shamt5);
          break;
        case 3: op2 = ((CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
      break;
    } 
    if(p==1 && w==0)
    {
        NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
        return 0;
    }
    if(p==0 && w==0)
    {
      NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
      Rn = Rn + op2;
    } 
    if(p==0 && w==1)
    {
      //do nothing
      return 0;
    }
    if(p==1 && w==1)
    {
      NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn]);
      Rn = Rn + op2;
    }
    //NEXT_STATE.REGS[Rd] = mem_read_32(op2);
  }

  return 0;
 
}

int STR (int p, int w, int I, int Rd, int Rn, int Operand2){
  int sh = (Operand2 & 0x00000060) >> 5;
  int shamt5 = (Operand2 & 0x00000F80) >> 7;
  int bit4 = (Operand2 & 0x00000010) >> 4;
  int Rm = Operand2 & 0x0000000F;
  if(I=0)
  {
    if(p==1 && w==0)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(Rn + Operand2);
      mem_write_32(CURRENT_STATE.REGS[Rn]+Operand2,CURRENT_STATE.REGS[Rd],)
    }
    if(p==0 && w==0)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(Rn);
        mem_write_32(CURRENT_STATE.REGS[Rn],CURRENT_STATE.REGS[Rd],);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    if(p==0 && w==1)
    {
        //do nothing
        return 0;
    }
    if(p==1 && w==1)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
        mem_write_32(CURRENT_STATE.REGS[Rn] + Operand2, CURRENT_STATE.REGS[Rd]);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    
  }
  if(I=1)
  {
    switch (sh) {
        case 0: op2 = (CURRENT_STATE.REGS[Rm] << shamt5);
      break;
        case 1: op2 = (CURRENT_STATE.REGS[Rm] >> shamt5);
      break;
        case 2: op2 = (CURRENT_STATE.REGS[Rm] >>> shamt5);
          break;
        case 3: op2 = ((CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
      break;
    } 
    if(p==1 && w==0)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
        mem_write_32(CURRENT_STATE.REGS[Rn] + op2, CURRENT_STATE.REGS[Rd]);
        return 0;
    }
    if(p==0 && w==0)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
      mem_write_32(CURRENT_STATE.REGS[Rn] + op2, CURRENT_STATE.REGS[Rd]);
      Rn = Rn + op2;
    } 
    if(p==0 && w==1)
    {
      //do nothing
      return 0;
    }
    if(p==1 && w==1)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn]);
      mem_write_32(CURRENT_STATE.REGS[Rn], CURRENT_STATE.REGS[Rd]);
      Rn = Rn + op2;
    }
    //NEXT_STATE.REGS[Rd] = mem_read_32(op2);
  }

  return 0;
 
}

int LDRB (int p, int w, int I, int Rd, int Rn, int Operand2){
  int sh = (Operand2 & 0x00000060) >> 5;
  int shamt5 = (Operand2 & 0x00000F80) >> 7;
  int bit4 = (Operand2 & 0x00000010) >> 4;
  int Rm = Operand2 & 0x0000000F;
  if(I=0)
  {
    if(p==1 && w==0)
    {
      NEXT_STATE.REGS[Rd] = 0x000000FF & mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
    }
    if(p==0 && w==0)
    {
        NEXT_STATE.REGS[Rd] = 0x000000FF & mem_read_32(CURRENT_STATE.REGS[Rn]);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    if(p==0 && w==1)
    {
        //do nothing
        return 0;
    }
    if(p==1 && w==1)
    {
        NEXT_STATE.REGS[Rd] = 0x000000FF & mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    
  }
  if(I=1)
  {
    switch (sh) {
        case 0: op2 = (CURRENT_STATE.REGS[Rm] << shamt5);
      break;
        case 1: op2 = (CURRENT_STATE.REGS[Rm] >> shamt5);
      break;
        case 2: op2 = (CURRENT_STATE.REGS[Rm] >>> shamt5);
          break;
        case 3: op2 = ((CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
      break;
    } 
    if(p==1 && w==0)
    {
        NEXT_STATE.REGS[Rd] = 0x000000FF & mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
        return 0;
    }
    if(p==0 && w==0)
    {
      NEXT_STATE.REGS[Rd] = 0x000000FF & mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
      Rn = Rn + op2;
    } 
    if(p==0 && w==1)
    {
      //do nothing
      return 0;
    }
    if(p==1 && w==1)
    {
      NEXT_STATE.REGS[Rd] = 0x000000FF mem_read_32(CURRENT_STATE.REGS[Rn]);
      Rn = Rn + op2;
    }
    //NEXT_STATE.REGS[Rd] = mem_read_32(op2);
  }

  return 0;
 
}

int STRB (int p, int w, int I, int Rd, int Rn, int Operand2){
  int sh = (Operand2 & 0x00000060) >> 5;
  int shamt5 = (Operand2 & 0x00000F80) >> 7;
  int bit4 = (Operand2 & 0x00000010) >> 4;
  int Rm = Operand2 & 0x0000000F;
  if(I=0)
  {
    if(p==1 && w==0)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(Rn + Operand2);
      mem_write_32(CURRENT_STATE.REGS[Rn]+Operand2, 0x000000FF & CURRENT_STATE.REGS[Rd])
    }
    if(p==0 && w==0)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(Rn);
        mem_write_32(CURRENT_STATE.REGS[Rn], 0x000000FF & CURRENT_STATE.REGS[Rd]);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    if(p==0 && w==1)
    {
        //do nothing
        return 0;
    }
    if(p==1 && w==1)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + Operand2);
        mem_write_32(CURRENT_STATE.REGS[Rn] + Operand2,  0x000000FF & CURRENT_STATE.REGS[Rd]);
        NEXT_STATE.REGS[Rn] = CURRENT_STATE.REGS[Rn] + Operand2;
    }
    
  }
  if(I=1)
  {
    switch (sh) {
        case 0: op2 = (CURRENT_STATE.REGS[Rm] << shamt5);
      break;
        case 1: op2 = (CURRENT_STATE.REGS[Rm] >> shamt5);
      break;
        case 2: op2 = (CURRENT_STATE.REGS[Rm] >>> shamt5);
          break;
        case 3: op2 = ((CURRENT_STATE.REGS[Rm] >> shamt5) | (CURRENT_STATE.REGS[Rm] << (32 - shamt5)));
      break;
    } 
    if(p==1 && w==0)
    {
        //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
        mem_write_32(CURRENT_STATE.REGS[Rn] + op2, 0x000000FF & CURRENT_STATE.REGS[Rd]);
        return 0;
    }
    if(p==0 && w==0)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn] + op2);
      mem_write_32(CURRENT_STATE.REGS[Rn] + op2, 0x000000FF & CURRENT_STATE.REGS[Rd]);
      Rn = Rn + op2;
    } 
    if(p==0 && w==1)
    {
      //do nothing
      return 0;
    }
    if(p==1 && w==1)
    {
      //NEXT_STATE.REGS[Rd] = mem_read_32(CURRENT_STATE.REGS[Rn]);
      mem_write_32(CURRENT_STATE.REGS[Rn], 0x000000FF & CURRENT_STATE.REGS[Rd]);
      Rn = Rn + op2;
    }
    //NEXT_STATE.REGS[Rd] = mem_read_32(op2);
  }

  return 0;
 
}
//?int ROR (char* i_);
int SWI (char* i_){return 0;}




//extra credit
int MLA (char* i_);













#endif
