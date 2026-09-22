#include "common.h"
#include "delay.h"
#include "stc.h"
#include "usb.h"
#include "uart.h"
#include <string.h>
#include "stdarg.h"

#include "coremark.h"

/**
 * 检查是否接收到特定的ISP固件升级请求。
 * 需要配合usb cdc使用,每次接收到信息,先检查这个
 */
void checkISP()
{
	// 怕编译器优化太傻，不能优化strlen,直接写8
	if (memcmp("@STCISP#", RxBuffer, 8) == 0)
	{
		usb_write_reg(OUTCSR1, 0);
		USBCON = 0x00;
		USBCLK = 0x00;
		IRC48MCR = 0x00;

		P3M0 &= ~0x03; // 设置为高阻
		P3M1 |= 0x03;
		delay_ms(10); // 留足时间给usb线插好
		// 复位到bootloader
		IAP_CONTR = 0x60;
		while (1)
			;
	}
}

char putchar(char c)
{
	// 如果usb没有连接，发送会卡住，这里检查是否连接再发送
	if (DeviceState != DEVSTATE_CONFIGURED)
	{
		return c;
	}

	// 如果usb已连接，但电脑上没有打开该串口，这时发送也会卡住
	// 观察到在win11上，打开串口后，CDC_DTR会变成1，反之为0
	// 不知道这个行为能否推广到其他系统。
	if (CDC_DTR == 0)
	{
		return c;
	}

	// 无优化，这样打印会很慢
	TxBuffer[0] = c;
	uart_send(1);
	return c;
}

/* ============================================================
 * 输出接口：HAS_PRINTF == 0，需自行实现 ee_printf
 * ============================================================ */
int ee_printf(const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = vprintf(fmt, ap);   /* 直接转发给 vprintf */
    va_end(ap);

    return len;
}

//10ms自增1
uint32_t lovely_timer=0;
void Timer0_Isr(void) interrupt TMR0_VECTOR
{
	lovely_timer++;
	if(lovely_timer%100==0){
	     P00 = !P00;		//LED flip for each second
	}
}

void main(void)
{
	WTST = 0;  // 设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXFR = 1; // 扩展寄存器(XFR)访问使能
	CKCON = 0; // 提高访问XRAM速度

	P0M1 = 0x00;   P0M0 = 0xff;   //设置P0为推挽输出 实验箱流水灯对应io
    P4M1 = 0x00;   P4M0 = 0x80;   //设置为准双向口
    P40 = 0; // 实验箱流水灯供电开关

	uart_init();
	usb_init();
	EA = 1;



//定时器0 10毫秒@40.000MHz
{
	TM0PS = 0x07;			//设置定时器时钟预分频 ( 注意:并非所有系列都有此寄存器,详情请查看数据手册 )
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xB0;				//设置定时初始值
	TH0 = 0x3C;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

P02=0;
delay_ms(3000);
printf("%d",core_main());
P03=0;

	while (1)
	{
		if (RxFlag)
		{
			checkISP();
			uart_recv_done(); // 对接收的数据处理完成后,一定要调用一次这个函数,以便CDC接收下一笔串口数据
      printf("%.*s", RxCount, RxBuffer);
		}
	}
}