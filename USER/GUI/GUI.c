/**
 * @file GUI.c
 * @author Aiden 
 * @brief 画面绘制
 * @version 0.1
 * @date 2025-05-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "GUI.h"

char str[BUFFER_LEN_MAX];
/*
 * 一小格里面各对数刻度宽度(从左至右)
 * 20 15 10 8 5 5 5 4 3
 */
uint8_t log_scale[9] = {20,15,10,8,5,5,5,4,3};

/**
 * @brief 清屏
 * 
 */
void clr_scr(void)
{
    sprintf(str,"clr WHITE\xff\xff\xff");
    sendString(str, UART_2_INST);
    sprintf(str,"ref p0\xff\xff\xff");
    sendString(str, UART_2_INST);
}
/**
 * @brief 画点
 * 
 * @param x 
 * @param y 
 * @param color 
 */
void draw_dot(int x, int y,int color)
{
    sprintf(str,"cirs %d,%d,%d,%d\xff\xff\xff",x,y,RADIUS,color);
    sendString(str, UART_2_INST);
    //delay_cycles(1000);
}

/**
 * @brief 画线
 * 
 * @param x1 
 * @param y1 
 * @param x2 
 * @param y2 
 * @param color 
 */
void draw_line(int x1, int y1, int x2, int y2,int color)
{
    sprintf(str,"line %d,%d,%d,%d,%d\xff\xff\xff",x1,y1,x2,y2,color);
    sendString(str, UART_2_INST);
    //delay_cycles(1000);
}

