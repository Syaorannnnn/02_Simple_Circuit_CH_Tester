/**
 * @file UserTask.c
 * @author  
 * @brief 用户任务
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "UserTask.h"

/*
@brief: 轮询扫描当前该执行的任务
*/
void TaskScan(void) {
    switch (TaskMark) {
        case InputImp:
            //sendString("CalInputImp()\r\n");
            CalInputImp();
            break;
        case OutputImp:
            //sendString("CalOutputImp()\r\n");
            CalOutputImp();
            break;
        case Gain:
            //sendString("CalGain()\r\n");
            CalGain();
            break;
        case AmpFreq:
            //sendString("PlotAmpFreq()\r\n");
            PlotAmpFreq();
            break;
        default:
            break;
    }
}

/*
@brief: 轮询扫描按键的状态，判断哪个按键被按下
*/
void BTNScan(void) {
    BTN_getData(&BTNData);
    if(BTNData.left) {
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_B_PIN);
        //sendString("left pressed\r\n");
        TaskMark = InputImp;    //left按键对应“测量输入阻抗”
    }
    if(BTNData.right) {
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_B_PIN);
        //sendString("right pressed\r\n");
        TaskMark = OutputImp;   //right按键对应“测量输出阻抗”
    }
    if(BTNData.down) {
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_B_PIN);
        //sendString("down pressed\r\n");
        TaskMark = Gain;        //down按键对应“测量增益”
    }
    if(BTNData.up) {
        DL_GPIO_togglePins(GPIO_LED_PORT, GPIO_LED_B_PIN);
        //sendString("up pressed\r\n");
        TaskMark = AmpFreq;     //up按键对应“绘制幅频曲线”
    }
}

/*
@brief: 测量输入阻抗
*/
void CalInputImp(void) {
    switch (InputImpState) {
        case IDLE:
            NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);  //使能ADC中断
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);
            InputImpState = WAIT;
            break;
        case WAIT:
            if(gCheckADC) {
                InputImpState = PROCESS;
            }
            break;
        case PROCESS:
            uint16_t InVol = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_0);
            float InAmp = AD8310_Map((float)InVol * 3300 / 4095);
            float R_i = InAmp / (SIGNAL_IN_mVPP - InAmp) * RESISTOR_IN;
            c_param.Ri = R_i;
            sendString("Input Resistor: ",UART_1_INST); sendNum(R_i, 2,UART_1_INST); sendString("\r\n",UART_1_INST);

            gCheckADC = false;
            InputImpState = IDLE;
            break;
    }
    //按键打断当前任务
    if(TaskMark != InputImp) {
        InputImpState = IDLE;
        NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
    }
}

/*
@brief: 测量输出阻抗
*/
void CalOutputImp(void) {
    switch (OutputImpState) {
        case IDLE:
            NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);  //使能ADC中断
            DL_GPIO_clearPins(GPIO_CON_PORT, GPIO_CON_OUT_PIN);
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);
            Out_state = Open;       //先测量开路的输出电压
            Vout_flag = false;
            OutputImpState = WAIT;
            break;
        case WAIT:
            if(gCheckADC) {
                OutputImpState = PROCESS;
            }
            break;
        case PROCESS:
            if(Out_state == Open) {
                uint16_t OutVol = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
                Vout_Open = AD8310_Map((float)OutVol * 3300 / 4095);
                Out_state = Load;

                DL_GPIO_setPins(GPIO_CON_PORT, GPIO_CON_OUT_PIN);   //吸合继电器 -> 连接负载
                delay_ms(50);   //稳定延时
                DL_ADC12_enableConversions(ADC12_0_INST);
                DL_ADC12_startConversion(ADC12_0_INST);
                gCheckADC = false;
                OutputImpState = WAIT;
            }
            else if(Out_state == Load) {
                uint16_t OutVol = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
                Vout_Load = AD8310_Map((float)OutVol * 3300 / 4095);
                float R_o = (Vout_Open / Vout_Load - 1.0f) * RESISTOR_LOAD;
                c_param.Ro = R_o;

                sendString("Output Resistor: ",UART_1_INST); sendNum(R_o, 2,UART_1_INST); sendString("\r\n",UART_1_INST);

                gCheckADC = false;
                OutputImpState = IDLE;
                DL_GPIO_clearPins(GPIO_CON_PORT, GPIO_CON_OUT_PIN);   //断开负载
            }
            break;
    }
    if(TaskMark != OutputImp) {
        OutputImpState = IDLE;
        NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
        DL_GPIO_clearPins(GPIO_CON_PORT, GPIO_CON_OUT_PIN);   //断开负载
    }
}

/**
 * @brief 测量增益
 */
void CalGain(void) {
    switch (GainState) {
        case IDLE:
            NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);  //使能ADC中断
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);
            GainState = WAIT;
            break;
        case WAIT:
            if(gCheckADC) {
                GainState = PROCESS;
            }
            break;
        case PROCESS:
            uint16_t OutVol = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
            float OutAmp = AD8310_Map((float)OutVol * 3300 / 4095);
            float Gain = OutAmp / SIGNAL_IN_mVPP;
            c_param.Av = Gain;

            sendString("Gain: ",UART_1_INST); sendNum(Gain, 2,UART_1_INST); sendString("\r\n",UART_1_INST);

            gCheckADC = false;
            GainState = IDLE;
            NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
            break;
    }
    if(TaskMark != Gain) {
        GainState = IDLE;
        NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
    }
}

/**
 * @brief 绘制幅频曲线
 * 
 */
