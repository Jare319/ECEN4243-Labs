This is the baseline files for the RISC-V RV32i simulator.

# UPDATED NOTES:

Within `sim.c`, we should probably implement all the instructions for the `r`, `b`, and `i` instructions first, as those already have the stucture to decode the incoming bit string.
The easiest, but also most tedious part will be the actual implementation of each function in the `isa.h`, which should probably be done last.

## TODO:
- [ ] implement the calls to each function in `sim.c`
- [ ] implement the bit sting decoder for the `s`, `j`, and `u` processes in `sim.c`
- [ ] implement and test each operation in `isa.h`

