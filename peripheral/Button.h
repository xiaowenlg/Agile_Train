#ifndef __BUTTON_H_
#define __BUTTON_H_
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "system.h"
//���尴������ֵ״̬(����,����,��_��,�ͷ�)
#define	KEY_DOWN               	0x8000     //����
#define	KEY_LONG                0x4000	   //����
#define	KEY_CONTINUE			0x2000	   //����
#define	KEY_UP     				0x1000	   //̧��
#define KEY_NULL				0x00ff	   //�ް�������
//���峤������TICK��, �Լ���_�������TICK��
#define	 KEY_LONG_PERIOD         100                         //100��ɨ������
#define	 KEY_CONTINUE_PERIOD     1
//���尴��״̬

#define BUTTON_PORT				GPIOE
#define KEY_2					GPIO_PIN_2
#define KEY_3					GPIO_PIN_3
#define KEY_4					GPIO_PIN_4
#define BUTTON_PORT_A			GPIOA
#define KEY_0					GPIO_PIN_0
enum my_button_event
{
	KEY_STATE_INIT,   //0
	KEY_STATE_WOBBLE,         			//����
	KEY_STATE_PRESS,            				//����
	KEY_STATE_LONG,            		
	KEY_STATE_CONTINUE,       		
	KEY_STATE_RELEASE         		
};
//�����ṹ��
typedef struct K_Info
{
	GPIO_TypeDef *GPIOx; //�����˿�
	uint16_t GPIO_Pin;   //��������
	uint8_t Key_count;  //ע�ᰴ������
	uint16_t keyvalue;
}Key_Message;
typedef void(*fun)(Key_Message keynum); //�ص�������ʽ
void Button_GPIO_Init();   //�����ӿڶ���
void KeyLoop(Key_Message *keys, fun callback);//����ɨ��
#endif // !__BUTTON_H