void PlotAmpFreq(void) {
    switch (PlotState) {
        case WAIT_COMMAND:
            NVIC_EnableIRQ(UART_0_INST_INT_IRQN);   //开启串口中断
            NVIC_EnableIRQ(ADC12_0_INST_INT_IRQN);  //使能ADC中断
            if(uart_rx_finish) {
                parse_rx_buffer((char *)uart_rx_buffer, &input_freq, &input_amp);
                uart_rx_finish = false;
                uart_rx_index = 0;
                NVIC_DisableIRQ(UART_0_INST_INT_IRQN);  //关闭中断直到采样完成
                PlotState = START_ADC;
            }
            break;
        case START_ADC:
            DL_ADC12_enableConversions(ADC12_0_INST);
            DL_ADC12_startConversion(ADC12_0_INST);
            PlotState = WAIT_ADC;
            break;
        case WAIT_ADC:
            if(gCheckADC) {
                gCheckADC = false;
                PlotState = DRAW_POINT;
            }
            break;
        case DRAW_POINT:
            uint16_t OutVol = DL_ADC12_getMemResult(ADC12_0_INST, DL_ADC12_MEM_IDX_1);
            float OutAmp = AD8310_Map((float)OutVol * 3300 / 4095);
            float gain = OutAmp / input_amp;
            if(point_index < MAX_POINTS) {
                freq_buffer[point_index] = input_freq;
                gain_buffer[point_index] = gain;
                point_index++;
            }
            uint16_t x = mapFreqToX(input_freq);
            uint16_t y = mapGainToY(gain);
            //画线
            if(lastX >= 0) {
                draw_line(x, y, lastX, lastY, BLUE);
                
            }
            //画第一个点
            else {
                draw_dot(x,y, BLUE);
            }
            lastX = x; lastY = y;
            //绘制完毕，可以计算上截频并更新参数了
            if(point_index == MAX_POINTS - 1) 
            {
                get_3dbcutoff_freq(freq_buffer, gain_buffer);
                Param_update(c_param);
            }
            PlotState = WAIT_COMMAND;
            break;
    }
    if(TaskMark != AmpFreq) {
        PlotState = WAIT_COMMAND;
        point_index = 0;
        lastX = lastY = -1;
        NVIC_DisableIRQ(UART_0_INST_INT_IRQN);
        NVIC_DisableIRQ(ADC12_0_INST_INT_IRQN);
    }
}

/** 
 * @brief AD8310输出电平 -> 输入信号幅值
 */
float AD8310_Map(float Amp) {
    float res = 0.0;
    //拟合公式: Vout = 491.7170 * log10(Vin) + 930.7041
    res = 491.7170 * log10(Amp) + 930.7041;
    return res;
}

/**
 * @brief 将串口接收的信号相关信息解包
 * 
 * @param buffer - 串口缓冲区指针
 * @param freq - 频率
 * @param mag - 幅度
 */
void parse_rx_buffer(char* buffer, float* freq, float* mag) {
    *freq = strtof(buffer, NULL);
    *mag = strtof(strchr(buffer, ' ') + 1, NULL);
}

/**
 * @brief 将频率映射到显示屏的X坐标
 * 
 * @param freq 
 * @return uint16_t
 */
uint16_t mapFreqToX(float freq) {
    float logF = log10f(freq);
    float logStart = log10f(FREQ_START);
    float logEnd = log10f(FREQ_END);
    uint16_t res = (uint16_t)(((logF - logStart) / (logEnd - logStart)) * SCOPE_WIDTH /*此处应为显示屏的宽度*/);
    return res;
}

/**
 * @brief 将增益映射到显示屏的Y坐标
 * 
 * @param gain 
 * @return uint16_t 
 */
uint16_t mapGainToY(float gain) {
    if(gain > GAIN_MAX) gain = GAIN_MAX;
    if(gain < 0) gain = 0;
    uint16_t res = (uint16_t)((1.0f - gain / GAIN_MAX) * SCOPE_HEIGHT /*此处应为显示屏的高度*/);
    return res;
}

/**
 * @brief 获取-3dB点的对应频率（上截止频率）
 * 
 * @param freq 
 * @param gain 
 */
void get_3dbcutoff_freq(float *freq, float *gain)
{
    float gain_max = gain[0];
    for(int i = 1; i < MAX_POINTS; i++)
    {
        if(gain[i] > gain_max)      gain_max = gain[i];
    }

    float gain_cutoff = 0.707 * gain_max;
    //反方向遍历寻找上截频
    for(int j = MAX_POINTS; j > 0; j--)
    {
        //找到第一个大于等于-3dB增益的那个频率点
        if(gain[j] >= gain_cutoff)
        {
            c_param.fh = freq[j];
            break;
        }
    }
}


/**
 * @brief 更新屏幕上电路参数
 * 
 * @param cp 
 */
void Param_update(Cir_param_t cp)
{
    char str[20];
    sprintf(str, "Ri.txt=\"%.2f\"\xff\xff\xff",cp.Ri);
    sendString(str, UART_2_INST);
    sprintf(str, "Ro.txt=\"%.2f\"\xff\xff\xff",cp.Ro);
    sendString(str, UART_2_INST);
    sprintf(str, "Av.txt=\"%.2f\"\xff\xff\xff",cp.Av);
    sendString(str, UART_2_INST);
    sprintf(str, "fh.txt=\"%.2f\"\xff\xff\xff",cp.fh);
    sendString(str, UART_2_INST);  
}

/**
 * @brief 电路故障检测
 * 
 */
 void isError(void)
 {
    //电阻开短路、电容开路、电容阻值改变    
 }