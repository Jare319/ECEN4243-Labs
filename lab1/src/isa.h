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
  cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm,11);
  NEXT_STATE.REGS[Rd] = cur;
  return 0;
}

int SLTI (int Rd, int Rs1, int Imm) {
  if (CURRENT_STATE.REGS[Rs1] < SIGNEXT(Imm,11)) {
    NEXT_STATE.REGS[Rd] = 1;
  } else {
    NEXT_STATE.REGS[Rd] = 0;
  }
  return 0;
}

int SLTIU (int Rd, int Rs1, int Imm) {
  if (CURRENT_STATE.REGS[Rs1] < (unsigned)SIGNEXT(Imm,11)) {
    NEXT_STATE.REGS[Rd] = 1;
  } else {
    NEXT_STATE.REGS[Rd] = 0;
  }
  return 0;
}

int XORI (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] ^ SIGNEXT(Imm,11);
}

int ORI (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm,11);
}

int ANDI (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] & SIGNEXT(Imm,11);
}

int JALR (int Rd, int Rs1, int Imm) {
  NEXT_STATE.PC = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 11);
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
}

int LB (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char *byte;
  for (int i = 0; i < 8; i++) {
    byte[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = SIGNEXT(bchar_to_int(byte), 7);
}

int LH (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char *half;
  for (int i = 0; i < 8; i++) {
    half[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = SIGNEXT(bchar_to_int(half), 15);
}

int LW (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = mem_read_32(Rs1 + SIGNEXT(Imm, 11));
}

int LBU (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm,11)));
  char *byte;
  for (int i = 0; i < 8; i++) {
    byte[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = (unsigned)bchar_to_int(byte);
}

int LHU (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char *half;
  for (int i = 0; i < 8; i++) {
    half[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = (unsigned)bchar_to_int(half);
}

int SLLI (int Rd, int Rs1, int Imm); // Zimm & Funct7 derived from Imm[4:0] and Imm[31:25] respectively, no need to pass as input

int SRLI (int Rd, int Rs1, int Imm);

int SRAI (int Rd, int Rs1, int Imm);


// U Instruction

int AUIPC (char* i_);

int LUI (char* i_);


// S Instruction

int SB (char* i_);

int SH (char* i_);

int SW (char* i_);


// R instruction

int ADD (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
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

/* These 2 shift-right statements should be changed to enforce the logical/arithmetic distinction, as it is undertermined otherwise*/
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
