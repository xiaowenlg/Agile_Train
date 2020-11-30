#include "74HC595.h"
#include "dwt_stm32_delay.h"
void HC595_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, HC595_DS_Pin | HC595_RCK_Pin | HC595_SCK_Pin, GPIO_PIN_SET);

	/*Configure GPIO pins : PAPin PAPin PAPin */
	GPIO_InitStruct.Pin = HC595_DS_Pin | HC595_RCK_Pin | HC595_SCK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void HC595_Write_Byte(uint8_t onebyte)
{
	uint8_t i;
	for (i = 0; i < 8;i++)
	{
		if (onebyte & 0x80)
			HAL_GPIO_WritePin(HC595_DS_GPIO_Port, HC595_DS_Pin, 1);
		else
			HAL_GPIO_WritePin(HC595_DS_GPIO_Port, HC595_DS_Pin, 0);

		//产生一个上升沿
		HAL_GPIO_WritePin(HC595_SCK_GPIO_Port, HC595_SCK_Pin, 0);
		DWT_Delay_us(10); //延时 10us 
		HAL_GPIO_WritePin(HC595_SCK_GPIO_Port, HC595_SCK_Pin,1);
		DWT_Delay_us(10); //延时 10us 
		onebyte <<= 1;      //左移一位

	}
}
void HC595_CS(void)
{
	HAL_GPIO_WritePin(HC595_RCK_GPIO_Port, HC595_RCK_Pin, 0);
	DWT_Delay_us(10); //延时 10us 
	HAL_GPIO_WritePin(HC595_RCK_GPIO_Port, HC595_RCK_Pin, 1);
	DWT_Delay_us(10); //延时 10us 
}

void HC_595_Send__Multi_Byte(uint8_t *data, uint16_t len)
{
	uint16_t i;
	for (i = 0; i < len;i++)
	{
		HC595_Write_Byte(data[i]);
	}
	HC595_CS();
}
void HC595_SendData(uint8_t onbyte)
{
	HC595_Write_Byte(onbyte);
	HC595_CS();
}