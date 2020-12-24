#ifndef _TFT_DRIVER_H
#define _TFT_DRIVER_H
#include "stm32f1xx_hal.h"
#include "stdint.h"
/*TFT操作函数*/
#define array_length          100  //定义数组长度
#define USER_R3               0xA5  //帧头首字节
#define USER_RA               0x5A  //帧头次字节
#define UART_TFT			  &huart2			//TFT屏(武汉晶显)
void write_register_80_1byte(uint8_t address, uint8_t data);
void write_multiple_register_80(uint8_t address, uint8_t data_length, uint8_t *data);
void write_variable_store_82_1word(uint16_t address, uint16_t data);
void write_multiple_variable_store_82(uint16_t address, uint8_t data_length, uint16_t *data);
void TFT_playsound(uint8_t data_length, uint8_t *data, UART_HandleTypeDef *huart);//TFT播放语音;
void playmusic(uint16_t num, uint8_t val);
void stopmusic(uint16_t num);
void Turen_Pic(uint16_t num);
void TFT_Beep(uint8_t n);//TFT屏蜂鸣器;
void TFT_Readbytes(uint8_t adress, uint8_t readlen, UART_HandleTypeDef *huart);//读寄存器;
#endif // !_TFT_DRIVER_H
