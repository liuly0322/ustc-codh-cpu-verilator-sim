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
    int sorted_cnt = 0;
    // x10 表明了结束了哪一项测试
    int x10 = 0;  // 默认为 0
    // x11 为 1 表明测试结束
    bool finished = false;

    top->eval();
    tfp->dump(main_time++);
    top->clk = 1;
    top->eval();
    tfp->dump(main_time++);
    top->rstn = 1;

    // 此时如果 x10 为 1，则通过所有测试
    while (!Verilated::gotFinish() && main_time < sim_time) {
        // 循环读取内存值
        int reg_index = main_time % 2 ? 10 : 11;  // 奇数取 10，偶数 11
        top->chk_addr = 0x1000 + reg_index;
        top->clk = !top->clk;
        top->eval();           // 仿真时间步进
        tfp->dump(main_time);  // 波形文件写入步进
        unsigned reg_val = (unsigned)(top->chk_data);
        if (reg_index == 11) {
            // 读取 1 判断测试是否结束
            if (reg_val == 1) {
                cout << "测试结束" << endl;
                finished = true;
            }
        } else {
            if (finished) {
                cout << (reg_val ? "通过全部测试！" : "有样例失败！") << endl;
                tfp->close();
                exit(0);
            }
            if (reg_val != x10 && reg_val > 1) {
                x10 = reg_val;
                cout << "结束运行样例：" << reg_val << endl;
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