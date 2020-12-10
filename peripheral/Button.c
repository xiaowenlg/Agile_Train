/*****************��������**********************
*���÷���
*1:��ʼ����������
*2:����һ�������ṹ������Key_Message
*3:��ʼ�������ṹ������
*3:����һ����������ص�����void  Key_CallBack(Key_Message index)
*4:�ڻص������и��ݰ�������ֵ�жϰ���״̬��������
*5:�ɲο���������
*6:��������ֵ�ĸ�8λ��������״̬
			#define	KEY_DOWN               	0x8000     //����
			#define	KEY_LONG                0x4000	   //����
			#define	KEY_CONTINUE			0x2000	   //����
			#define	KEY_UP     				0x1000	   //̧��
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
	static	Key_Message key_null = { 0 }; //�����ṹ��
	for (uint8_t i = 0; i < keys->Key_count;i++)
	{
		GPIO_PinState keyState = HAL_GPIO_ReadPin(keys[i].GPIOx, keys[i].GPIO_Pin);//����������ֵ
		if (keyState==0)
		{
			key_null = keys[i];//���水��  
			return keys[i];// ���ؼ�ֵ
			
		}
		
	}
	
	key_null.keyvalue = 0xff;    //���ذ���̧��󰴼�ֵ
	return key_null;
}
void KeyLoop(Key_Message *keys, fun callback)
{

	static uint8_t s_u8KeyState = KEY_STATE_INIT;
	static uint8_t s_u8KeyTimeCount = 0;
	static uint16_t s_u8LastKey = KEY_NULL;   //���水���ͷ�ʱ��ļ�ֵ
	Key_Message KeyTemp = { 0 };//�����ṹ��

	KeyTemp = KeyScan(keys);         //��ȡ��ֵ
	switch (s_u8KeyState)
	{
	case KEY_STATE_INIT:
	{
		if (KEY_NULL != (KeyTemp.keyvalue))
		{
			s_u8KeyState = KEY_STATE_WOBBLE;        //����״̬
		}
	}
		break;
	case KEY_STATE_WOBBLE:       //����
	{
		 s_u8KeyState = KEY_STATE_PRESS;
	}
		break;
	case KEY_STATE_PRESS:
	{
		if (KEY_NULL != (KeyTemp.keyvalue))
		{
			s_u8LastKey = KeyTemp.keyvalue; //�����ֵ,�Ա����ͷŰ���״̬���ؼ�ֵ
			KeyTemp.keyvalue |= KEY_DOWN;   //��������
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
				KeyTemp.keyvalue |= KEY_LONG;   //�������¼�����
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
	(*callback)(KeyTemp);        //���ûص�����
}
