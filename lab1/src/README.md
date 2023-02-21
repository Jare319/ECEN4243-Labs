This is the baseline files for the RISC-V RV32i simulator.

# UPDATED NOTES:

Within `sim.c`, we should probably implement all the instructions for the `r`, `b`, and `i` instructions first, as those already have the stucture to decode the incoming bit string.
The easiest, but also most tedious part will be the actual implementation of each function in the `isa.h`, which should probably be done last.

## TODO:
- [X] implement the calls to each function in `sim.c`
- [X] implement the bit string decoder for the `s`, `j`, and `u` processes in `sim.c`
- [ ] implement and test each operation in `isa.h`

## Instruction List
- [X] ADD 
- [X] ADDI 
- [X] AND
- [X] ANDI
- [X] AUIPC
- [X] BEQ
- [X] BGE
- [X] BGEU
- [X] BLT
- [X] BLTU
- [X] BNE
- [X] JAL
- [X] JALR
- [X] LB
- [X] LBU
- [X] LH
- [X] LHU
- [X] LUI
- [X] LW
- [X] OR
- [X] ORI
- [X] SB
- [X] SH
- [X] SLL
- [X] SLLI
- [X] SLT
- [X] SLTI
- [X] SLTIU
- [X] SLTU
- [X] SRA
- [X] SRAI
- [X] SRL
- [X] SRLI
- [X] SUB
- [X] SW
- [X] XOR
- [X] XORI
- [X] ECALL

