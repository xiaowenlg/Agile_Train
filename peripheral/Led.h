#ifndef __LED_H
#define __LED_H
#include "stdint.h"
#include "stm32f1xx_hal.h"

//板载LED引脚
#define LED0		GPIO_PIN_15
#define LED0_PORT	GPIOA
#define LED1		GPIO_PIN_3
#define LED2		GPIO_PIN_4
#define LED3		GPIO_PIN_5
#define LED4		GPIO_PIN_6
#define LED5		GPIO_PIN_7
#define LED1TO5		GPIOB

typedef struct Led_Struct
{
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
}led;
void LED_Init(void);
void Led_Reg(void);
void Led_Open(uint8_t dat, led *ledarr);
void Led_App(uint8_t data);//实验应用
#endif