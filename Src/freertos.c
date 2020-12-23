/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "BspConfig.h"
#include "74HC595.h"
#include "time.h"
#include "Button.h"
#include "flexible_button.h"
#include <string.h>
#include <stdlib.h>
#include "Led.h"
#include "Key.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
osThreadId StartTaskHandle; //系统启动线程
osThreadId LEDDriveHandle; //LED驱动线程
osThreadId ButtonScanStaskHandle;//调试线程
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/***************tim.c中的全局变量*******************/
uint16_t Game_Tim_Long = 0;//游戏时长
_Bool Notice_flg = 0;
uint16_t Led_period;//亮灯周期
/***************************************************/
uint8_t Move_Index = 0;//亮灯数据位移
uint16_t User_score = 0;//得分
uint16_t GradeArr[4] = {300,600,1000,2000};
/* USER CODE END Variables */


//按键变量和函数

static flex_button_t user_button[USER_BUTTON_MAX];//按钮数组
static uint8_t common_btn_read(void *arg);//读按键值
static void common_btn_evt_cb(void *arg);// 按键事件处理函数

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LED_Drive_CallBack(void const* argument);
void ButtonScan_Task_CallBack(void const *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of StartTask */
  Game_Tim_Long = TIM_LONG;
  Led_period = GradeArr[2];
  osThreadDef(StartTask, StartDefaultTask, osPriorityNormal, 0, 128);
  StartTaskHandle = osThreadCreate(osThread(StartTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the StartTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	taskENTER_CRITICAL();//进入临界区
	
	//传感器驱动线程
	osThreadDef(LED_Drive, LED_Drive_CallBack, 4, 0, 128);
	LEDDriveHandle = osThreadCreate(osThread(LED_Drive), NULL);
	//调试线程
	osThreadDef(ButtonScan_Task, ButtonScan_Task_CallBack, 3, 0, 128);
	ButtonScanStaskHandle = osThreadCreate(osThread(ButtonScan_Task), NULL);
#if DEBUG_PRINT
	Uart_printf(&huart1, "Start sub stask\r\n");
#endif	
	vTaskDelete(StartTaskHandle); //删除任务
	taskEXIT_CRITICAL();//推出临界区
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void LED_Drive_CallBack(void const* argument)  //LED驱动线程
{
	
	for (;;)
	{
		if (Game_Tim_Long > 0)//
		{
			if (Notice_flg)
			{
				Move_Index = rand() % 7;
				HC595_SendData(1 << Move_Index);
				Led_App(1 << Move_Index);
				Uart_printf(&huart1, "LED_Value=%d,period=%d\r\n", Move_Index, Game_Tim_Long);
				Notice_flg = 0;
			}
		}
		else
		{
			//超时后的处理（游戏结束）
		}

		osDelay(200);
	}
}
void ButtonScan_Task_CallBack(void const *argument)  //按键扫描线程
{
	
	for (;;)
	{
		flex_button_scan();//按键扫描
		osDelay(20);
	}
}
//按键注册
static uint8_t common_btn_read(void *arg) //读按键函数
{
	flex_button_t *btn = (flex_button_t *)arg; //类型强制转换
	uint8_t value = 0;
	switch (btn->id)
	{
	case USER_BUTTON_0:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K0_PIN); break;   //按键id=0
	case USER_BUTTON_1:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K1_PIN); break;//按键id=1
	case USER_BUTTON_2:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K2_PIN); break;//按键id=2
	case USER_BUTTON_3:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K3_PIN); break;//按键id=3
	case USER_BUTTON_4:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K4_PIN); break;//按键id=4
	case USER_BUTTON_5:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K5_PIN); break;//按键id=5
	case USER_BUTTON_6:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K6_PIN); break;//按键id=6
	default:
		break;
	}
	return value;
}
 void button_init(void)//按键初始
{
	memset(&user_button[0], 0x0, sizeof(user_button));//清空结构体变量数组
	for (uint8_t i = 0; i < USER_BUTTON_MAX; i++)
	{
		user_button[i].id = i;
		user_button[i].usr_button_read = common_btn_read;
		user_button[i].cb = common_btn_evt_cb;
		user_button[i].pressed_logic_level = 0;
		user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
		user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(4000);
		user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

		flex_button_register(&user_button[i]);
	}
}
static void common_btn_evt_cb(void *arg)//按键事件回调函数
{
	flex_button_t *btn = (flex_button_t *)arg;
	uint8_t value = 0,key_id = 50;
	
	//Uart_printf(&huart1, "Button id=%d\r\n", btn->id);
	
	if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK)
	{
		key_id = btn->id; //传出被按下的按键id
		Uart_printf(&huart1, "Button id=%d\r\n", btn->id);
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_2]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_3]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_4]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_5]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_6]) == FLEX_BTN_PRESS_CLICK)
	 {
		 key_id = btn->id;
	 }
	
	
	 if (key_id == Move_Index)
	{
		value = Move_Index&(~Move_Index);
		HC595_SendData(value);
		Uart_printf(&huart1, "FLEX_BTN_PRESS_CLICK id=%d,The Score = %d\r\n", key_id,User_score++);
	}
	//按键控制
	 if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_LONG_START)
	 {
		 Uart_printf(&huart1, "FLEX_BTN_PRESS_LONG_START \r\n");
		 Game_Tim_Long = TIM_LONG;//重置时间
		 User_score = 0;//清空分数

	 }
	
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
