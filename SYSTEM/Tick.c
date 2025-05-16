#include "Tick.h"

volatile uint32_t Tick = 0;
volatile uint32_t delay_times = 0;

void delay_ms(uint8_t ms)
{
    delay_times = ms;
    while( delay_times != 0);
}

// SysTick中断处理函数(1ms)
void SysTick_Handler(void) {
    if(delay_times != 0)    delay_times--;
    /*    按键SYSTICK中断     */
    Tick++;
    // 按键消抖倒计时
    if (!BTN_LEFT_PRESS && BTNTick.left) BTNTick.left--;
    if (!BTN_DOWN_PRESS && BTNTick.down) BTNTick.down--;
    if (!BTN_RIGHT_PRESS && BTNTick.right) BTNTick.right--;
    if (!BTN_UP_PRESS && BTNTick.up) BTNTick.up--;
    if (!BTN_MID_PRESS && BTNTick.mid) BTNTick.mid--;
}