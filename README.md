# USTC-COD-VERILATOR

USTC CODH 课程的 CPU Verilog 代码的自动化测试

目前确认支持 Ubuntu 系统，理论上别的 Linux 发行版问题不大

## 依赖

安装 Verilator

Ubuntu 用户可以直接：

```
sudo apt install verilator
```

Verilator 注意事项，来源 [fdu-ics](https://fducslg.github.io/ICS-2021Spring-FDU/misc/verilate.html) :

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
- 请注意更改 ip 核模块名为 `dist_ir` （指令寄存器）和 `dist_mem`（数据寄存器），约定这两个都是 256\*32 的分布式存储器 ip 核
  - 如果你不想改变你的源文件，可以把 headers 下的 dist_ir.v 和 dist_mem.v 的文件名以及文件内部的模块名对应更改为你的 ip 核模块名
  - 关于 ip 核，请确认：指令寄存器单端口，数据寄存器因为 debug_bus 的缘故是双端口，端口信息可以到具体文件确认
- .text 从 0x3000 开始， .data 从 0x0000 开始
- **请务必实现 bne**

以上大部分约定来源于之前的 lab4，这里沿袭了传统

随后可以在当前目录运行 python 脚本：

```bash
python judge.py
```

运行之后会输出结果提示，并且在项目目录下生成 `cpu.vcd`，供 debug 查看（如果需要）

如果因为语法原因编译错误请检查提示，其实 Verilator 当 linter 提示作用挺强的

目前支持的 testcase:

- `sort_vcd`，代表测试排序
- `bypass`，这是一个系列（`bypass` 本身没有意义），具体每个 testcase 的含义参见 [这里](https://github.com/cs3001h/cs3001h.tests/blob/main/isa/rv32mi/bypass.S) ，其中每个宏的第一个参数是 testcase 的编号，拆分的原因是考虑指令寄存器 256\*32 的限制
  - `bypass1` test2 到 test22
  - `bypass2` test23 到 test45
  - `bypass3` test46 到 test64
  - `bypass4` test65 到 test83
  - `bypass5` test84 到 test94
- `no_hazard` 基本指令功能测试，~~如果你能找到这个仓库但是找不到测试案例含义可以戳我~~

## 定制

如果本项目的一些配置不满足你的要求（例如你的存储器比较大，不止 256，可以自行修改 headers 文件夹内的 dist_ir.v 和 dist_mem.v）

本项目的基本配置文件都在 headers 下

## 贡献

本项目的基本思路是每个周期利用 `chk_addr` 查询，并读出对应的 `chk_data`，进行正确性验证

本项目对于每个测试需要配套编写 cpu\_\*.cpp, \*\_data.coe 和 \*\_text.coe

关于编写测试样例，可以参考[这篇](http://www.sunnychen.top/2019/07/25/%E8%B7%A8%E8%AF%AD%E8%A8%80%E7%9A%84Verilator%E4%BB%BF%E7%9C%9F%EF%BC%9A%E4%BD%BF%E7%94%A8%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E4%BF%A1/) verilator 的使用文章

之后即可在 python 脚本 repl 流程中载入

欢迎 issue/pr
