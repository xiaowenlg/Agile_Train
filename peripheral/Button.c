/*****************按键程序**********************
*试用方法
*1:初始化按键引脚
*2:声明一个按键结构体数组Key_Message
*3:初始化按键结构体数组
*3:声明一个按键处理回调函数void  Key_CallBack(Key_Message index)
*4:在回调函数中根据按键返回值判断按键状态，并处理
*5:可参考下面例子
*6:按键返回值的高8位代表按键的状态
			#define	KEY_DOWN               	0x8000     //按下
			#define	KEY_LONG                0x4000	   //长按
			#define	KEY_CONTINUE			0x2000	   //连续
			#define	KEY_UP     				0x1000	   //抬起
*/
#include "Button.h"
#include "usart.h"
void Button_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = KEY_2 | KEY_3 | KEY_4;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

Key_Message KeyScan(Key_Message *keys)
{
	static	Key_Message key_null = { 0 }; //按键结构体
	for (uint8_t i = 0; i < keys->Key_count;i++)
	{
		GPIO_PinState keyState = HAL_GPIO_ReadPin(keys[i].GPIOx, keys[i].GPIO_Pin);//读按键引脚值
		if (keyState==0)
		{
			key_null = keys[i];//保存按键  
			return keys[i];// 返回键值
			
		}
		
	}
	
	key_null.keyvalue = 0xff;    //返回按键抬起后按键值
	return key_null;
}
void KeyLoop(Key_Message *keys, fun callback)
{

	static uint8_t s_u8KeyState = KEY_STATE_INIT;
	static uint8_t s_u8KeyTimeCount = 0;
	static uint16_t s_u8LastKey = KEY_NULL;   //保存按键释放时候的键值
	Key_Message KeyTemp = { 0 };//按键结构体

	KeyTemp = KeyScan(keys);         //获取键值
	switch (s_u8KeyState)
	{
	case KEY_STATE_INIT:
	{
		if (KEY_NULL != (KeyTemp.keyvalue))
		{
			s_u8KeyState = KEY_STATE_WOBBLE;        //抖动状态
		}
	}
		break;
	case KEY_STATE_WOBBLE:       //消抖
	{
		 s_u8KeyState = KEY_STATE_PRESS;
	}
		break;
	case KEY_STATE_PRESS:
	{
		if (KEY_NULL != (KeyTemp.keyvalue))
		{
			s_u8LastKey = KeyTemp.keyvalue; //保存键值,以便在释放按键状态返回键值
			KeyTemp.keyvalue |= KEY_DOWN;   //按键按下
			s_u8KeyState = KEY_STATE_LONG;   
		}
		else
		{
			s_u8KeyState = KEY_STATE_INIT;
		}
	}
		break;
	case KEY_STATE_LONG:
	{
		if (KEY_NULL != (KeyTemp.keyvalue))
		{
			if (++s_u8KeyTimeCount > KEY_LONG_PERIOD)
			{
				s_u8KeyTimeCount = 0;
				KeyTemp.keyvalue |= KEY_LONG;   //长按键事件发生
				s_u8KeyState = KEY_STATE_CONTINUE;
			}
		}
		else
		{
			s_u8KeyState = KEY_STATE_RELEASE;
		}
	}
		break;
	case KEY_STATE_CONTINUE:
	{
							   if (KEY_NULL != (KeyTemp.keyvalue))
							   {
								   if (++s_u8KeyTimeCount > KEY_CONTINUE_PERIOD)
								   {
									   s_u8KeyTimeCount = 0;
									   KeyTemp.keyvalue |= KEY_CONTINUE;

								   }
							   }
							   else
							   {
								   s_u8KeyState = KEY_STATE_RELEASE;
							   }
	}
		break;
	case KEY_STATE_RELEASE:
	{
							 
							  s_u8LastKey |= KEY_UP;
							  KeyTemp.keyvalue = s_u8LastKey;
							//  Uart_printf(&huart1, "LED Task=====+++++++++++++%d=======%d\r\n", KeyTemp.keyvalue,KeyTemp.GPIO_Pin);
							  s_u8KeyState = KEY_STATE_INIT;
	}
		break;
	default: break;		
	}
	(*callback)(KeyTemp);        //调用回调函数
}
