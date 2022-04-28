# USTC-COD-VERILATOR

USTC CODH 课程的 CPU Verilog 代码的自动化测试

## 依赖

安装 Verilator

> 注意事项，来源 [fdu-ics](https://fducslg.github.io/ICS-2021Spring-FDU/misc/verilate.html)
> Verilator 目前依然有许多不足之处。首先 Verilator 对 SystemVerilog 的语言支持还非常不完整，比如 unpacked 结构体是不支持的。此外 `interface`、`package` 这些关键字虽然支持，但是在功能上还不够完善。为了避免你的 SystemVerilog 代码不能通过 Verilator 的综合和不正确的仿真行为，请**尽量避免**以下事项：
>
> - 不可综合的语法，例如延时。
> - `initial` 语句。
> - unpacked 数组、结构体。
> - `interface`、`package`、`class`。
> - 小端序位标号，如 `[0:31]`。
> - 锁存器。
> - `logic` 类型的 `X` 状态和高阻抗 `Z` 状态。
> - 使用时钟下降沿触发。
> - 异步 reset 和跨时钟域。
> - 尝试屏蔽全局时钟信号。
>   此外，我们建议每个 SystemVerilog 文件只放一个模块，并且文件名和模块名保持一致。

## 使用

将该项目 clone 到本地

将你的 CPU 用到的模块文件全部放在 src 目录下，约定：

- 存在文件名 cpu.v ，包含 cpu 模块
- 尽量每个模块单独一个文件
- 模块名一定要与文件名相同，不然可能找不到
- 请注意更改 ip 核模块名为 `dist_ir` （指令寄存器）和 `dist_mem`（数据寄存器）

随后可以在当前目录运行 python 脚本：

```bash
python judge.py
```

目前仅支持 `sort_vcd`，代表测试排序并生成 vcd 文件
