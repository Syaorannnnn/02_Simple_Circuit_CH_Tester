

#include "UART.h"


RingBuff_t ringBuff;	//创建一个ringBuff的缓冲区

void sendData(uint8_t data, UART_Regs *huart){
    while(DL_UART_isBusy(huart));
    DL_UART_Main_transmitData(huart, data);
}

void sendString(char *p, UART_Regs *huart) {
    while(*p != '\0') {
        sendData(*p, huart);
        p++;
    }
}

/**
@brief: 串口发送数字
@param: num - 要发送的数字
@param: decimalPlaces - 小数位数
*/
void sendNum(float num, int decimalPlaces, UART_Regs *huart) {
    char str[20];
    char format[10];
    sprintf(format, "%%.%df", decimalPlaces);
    sprintf(str, format, num);
    sendString(str,huart);
}

/**
 * @brief 初始化环形缓冲区
 * 
 */
void initRingBuff(void)
{
  //初始化相关信息
  ringBuff.Head = 0;
  ringBuff.Tail = 0;
  ringBuff.Length = 0;
}

/**
 * @brief 向环形缓冲区写入数据
 * 
 * @param data 
 */
void writeRingBuff(uint8_t data)
{
  if(ringBuff.Length >= RINGBUFF_LEN) //判断缓冲区是否已满
  {
    return ;
  }
  ringBuff.Ring_data[ringBuff.Tail]=data;
  ringBuff.Tail = (ringBuff.Tail+1)%RINGBUFF_LEN;//防止越界非法访问
  ringBuff.Length++;

}

/**
 * @brief 删除环形缓冲区里size长度的数据
 * 
 * @param size 
 */
void deleteRingBuff(uint16_t size)
{
	if(size >= ringBuff.Length)
	{
	    initRingBuff();
	    return;
	}
	for(int i = 0; i < size; i++)
	{

		if(ringBuff.Length == 0)//判断非空
		{
		initRingBuff();
		return;
		}
		ringBuff.Head = (ringBuff.Head + 1) % RINGBUFF_LEN;//防止越界非法访问
		ringBuff.Length--;

	}

}

/**
 * @brief 从环形缓冲区内position位置读取1字节
 * 
 * @param position 
 * @return uint8_t 
 */
uint8_t read1BFromRingBuff(uint16_t position)
{
	uint16_t realPosition = (ringBuff.Head + position) % RINGBUFF_LEN;

	return ringBuff.Ring_data[realPosition];
}

/**
 * @brief 获取环形缓冲区数据长度
 * 
 * @return uint16_t 
 */
uint16_t getRingBuffLength()
{
	return ringBuff.Length;
}

/**
 * @brief 判断环形缓冲区是否已满
 * 
 * @return uint8_t 
 */
uint8_t isRingBuffOverflow()
{
	return ringBuff.Length == RINGBUFF_LEN;
}