#ifndef __74HC595_H_
#define __74HC595_H_
#include "system.h"
#include "stdint.h"
#include "stm32f1xx_hal.h"
//74HC595����
#define HC595_DS_Pin GPIO_PIN_5
#define HC595_DS_GPIO_Port GPIOA
#define HC595_RCK_Pin GPIO_PIN_6
#define HC595_RCK_GPIO_Port GPIOA
#define HC595_SCK_Pin GPIO_PIN_7
#define HC595_SCK_GPIO_Port GPIOA

//��ʼ��
void HC595_Init(void);
void HC595_Write_Byte(uint8_t onebyte);//���һ���ֽ�
void HC595_CS(void);		//����д��
void HC_595_Send__Multi_Byte(uint8_t *data, uint16_t len);//����ֽ�����
void HC595_SendData(uint8_t onbyte);//��������
#endif // !__74HC595_H_
