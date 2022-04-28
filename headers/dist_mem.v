module dist_mem(
    input  [7:0] a,
    input  [7:0] dpra,
    input  [31:0] d,
    input  clk,
    input  we,
    output [31:0] spo,
    output [31:0] dpo);

    reg [31: 0] mem[0: 255];

    initial begin
       $readmemh("headers/data.coe", mem);
    end

    always @(posedge clk)
        if (we) mem[a] <= d;
    
    assign spo = mem[a];
    assign dpo = mem[dpra];
endmodule
