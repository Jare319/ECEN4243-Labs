// 32-bit register with 2 5-bit read address ports and a 5-bit write address port
// Port names will be adjusted for easier reading, see reference below. 
// Ports are prefaced with a 'r' or 'w' for 'read' or 'write' respectively, followed by function.
// wenable - write enable
// raddr1/2 - read address 1/2
// waddr - write address
// wdata - write data
// rdata1/2 - read data 1/2

module regfile (input logic clock, 
		input logic wenable, 
		input logic [4:0] raddr1, raddr2, waddr, 
		input logic [31:0] wdata, 
		output logic [31:0] rdata1, rdata2);
   
   logic [31:0] 		    rf[31:0];
   
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0

   always @(posedge clock) begin
      if (wenable && waddr != 'b00000) // Writes to the write address as long as the enable is high, and write address isn't register 0
         rf[waddr] = wdata;
      rdata1 = rf[raddr1]; // Output ports show value of register with address of 'read addresses' 1 and 2
      rdata2 = rf[raddr2];
   end
         
endmodule // regfile
