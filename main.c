#include "common.h"
#include "delay.h"
#include "stc.h"
#include "usb.h"
#include "uart.h"
#include <string.h>

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




void main(void)
{
	WTST = 0;  // 设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXFR = 1; // 扩展寄存器(XFR)访问使能
	CKCON = 0; // 提高访问XRAM速度


	uart_init();
	usb_init();
	EA = 1;






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