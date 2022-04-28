module  cpu (
        input clk,
        input rstn,

        // IO_BUS
        output [7:0]  io_addr,	// 外设地址
        output [31:0]  io_dout,	// 向外设输出的数据
        output  io_we,		    // 向外设输出数据时的写使能信号
        output  io_rd,		    // 从外设输入数据时的读使能信号
        input [31:0]  io_din,	    // 来自外设输入的数据

        // Debug_BUS
        output [31:0] pc,      	// 当前执行指令地址
        input [15:0] chk_addr,	// 数据通路状态的编码地址
        output [31:0] chk_data    // 数据通路状态的数据
    );

    // For example
    // 评测是靠 chk_addr 和 chk_data 实现
    // 因此请确认端口名

endmodule
