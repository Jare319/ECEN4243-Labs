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
	#0 wenable = 0;
     #0 waddr = 'b00001;
     #0 wdata = 'h02A03C30;
     #0 raddr1 = 'b00001; // Tests the write-enable
     #15 wenable = 1;
     #5 wdata = 'hFFFFFFFF;
     #0 raddr1 = 'b00001; // Tests writing in general
     #20 waddr = 'b00000;
     #0 wdata = 'hFFFFFFFF;
     #0 raddr1 = 'b00000; // tests writing to reg 0 and reading from an address
     #20 waddr = 'b00010;
     #0 wdata = 'hFFFFFFFF;
     #0 raddr2 = 'b00010;
     #20 wdata = 'h0000000;
     #0 raddr2 = 'b00010; // Tests reading from an address as it is being re-writen

     end

endmodule // FSM_tb

