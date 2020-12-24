/*灯的按键接口*/
#ifndef __KEY_H
#define __KEY_H
#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "flexible_button.h"
#define K0_3_PORT	GPIOB
#define K0_PIN		GPIO_PIN_11
#define K1_PIN		GPIO_PIN_10
#define K2_PIN		GPIO_PIN_1
#define K3_PIN		GPIO_PIN_0
#define K4_6_PORT	GPIOA
#define K4_PIN		GPIO_PIN_7
#define K5_PIN		GPIO_PIN_6
#define K6_PIN		GPIO_PIN_5
//按键ID
typedef enum
{
	USER_BUTTON_0 = 0,
	USER_BUTTON_1,
	USER_BUTTON_2,
	USER_BUTTON_3,
	USER_BUTTON_4,
	USER_BUTTON_5,
	USER_BUTTON_6,
	USER_BUTTON_MAX
} user_button_t;
void Key_GPIO_Init();//按键引脚初始化

#endif // !__KEY_H
