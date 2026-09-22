# CoreMark 跑分记录：AI8051U @ 40MHz

## 硬件与软件环境

| 项目                   | 说明                         |
| ---------------------- | ---------------------------- |
| **开发板**       | AI8051U 实验箱 V1.2          |
| **主频**         | 40 MHz                       |
| **运行模式**     | 32 位模式                    |
| **硬件加速**     | MDU32（硬件 32 位乘除法器）  |
| **数据存放位置** | xdata（32KB 外部数据存储区） |
| **编译器**       | Keil C251 V5.60.0.0          |
| **优化选项**     | OPTIMIZE(9, SPEED)           |

## CoreMark 跑分结果

下载固件后，约十几秒后，通过usb cdc串口发送过来结果

```
2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 1286
Total time (secs): 12.860000
Iterations/Sec   : 23.328150
Iterations       : 300
Compiler version : Keil C251 V5.60.0.0
Compiler flags   : OPTIMIZE(9, SPEED)
Memory location  : xdata
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x5275
Correct operation validated. See readme.txt for run and reporting rules.
CoreMark 1.0 : 23.328150 / Keil C251 V5.60.0.0 OPTIMIZE(9, SPEED) / xdata
```

## 关键指标

| 指标          | 数值                           |
| ------------- | ------------------------------ |
| CoreMark 得分 | **23.33** Iterations/Sec |
| 总运行时间    | 12.86 秒                       |
| 迭代次数      | 300                            |
| 功能验证      | ? 正确通过                    |

## 源码与移植说明

- **源码仓库**：https://github.com/liux-pro/coremark-ai8051u
- **移植改动**：对 CoreMark 源码有少量修改才能编译通过，具体改动详见仓库 commit 记录
- **数据存放**：数据放置于 `xdata` 区域

> ?? **注意**：跑分结果受编译器版本、优化选项、存储位置等因素影响，**仅供参考**，不代表芯片在实际应用中的绝对性能。