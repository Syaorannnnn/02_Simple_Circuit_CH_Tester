/*

*/

#ifndef __GUI_H__
#define __GUI_H__

#include <stdio.h>
#include "ti_msp_dl_config.h"
#include "./UART/UART.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************/
/***    参数定义    ***/
#define     BUFFER_LEN_MAX      100     //发送指令缓冲区最大长度
/*********************************************/
/***  绘图参数    ***/
#define     RADIUS          3           //画点半径
#define     PI              3.14159265358979
#define     SCALE           3

/*  颜色(十进制)    */
#define     RED             63488
#define     BLUE            31
#define     GRAY            33840
#define     BLACK           0
#define     WHITE           65535
#define     GREEN           2016
#define     BROWN           48192
#define     YELLOW          65504
/******************************************************/
/*  背景色  */
#define     BACKGROUND          WHITE
/******************************************************/
/*
 * 新示波器窗口参数
 * x轴 400-700     一小格75
 * y轴 50-350		一小格60
 *
 */

#define     SCOPE_X         400
#define     SCOPE_Y         700
#define     SCOPE_WIDTH     300
#define     SCOPE_HEIGHT    300
#define     TOTAL_DOTS      37          //x轴每大格分为10小格，一共37个小格
/*
 * 一小格里面各对数刻度宽度(从左至右)
 * 20 15 10 8 5 5 5 4 3
 */
//uint8_t log_scale[9];

/***********  绘图功能函数    **********/
void clr_scr(void);                                             //清屏
void draw_dot(int x,int y,int color);                           //画点
void draw_line(int x1,int y1,int x2,int y2,int color);          //画线

#ifdef __cplusplus
}
#endif

#endif