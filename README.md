# USTC-COD-CPUTEST

本项目具有实验性质

## 依赖

安装 Verilator

注意事项，来源 [fdu-ics](https://fducslg.github.io/ICS-2021Spring-FDU/misc/verilate.html)

Verilator 目前依然有许多不足之处。首先 Verilator 对 SystemVerilog 的语言支持还非常不完整，比如 unpacked 结构体是不支持的。此外 `interface`、`package` 这些关键字虽然支持，但是在功能上还不够完善。为了避免你的 SystemVerilog 代码不能通过 Verilator 的综合和不正确的仿真行为，请**尽量避免**以下事项：

- 不可综合的语法，例如延时。
- `initial` 语句。
- unpacked 数组、结构体。
- `interface`、`package`、`class`。
- 小端序位标号，如 `[0:31]`。
- 锁存器。
- `logic` 类型的 `X` 状态和高阻抗 `Z` 状态。
- 使用时钟下降沿触发。
- 异步 reset 和跨时钟域。
- 尝试屏蔽全局时钟信号。

此外，我们建议每个 SystemVerilog 文件只放一个模块，并且文件名和模块名保持一致。

## 使用

```bash
cd src
verilator --cc cpu.v --trace --exe cpu_harness.cpp
make -j -C ./obj_dir -f Vcpu.mk Vcpu
./obj_dir/Vcpu
```

可以生成波形图，也可以在 `cpu_harness.cpp` 中修改评测配置，例如可以考虑每个周期更改输入的 `chk_addr`，利用 `chk_data` 和 `pc` 的输出判断程序是否正常运行，利用标准输出，读取终端信息判断程序是否运行正常

仿真时间也在该仿真文件中进行配置：

```cpp
const vluint64_t sim_time = 10240;  // 最大仿真时间戳
```

本项目默认载入了一个排序程序，并进行了正确性判断

## 说明

两个 256 x 32 的寄存器分别被 `sort_data.coe` 和 `sort_text.coe` 初始化

请注意更改模块名字为 `dist_ir` （指令寄存器）和 `dist_mem`（数据寄存器）
