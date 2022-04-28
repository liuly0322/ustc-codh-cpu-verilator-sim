module dist_ir(
    input  [7:0] a,
    input  [31:0] d,
    input  clk,
    input  we,
    output [31:0] spo);

    reg [31: 0] mem[0: 255];

    initial begin
       $readmemh("headers/text.coe", mem);
    end

    always @(posedge clk)
        if (we) mem[a] <= d;
    
    assign spo = mem[a];
endmodule
