#include "Led.h"
led led_arr[6] = { 0 };
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	HAL_GPIO_WritePin(LED0_PORT, LED0, 0);
	HAL_GPIO_WritePin(LED1TO5, LED1 | LED2 | LED3 | LED4 | LED5,0);

	GPIO_InitStruct.Pin = LED0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LED0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED1 | LED2 | LED3 | LED4 | LED5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(LED1TO5, &GPIO_InitStruct);

}
void Led_Reg(void)
{
	led_arr[0].GPIOx = LED0_PORT;
	led_arr[0].GPIO_Pin = LED0;

	led_arr[1].GPIOx = LED1TO5;
	led_arr[1].GPIO_Pin = LED1;
	led_arr[2].GPIOx = LED1TO5;
	led_arr[2].GPIO_Pin = LED2;
	led_arr[3].GPIOx = LED1TO5;
	led_arr[3].GPIO_Pin = LED3;
	led_arr[4].GPIOx = LED1TO5;
	led_arr[4].GPIO_Pin = LED4;
	led_arr[5].GPIOx = LED1TO5;
	led_arr[5].GPIO_Pin = LED5;

}
void Led_Open(uint8_t dat,led *ledarr)
{
	uint8_t i = 0;
	for (i = 0; i < 6;i++)
	{
		if (dat&0x01)  //判断比位是0还是1
		
			HAL_GPIO_WritePin(ledarr[i].GPIOx, ledarr[i].GPIO_Pin, 1);//是1亮
		else
			HAL_GPIO_WritePin(ledarr[i].GPIOx, ledarr[i].GPIO_Pin, 0);//是0灭
		dat >>= 1;
	}
}
void Led_App(uint8_t data)
{
	Led_Open(data, led_arr);
}
void Led_Open_Control(uint8_t mode)
{

}