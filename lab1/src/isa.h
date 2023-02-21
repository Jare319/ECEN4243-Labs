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

// Moved these here to allow for compilation and testing. Eventually, these three methods should be moved back to sim.c and the conflicts should be fixed.
char *byte_to_binary(int x) {

  static char b[9];
  b[0] = '\0';

  int z;
  for (z = 128; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

char *byte_to_binary32(int x) {

  static char b[33];
  b[0] = '\0';

  unsigned int z;
  for (z = 2147483648; z > 0; z >>= 1) {
    strcat(b, ((x & z) == z) ? "1" : "0");
  }

  return b;
}

int bchar_to_int(char* rsa) {

  int i = 0;
  int result = 0;
  int t = 0;
  while(rsa[i] != '\0')i++;
  while(i>0)
    {
      --i;
      // printf("%d\n", (rsa[i]-'0')<<t);
      result += (rsa[i] - '0')<<t;
      t++;
    }
  return result;
}

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
  return 0;
}

int ORI (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm,11);
  return 0;
}

int ANDI (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] & SIGNEXT(Imm,11);
  return 0;
}

int JALR (int Rd, int Rs1, int Imm) {
  NEXT_STATE.PC = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 11);
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  return 0;
}

int LB (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char byte[9]; byte[8] = '\0';
  for (int i = 0; i < 8; i++) {
    byte[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = SIGNEXT(bchar_to_int(byte), 7);
  return 0;
}

int LH (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char half[17]; half[16] = '\0';
  for (int i = 0; i < 16; i++) {
    half[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = SIGNEXT(bchar_to_int(half), 15);
  return 0;
}

int LW (int Rd, int Rs1, int Imm) {
  NEXT_STATE.REGS[Rd] = mem_read_32(Rs1 + SIGNEXT(Imm, 11));
  return 0;
}

int LBU (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char byte[9]; byte[8] = '\0';
  for (int i = 0; i < 8; i++) {
    byte[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = (unsigned)bchar_to_int(byte);
  return 0;
}

int LHU (int Rd, int Rs1, int Imm) {
  char *word = byte_to_binary32(mem_read_32(Rs1 + SIGNEXT(Imm, 11)));
  char half[17]; half[16] = '\0';
  for (int i = 0; i < 16; i++) {
    half[i] = word[i];
  }
  NEXT_STATE.REGS[Rd] = (unsigned)bchar_to_int(half);
  return 0;
}

// Zimm & Funct7 derived from Imm[4:0] and Imm[31:25] respectively, no need to pass as input
int SLLI (int Rd, int Rs1, int Imm) {
  char *ImmBinary = byte_to_binary32(Imm);
  char ZimmBinary[6]; ZimmBinary[5] = '\0';
  for (int i = 0; i < 5; i++) {
    ZimmBinary[i] = ImmBinary[31-24+i];
  }
  int Zimm = (unsigned)bchar_to_int(ZimmBinary);
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] << Zimm;
  return 0;
}

int SRLI (int Rd, int Rs1, int Imm) {
  char *ImmBinary = byte_to_binary32(Imm);
  char ZimmBinary[6]; ZimmBinary[5] = '\0';
  for (int i = 0; i < 5; i++) {
    ZimmBinary[i] = ImmBinary[31-24+i];
  }
  int Zimm = (unsigned)bchar_to_int(ZimmBinary);
  NEXT_STATE.REGS[Rd] = (unsigned)CURRENT_STATE.REGS[Rs1] >> Zimm; // This is a quick and dirty attempt to get this to work, however this may not end up doing the arithmetic shift correctly.
  return 0;
}

int SRAI (int Rd, int Rs1, int Imm) {
  char *ImmBinary = byte_to_binary32(Imm);
  char ZimmBinary[6]; ZimmBinary[5] = '\0';
  for (int i = 0; i < 5; i++) {
    ZimmBinary[i] = ImmBinary[31-24+i];
  }
  int Zimm = (unsigned)bchar_to_int(ZimmBinary);
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] >> Zimm;
  return 0;
}


// U Instruction

int AUIPC (int Rd, int Upimm) {
  NEXT_STATE.REGS[Rd] = Upimm + CURRENT_STATE.PC;
  return 0;
}

int LUI (int Rd, int Upimm) {
  NEXT_STATE.REGS[Rd] = Upimm;
  printf(" Hex: %x\n Int: %i\n\n",Upimm,Upimm);
  return 0;
}


// S Instruction

int SB (int Rs1, int Rs2, int Imm, char *mem) {
  char byte[9]; byte[8] = '\0';
  for (int i = 0; i < 8; i++) {
    byte[i] = mem[31-7+i];
  }
  NEXT_STATE.REGS[Rs2] = bchar_to_int(byte);
  return 0;
}

int SH (int Rs1, int Rs2, int Imm, char *mem) {
  char half[17]; half[16] = '\0';
  for (int i = 0; i < 16; i++) {
    half[i] = mem[31-15+i];
  }
  NEXT_STATE.REGS[Rs2] = bchar_to_int(half);
  return 0;
}

int SW (int Rs1, int Rs2, int Imm, char *mem) {
  char word[33]; word[32] = '\0';
  for (int i = 0; i < 32; i++) {
    word[i] = mem[i];
  }
  NEXT_STATE.REGS[Rs2] = bchar_to_int(word);
  return 0;
}


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
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] ^ CURRENT_STATE.REGS[Rs2];
  return 0;
}

/* These 2 shift-right statements should be changed to enforce the logical/arithmetic distinction, as it is undertermined (documentation states
  determined somewhat arbitrarily by compiler) otherwise */
int SRL (int Rd, int Rs1, int Rs2) {
  char *binary = byte_to_binary32(CURRENT_STATE.REGS[Rs1]);
  for (int j = 0; j < Rs2; j++) {
    for (int i = 31; i > 0; i--) {
      binary[i] = binary[i-1];
    }
    binary[0] = 0;
  }
  NEXT_STATE.REGS[Rd] = bchar_to_int(binary);
  return 0;
}

int SRA (int Rd, int Rs1, int Rs2) {
  char *binary = byte_to_binary32(CURRENT_STATE.REGS[Rs1]);
  for (int j = 0; j < Rs2; j++) {
    for (int i = 31; i > 0; i--) {
      binary[i] = binary[i-1];
    }
  }
  NEXT_STATE.REGS[Rd] = bchar_to_int(binary);
  return 0;
}

int OR (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] | CURRENT_STATE.REGS[Rs2];
  return 0;
}

int AND (int Rd, int Rs1, int Rs2) {
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.REGS[Rs1] & CURRENT_STATE.REGS[Rs2];
  return 0;
}


// B instructions

int BNE (int Rs1, int Rs2, int BTA) {
  if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}

int BEQ (int Rs1, int Rs2, int BTA) {
  if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}

int BLT (int Rs1, int Rs2, int BTA) {
  if (CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}

int BGE (int Rs1, int Rs2, int BTA) {
  if (CURRENT_STATE.REGS[Rs1] >= CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}

int BLTU (int Rs1, int Rs2, int BTA) {
  if ((unsigned)CURRENT_STATE.REGS[Rs1] < (unsigned)CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}

int BGEU (int Rs1, int Rs2, int BTA) {
  if ((unsigned)CURRENT_STATE.REGS[Rs1] >= (unsigned)CURRENT_STATE.REGS[Rs2]) {
    printf(" BTA: %x\n", BTA);
    NEXT_STATE.PC = BTA;
  }
  return 0;
}


// J instruction
int JAL (int Rd, int JTA) {
  NEXT_STATE.PC = JTA;
  NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
  return 0;
}

int ECALL (char* i_){return 0;}

#endif
