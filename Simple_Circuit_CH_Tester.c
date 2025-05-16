#include "ti_msp_dl_config.h"
#include "./UART/UART.h"
#include "./BTN/BTN.h"
#include "UserTask.h"


volatile bool gCheckADC = false;

volatile bool uart_rx_finish = false;
volatile char uart_rx_buffer[RX_BUFFER_SIZE];
volatile uint8_t uart_rx_index = 0;

Cir_param_t c_param = {0.0};
TaskState_t InputImpState = IDLE;
TaskState_t OutputImpState = IDLE;
TaskState_t GainState = IDLE;

volatile uint8_t Out_state = Load;
volatile float Vout_Load = 0.0;     //接入负载时的输出电压
volatile float Vout_Open = 0.0;     //负载开路时的输出电压
volatile bool Vout_flag = false;    //是否测量完成开路和负载输出电压的标志位

PlotState_t PlotState = WAIT_COMMAND;
volatile uint16_t point_index = 0;
volatile uint16_t lastX = -1;
volatile uint16_t lastY = -1;
volatile float input_freq = 0.0;    //串口接收到的输入信号的频率
volatile float input_amp = 0.0;     //串口接收到的输入信号的幅值
volatile float freq_buffer[MAX_POINTS];
volatile float gain_buffer[MAX_POINTS];

BTNData_t BTNData = {0};

volatile uint8_t TaskMark = 0;  //执行任务标志位，初始不执行任何任务

int main(void)
{
    SYSCFG_DL_init();  
	//NVIC_EnableIRQ(UART_0_INST_INT_IRQN);

    while (1)
    {
		BTNScan();
        TaskScan();
    }
}

void UART_0_INST_IRQHandler(void) {
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_R_PIN);
            uint8_t data = DL_UART_Main_receiveData(UART_0_INST);
            if(data == '\n') {
                uart_rx_index = 0;
            }
            else if(data == '\r') {
                uart_rx_buffer[uart_rx_index] = '\0';
                uart_rx_finish = true;
                NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
            }
            else if(uart_rx_index < RX_BUFFER_SIZE - 1 && !uart_rx_finish) {
                uart_rx_buffer[uart_rx_index++] = data;
            }
            break;

        default:
            break;
    }
}

void ADC12_0_INST_IRQHandler(void) {
    switch (DL_ADC12_getPendingInterrupt(ADC12_0_INST)) {
        case DL_ADC12_IIDX_MEM1_RESULT_LOADED:
            DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_G_PIN);
            gCheckADC = true;
            break;
        default:
            break;
    }
}