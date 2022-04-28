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

unsigned int data[16] = {0x814d0bcf, 0x900b07d1, 0xc5246b59, 0x85f34240,
                         0xf0648f06, 0xa726af47, 0x59e970a2, 0x79f57ff2,
                         0x4dd57a20, 0x6ffe43a2, 0x8eee87a2, 0xa96ab6ff,
                         0x3bbbd029, 0xaf1b67c7, 0xaf3401e6, 0x35336804};

int main(int argc, char** argv) {
    // 先用 cpp 进行冒泡排序
    int n = 16;
    for (bool sorted = false; sorted = !sorted; n--)
        for (int i = 1; i < n; i++)
            if ((int)data[i - 1] > (int)data[i]) {
                swap(data[i - 1], data[i]);
                sorted = false;
            }

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
    top->rstn = 1;
    int sorted_cnt = 0;
    while (!Verilated::gotFinish() && main_time < sim_time) {
        // 循环读取内存值
        int i = main_time % 16;
        top->chk_addr = 0x2000 + i * 4;
        top->clk = !top->clk;
        top->eval();           // 仿真时间步进
        tfp->dump(main_time);  // 波形文件写入步进
        unsigned mem_val = (unsigned)(top->chk_data);
        if (mem_val == data[i]) {
            sorted_cnt++;
            if (sorted_cnt == 16) {
                cout << "排序完成！" << endl;
                cout << "总耗费周期：" << main_time << endl;
                tfp->close();
                exit(0);
            }
        } else {
            sorted_cnt = 0;
        }
        count++;
        main_time++;
    }

    // 清理工作
    cout << "未能在给定周期内完成排序" << endl;
    cout << "当前周期：" << main_time << endl;
    tfp->close();
    exit(0);
}