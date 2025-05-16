#ifndef __USERTASK_H__
#define __USERTASK_H__

#include "ti_msp_dl_config.h"
#include "./UART/UART.h"
#include "./GUI/GUI.h"
#include "./BTN/BTN.h"
#include "Tick.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RX_BUFFER_SIZE 30
#define SIGNAL_IN_mVPP  500.f
#define RESISTOR_IN    1000.0f     //信号源与放大器之间串联的电阻大小
#define RESISTOR_LOAD  1000.0f     //放大器负载电路大小

#define MAX_POINTS  64      //扫频点数
#define GAIN_MAX 200.0f     //最大增益
#define FREQ_START 100.0f   //起始频率
#define FREQ_END 300000.0f  //终止频率

#define EPSILON 0.01

enum {
    InputImp = 1,   //输入阻抗
    OutputImp = 2,  //输出阻抗
    Gain = 3,       //增益
    AmpFreq = 4,    //幅频曲线
};
//测量输出电阻时的状态
enum {
    Open = 1,       //负载开路
    Load = 2,       //负载接入
};
//非阻塞任务函数状态变量
typedef enum {
    IDLE,
    WAIT,
    PROCESS
}TaskState_t;
//绘制增益曲线任务状态变量
typedef enum {
    WAIT_COMMAND,
    START_ADC,
    WAIT_ADC,
    DRAW_POINT
}PlotState_t;

typedef struct Circuit_Paramter {
    float Ri;   //输入电阻
    float Ro;   //输出电阻
    float Av;   //电压增益
    float fh;   //3dB上截止频率
}Cir_param_t;

extern volatile uint8_t TaskMark;
extern BTNData_t BTNData;

extern volatile bool uart_rx_finish;
extern volatile char uart_rx_buffer[RX_BUFFER_SIZE];
extern volatile uint8_t uart_rx_index;
extern volatile bool gCheckADC;

extern Cir_param_t c_param;
extern TaskState_t InputImpState;
extern TaskState_t OutputImpState;
extern TaskState_t GainState;

//测量输出阻抗需要的全局变量
extern volatile uint8_t Out_state;
extern volatile float Vout_Load;     //接入负载时的输出电压
extern volatile float Vout_Open;     //负载开路时的输出电压
extern volatile bool Vout_flag;      //是否测量完成开路和负载输出电压的标志位

//绘制增益曲线需要的全局变量
extern PlotState_t PlotState;
extern volatile uint16_t point_index;
extern volatile uint16_t lastX;
extern volatile uint16_t lastY;
extern volatile float input_freq;    
extern volatile float input_amp;     
extern volatile float freq_buffer[MAX_POINTS];
extern volatile float gain_buffer[MAX_POINTS];

void TaskScan(void);
void BTNScan(void);
void CalInputImp(void);
void CalOutputImp(void);
void CalGain(void);
void PlotAmpFreq(void);

void get_3dbcutoff_freq(float *freq, float *gain);

float AD8310_Map(float Amp);
void parse_rx_buffer(char* , float* , float* );
uint16_t mapFreqToX(float freq);
uint16_t mapGainToY(float gain);

void Param_update(Cir_param_t cp);
void isError(void);

#ifdef __cplusplus
}
#endif

#endif