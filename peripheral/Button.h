#ifndef __BUTTON_H_
#define __BUTTON_H_
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "system.h"
//定义按键返回值状态(按下,长按,连_发,释放)
#define	KEY_DOWN               	0x8000     //按下
#define	KEY_LONG                0x4000	   //长按
#define	KEY_CONTINUE			0x2000	   //连续
#define	KEY_UP     				0x1000	   //抬起
#define KEY_NULL				0x00ff	   //无按键按下
//定义长按键的TICK数, 以及连_发间隔的TICK数
#define	 KEY_LONG_PERIOD         100                         //100个扫描周期
#define	 KEY_CONTINUE_PERIOD     1
//定义按键状态

#define BUTTON_PORT				GPIOE
#define KEY_2					GPIO_PIN_2
#define KEY_3					GPIO_PIN_3
#define KEY_4					GPIO_PIN_4
#define BUTTON_PORT_A			GPIOA
#define KEY_0					GPIO_PIN_0
enum my_button_event
{
	KEY_STATE_INIT,   //0
	KEY_STATE_WOBBLE,         			//抖动
	KEY_STATE_PRESS,            				//按下
	KEY_STATE_LONG,            		
	KEY_STATE_CONTINUE,       		
	KEY_STATE_RELEASE         		
};
//按键结构体
typedef struct K_Info
{
	GPIO_TypeDef *GPIOx; //按键端口
	uint16_t GPIO_Pin;   //按键引脚
	uint8_t Key_count;  //注册按键数量
	uint16_t keyvalue;
}Key_Message;
typedef void(*fun)(Key_Message keynum); //回调函数形式
void Button_GPIO_Init();   //按键接口定义
void KeyLoop(Key_Message *keys, fun callback);//按键扫描
#endif // !__BUTTON_H
