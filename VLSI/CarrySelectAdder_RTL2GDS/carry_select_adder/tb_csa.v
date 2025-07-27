`timescale 1ns / 1ps

module tb_csa;
    reg [3:0] a, b;
    reg cin;
    wire [3:0] sum;
    wire cout;
    
    // Instantiate the Carry Select Adder
    csa uut (
        .a(a), .b(b), .cin(cin), .sum(sum), .cout(cout)
    );
    
    initial begin
        // Open VCD file for waveform dumping
        $dumpfile("csa.vcd");
        $dumpvars(0, tb_csa);
        
        // Test cases
        a = 4'b0000; b = 4'b0000; cin = 0; #10;
        a = 4'b0001; b = 4'b0001; cin = 0; #10;
        a = 4'b0011; b = 4'b0101; cin = 0; #10;
        a = 4'b0110; b = 4'b0011; cin = 1; #10;
        a = 4'b1111; b = 4'b1111; cin = 0; #10;
        a = 4'b1010; b = 4'b0101; cin = 1; #10;
        
        // End simulation
        $finish;
    end
    
    initial begin
        $monitor("Time = %0t | a = %b | b = %b | cin = %b | sum = %b | cout = %b", 
                 $time, a, b, cin, sum, cout);
    end
    
endmodule
