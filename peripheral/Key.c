#include "Key.h"
#include "Button.h"
static flex_button_t user_button[USER_BUTTON_MAX];//��ť����
void Key_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//����0--3
	GPIO_InitStruct.Pin = K0_PIN | K1_PIN | K2_PIN | K3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(K0_3_PORT, &GPIO_InitStruct);
	//����4--6
	GPIO_InitStruct.Pin = K4_PIN | K5_PIN | K6_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(K4_6_PORT, &GPIO_InitStruct);
}

