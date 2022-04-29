// cpu-harness.cpp
#include <verilated.h>        // 核心头文件
#include <verilated_vcd_c.h>  // 波形生成头文件
#include <fstream>
#include <iostream>
#include "Vcpu.h"  // 译码器模块类
using namespace std;

Vcpu* top;           // 顶层dut对象指针
VerilatedVcdC* tfp;  // 波形生成对象指针

vluint64_t main_time = 0;           // 仿真时间戳
const vluint64_t sim_time = 10240;  // 最大仿真时间戳

int main(int argc, char** argv) {
    // 一些初始化工作
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);

    // 为对象分配内存空间
    top = new Vcpu;
    tfp = new VerilatedVcdC;

    // tfp初始化工作
    top->trace(tfp, 99);
    tfp->open("cpu.vcd");

    int count = 0;

    top->clk = 0;
    top->rstn = 0;
    top->chk_addr = 0x1005;
    int sorted_cnt = 0;
    // x5 表示通过了几项
    int x5 = 0;  // 默认为 0

    top->eval();
    tfp->dump(main_time++);
    top->clk = 1;
    top->eval();
    tfp->dump(main_time++);
    top->rstn = 1;

    while (!Verilated::gotFinish() && main_time < sim_time) {
        // 循环读取内存值
        top->clk = !top->clk;
        top->eval();           // 仿真时间步进
        tfp->dump(main_time);  // 波形文件写入步进
        unsigned reg_val = (unsigned)(top->chk_data);
        if (reg_val != x5) {
            cout << "通过测试" << reg_val << endl;
            x5 = reg_val;
            if (x5 == 8) {
                cout << "通过所有测试" << endl;
                tfp->close();
                exit(0);
            }
        }
        count++;
        main_time++;
    }

    cout << "未能在给定周期内结束测试，可以考虑手动调整 cpp 文件中周期设置"
         << endl;
    tfp->close();
    exit(0);
}