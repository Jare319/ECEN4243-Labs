/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

//
// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
//
#define SIGNEXT(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

// I Instructions

int ADDI (int Rd, int Rs1, int Imm) {
  int cur = 0;
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,12);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTI (int Rd, int Rs1, int Imm) {
  if (CURRENT_STATE.REGS[Rs1] < SIGNEXT(Imm,12)) {
    NEXT_STATE.REGS[Rd] = 1;
  } else {
    NEXT_STATE.REGS[Rd] = 0;
  }
  return 0;
}

int SLTIU (char* i_);

int XORI (char* i_);

int ORI (char* i_);

int ANDI (char* i_);

int JALR (char* i_);

int LB (char* i_); //Imm(Rs1)

int LH (char* i_);

int LW (char* i_);

int LBU (char* i_);

int LHU (char* i_);

int SLLI (char* i_);//Zimm

int SRLI (char* i_);//Zimm & Funct7???

int SRAI (char* i_);


// U Instruction

int AUIPC (char* i_);

int LUI (char* i_);


// S Instruction

int SB (char* i_);

int SH (char* i_);

int SW (char* i_);


// R instruction

int ADD (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];;
  return 0;
}

int SUB (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
  return 0;
}

int SLL (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1]<<CURRENT_STATE.REGS[Rs2];
  return 0;
}

int SLT (int Rd, int Rs1, int Rs2) {
  if (CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2]) {
    NEXT_STATE.REGS[Rd] = 1;
  } else {
    NEXT_STATE.REGS[Rd] = 0;
  }
  return 0;
}

int SLTU (int Rd, int Rs1, int Rs2) {
  if ((unsigned)CURRENT_STATE.REGS[Rs1] < (unsigned)CURRENT_STATE.REGS[Rs2]) {
    NEXT_STATE.REGS[Rd] = 1;
  } else {
    NEXT_STATE.REGS[Rd] = 0;
  }
  return 0;
}

int XOR (int Rd, int Rs1, int Rs2) {
  char rs1[6]=byte_to_binary(Rs1); rs1[5] = '\0';
  char rs2[6]=byte_to_binary(Rs2); rs2[5] = '\0';
  char rd[6]; rd[5] = '\0';
  for (int i = 0; i < 4; i++) {
    if(rs1[i]!=rs2[i]){
      rd[i]=1;
    }
    else{
      rd[i]=0;
    }
  }
  NEXT_STATE.REGS[Rd] = bchar_to_int(rd);
  return 0;
}

int SRL (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] >> CURRENT_STATE.REGS[Rs2];
  return 0;
}

int SRA (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] >> CURRENT_STATE.REGS[Rs2];
  return 0;
}

int OR (int Rd, int Rs1, int Rs2) {
  char rs1[6]=byte_to_binary(Rs1); rs1[5] = '\0';
  char rs2[6]=byte_to_binary(Rs2); rs2[5] = '\0';
  char rd[6]; rd[5] = '\0';
  for (int i = 0; i < 4; i++) {
    if(rs1[i] + rs2[i] > 0){
      rd[i]=1;
    }
    else{
      rd[i]=0;
    }
  }
  NEXT_STATE.REGS[Rd] = bchar_to_int(rd);
  return 0;
}

int AND (int Rd, int Rs1, int Rs2) {
  char rs1[6]=byte_to_binary(Rs1); rs1[5] = '\0';
  char rs2[6]=byte_to_binary(Rs2); rs2[5] = '\0';
  char rd[6]; rd[5] = '\0';
  for (int i = 0; i < 4; i++) {
    if(rs1[i] == rs2[i]){
      rd[i]=1;
    }
    else{
      rd[i]=0;
    }
  }
  NEXT_STATE.REGS[Rd] = bchar_to_int(rd);
  return 0;
}


// B instructions

int BNE (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2]) {
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  }
  return 0;
}

int BEQ (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2]) {
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  }
  return 0;
}

int BLT (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2]) {
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  }
  return 0;
}

int BGE (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if (CURRENT_STATE.REGS[Rs1] >= CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

int BLTU (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned)CURRENT_STATE.REGS[Rs1] < (unsigned)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}

int BGEU (int Rs1, int Rs2, int Imm) {
  int cur = 0;
  Imm = Imm << 1;
  if ((unsigned)CURRENT_STATE.REGS[Rs1] >= (unsigned)CURRENT_STATE.REGS[Rs2])
    NEXT_STATE.PC = (CURRENT_STATE.PC + 4) + (SIGNEXT(Imm,13));
  return 0;
}


// J instruction
int JAL (char* i_);

int ECALL (char* i_){return 0;}

#endif
