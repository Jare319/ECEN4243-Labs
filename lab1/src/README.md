This is the baseline files for the RISC-V RV32i simulator.

# UPDATED NOTES:

Within `sim.c`, we should probably implement all the instructions for the `r`, `b`, and `i` instructions first, as those already have the stucture to decode the incoming bit string.
The easiest, but also most tedious part will be the actual implementation of each function in the `isa.h`, which should probably be done last.

## TODO:
- [ ] implement the calls to each function in `sim.c`
- [X] implement the bit sting decoder for the `s`, `j`, and `u` processes in `sim.c`
- [ ] implement and test each operation in `isa.h`

## Instruction List
- [X] ADD 
- [X] ADDI 
- [X] AND
- [X] ANDI
- [ ] AUIPC
- [X] BEQ
- [X] BGE
- [X] BGEU
- [X] BLT
- [X] BLTU
- [X] BNE
- [ ] JAL
- [X] JALR
- [X] LB
- [X] LBU
- [X] LH
- [X] LHU
- [ ] LUI
- [X] LW
- [X] OR
- [X] ORI
- [ ] SB
- [ ] SH
- [X] SLL
- [ ] SLLI
- [X] SLT
- [X] SLTI
- [X] SLTIU
- [X] SLTU
- [X] SRA
- [ ] SRAI
- [X] SRL
- [ ] SRLI
- [X] SUB
- [ ] SW
- [X] XOR
- [X] XORI
- [X] ECALL