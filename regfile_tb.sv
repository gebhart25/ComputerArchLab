module stimulus();
	logic clk;
	logic we3;
	logic ra1, ra2, wa3;
	logic wd3;
	logic rd1, rd2;

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
			#5 $fdisplay(desc3, "%b %b || %b", we3, ra1, ra2, rd1, wa3, wd3, rd2);
		end
	initial
		begin
			/*
				VALUES
				*/
		end
endmodule //regfile_tb

