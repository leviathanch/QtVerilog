module counter (
out, // Output of the counter
enable, // enable for counter
clk, // clock Input
reset // reset Input
);
	output [7:0] out;
	input enable, clk, reset;
	reg [7:0] out;
	always @(posedge clk)
		if (reset) begin
			out <= 8'b0 ;
		end else if (enable) begin
			out <= out + 1;
		end
endmodule
