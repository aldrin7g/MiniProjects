module csa(
    input [3:0] a, b,
    input cin,
    output [3:0] sum,
    output cout
);
    
    wire [3:0] sum0, sum1;
    wire cout0, cout1, sel;
    
    // Ripple Carry Adder with carry-in = 0
    ripple_carry_adder_4bit rca0 (
        .a(a), .b(b), .cin(1'b0), .sum(sum0), .cout(cout0)
    );
    
    // Ripple Carry Adder with carry-in = 1
    ripple_carry_adder_4bit rca1 (
        .a(a), .b(b), .cin(1'b1), .sum(sum1), .cout(cout1)
    );
    
    // Carry Select Logic
    assign sel = cin;
    assign sum = sel ? sum1 : sum0;
    assign cout = sel ? cout1 : cout0;
    
endmodule

// 4-bit Ripple Carry Adder Module
module ripple_carry_adder_4bit(
    input [3:0] a, b,
    input cin,
    output [3:0] sum,
    output cout
);
    wire c1, c2, c3;
    
    full_adder fa0 (.a(a[0]), .b(b[0]), .cin(cin),  .sum(sum[0]), .cout(c1));
    full_adder fa1 (.a(a[1]), .b(b[1]), .cin(c1),   .sum(sum[1]), .cout(c2));
    full_adder fa2 (.a(a[2]), .b(b[2]), .cin(c2),   .sum(sum[2]), .cout(c3));
    full_adder fa3 (.a(a[3]), .b(b[3]), .cin(c3),   .sum(sum[3]), .cout(cout));
    
endmodule

// Full Adder Module
module full_adder(
    input a, b, cin,
    output sum, cout
);
    assign sum = a ^ b ^ cin;
    assign cout = (a & b) | (b & cin) | (a & cin);
    
endmodule
