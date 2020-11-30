#ifndef __74HC595_H_
#define __74HC595_H_
#include "system.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"
//74HC595引脚
#define HC595_DS_Pin GPIO_PIN_5
#define HC595_DS_GPIO_Port GPIOA
#define HC595_RCK_Pin GPIO_PIN_6
#define HC595_RCK_GPIO_Port GPIOA
#define HC595_SCK_Pin GPIO_PIN_7
#define HC595_SCK_GPIO_Port GPIOA

//初始化
void HC595_Init(void);
void HC595_Write_Byte(uint8_t onebyte);//输出一个字节
void HC595_CS(void);		//数据写出
void HC_595_Send__Multi_Byte(uint8_t *data, uint16_t len);//输出字节数组
void HC595_SendData(uint8_t onbyte);//发送数据
#endif // !__74HC595_H_
