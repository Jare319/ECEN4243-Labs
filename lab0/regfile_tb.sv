module stimulus ();

     logic clock, wenable;
     logic [4:0] raddr1, raddr2, waddr;
     logic [31:0] wdata, rdata1, rdata2;
   
   // Instantiate DUT
   regfile r (clock, wenable, raddr1, raddr2, waddr, wdata, rdata1, rdata2);

   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	clock = 1'b1;
	forever #5 clock = ~clock;
     end

   initial
     begin
	// Gives output file name
	handle3 = $fopen("regfile_test.out");
	// Tells when to finish simulation
	#500 $finish;		
     end 
   
   initial 
     begin      
	#0 wenable = 0;
     #0 waddr = 'b00001;
     #0 wdata = 'h02A03C30; // Tests the write-enable
     #20 wenable = 1;
     #0 wdata = 'hFFFFFFFF; // Tests writing in general
     #30 waddr = 'b00000;
     #0 wdata = 'hFFFFFFFF;
     #0 raddr1 = 'b00001; // tests writing to reg 0 and reading from an address
     #40 waddr = 'b00010;
     #0 wdata = 'hFFFFFFFF;
     #50 wdata = 'h0000000;
     #0 raddr2 = 'b00010; // Tests reading from an address as it is being re-writen

     end

endmodule // FSM_tb

