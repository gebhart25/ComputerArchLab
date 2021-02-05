module stimulus();
	logic clk;
	logic we3;
	logic [4:0] ra1, ra2, wa3;
	logic [31:0] rd1, rd2, wd3;
	logic Out;
	
	integer handle3;
	integer desc3;

	// Instantiate DUT
	regfile dut(.rd1(rd1),
			.clk(clk),
			.we3(we3),
			.ra1(ra1),
			.ra2(ra2),
			.wa3(wa3),
			.wd3(wd3),
			.rd2(rd2));

// Setup the clock to toggle every 1 time units 
	initial
		begin
			clk = 1'b1;
			forever #5 clk = ~clk;
		end
	initial
		begin
			//gives outpit file name
			handle3 = $fopen("regfileTEST.out");
			// Tells when to finish simulation
			#500 $finish;
		end
	always
		begin
			desc3 = handle3;
			#5 $fdisplay(desc3, "%b %b", we3,clk);
		end
	initial
		begin
			//beginning with all values set to 0
			#0 ra1 = 5'h0;
			#0 ra2 = 5'h0;
			#0 we3 = 1'b0;
			#0 wa3 = 5'h0;
			#0 wd3 = 32'h0;
			
			//on first positive edge
			#9 wd3 = 32'hFFFF_FFFF;
			wa3 = 5'h1;
			ra1 = 5'h1;
			we3 = 1'b1;
			
			//test if wd write when bit is 0
			#10 we3 = 1'b0;
			wd3 = 32'h2525_2525;
			
			//writing the array to a different register
			#10 wa3 = 5'h2;
			ra2 = 5'h2;
			we3 = 1'b1;
			
			//test writing to register 0 (should fail)
			#10 wa3 = 5'h0;
			ra1 = 5'h0;
			
			//read updates on positive clk edge
			#10 wa3 = 5'h2;
			wd3 = 32'h1234_5678;
		end
endmodule //regfile_tb

