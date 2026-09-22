# CoreMark for AI8051U

在 STC AI8051U 上运行 CoreMark 1.01 基准测试  
结果会通过虚拟串口输出  
coremark要求2k内存，edata不够，只能放到xdata  

可调整`TOTAL_DATA_SIZE`降低运算内存占用把内存放edata，但这会同时降低运算量。只有TOTAL_DATA_SIZE=2000才是标准的coremark分数。
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
