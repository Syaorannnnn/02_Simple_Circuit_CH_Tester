/*

*/

#ifndef __UART_H__
#define __UART_H__

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ti/driverlib/dl_uart_main.h>
#include "ti_msp_dl_config.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************/
/*数据发送程序*/
void sendData(uint8_t data, UART_Regs *huart);
void sendString(char *p, UART_Regs *huart);
void sendNum(float num, int decimalPlaces, UART_Regs *huart);

/******************************************************************/
/*以下是串口屏页面切换可能会用到的功能*/

/*
 * 以下代码只在上电时运行一次,一般用于全局变量定义和上电初始化数据
 * int sys0=0,sys1=0,sys2=0     //全局变量定义目前仅支持4字节有符号整形(int),不支持其他类型的全局变量声明,如需使用字符串类型可以在页面中使用变量控件来实现
 * baud=115200//配置波特率
 * dim=100//配置屏幕背光
 * recmod=0//串口协议模式:0-字符串指令协议;1-主动解析协议
 * bkcmd=2
 * printh 00 00 00 ff ff ff 88 ff ff ff//输出上电信息到串口
 * page 0                       //上电刷新第0页
*/

#define     RINGBUFF_LEN	    (500)                   //定义最大接收字节数 500
#define     usize               getRingBuffLength()
#define     u_init()            initRingBuff()
#define     udelete(x)          deleteRingBuff(x)
#define     u(x)                read1BFromRingBuff(x)

/*环形缓冲区结构体*/
typedef struct RingBuff
{
	uint16_t Head;
	uint16_t Tail;
	uint16_t Length;
	uint8_t  Ring_data[RINGBUFF_LEN];
}RingBuff_t;

void initRingBuff(void);
void writeRingBuff(uint8_t data);
void deleteRingBuff(uint16_t size);
uint16_t getRingBuffLength(void);
uint8_t read1BFromRingBuff(uint16_t position);

#ifdef __cplusplus
}
#endif

#endif	/* __UART_H__ */