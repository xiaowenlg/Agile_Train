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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
osThreadId LEDDriveHandle; //LED驱动线程
osThreadId TestStaskHandle;//调试线程
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId StartTaskHandle;

//按键变量和函数
typedef enum
{
	USER_BUTTON_0 = 0,
	USER_BUTTON_1,
	USER_BUTTON_2,
	USER_BUTTON_3,
	USER_BUTTON_MAX
} user_button_t;
static flex_button_t user_button[USER_BUTTON_MAX];//按钮数组
static uint8_t common_btn_read(void *arg);//读按键值
static void common_btn_evt_cb(void *arg);// 按键事件处理函数

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LED_Drive_CallBack(void const* argument);
void Test_Task_CallBack(void const *argument);
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
	osThreadDef(Test_Task,Test_Task_CallBack,3,0,128);
	TestStaskHandle = osThreadCreate(osThread(Test_Task), NULL);
#if DEBUG_PRINT
	Uart_printf(&huart1, "Start sub stask\r\n");
#endif	
	vTaskDelete(StartTaskHandle); //删除任务
	taskEXIT_CRITICAL();//推出临界区
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void LED_Drive_CallBack(void const* argument)
{
	
	for (;;)
	{
		
//		i = rand() % 7 ;      //产生一个30到50的随机数
//#if DEBUG_PRINT
//		Uart_printf(&huart1,"LED Task\r\n");
//#endif	
//		HC595_SendData(1 << i);
		//按键测试
		//Uart_printf(&huart1, "LED Task==%d\r\n",HAL_GPIO_ReadPin(BUTTON_PORT,KEY_2));
		//KeyLoop(keys, Key_CallBack);
		
		//HAL_GPIO_WritePin(GPIOA, LED0,0);
		flex_button_scan();//按键扫描
		
		osDelay(20);

	}
}
void Test_Task_CallBack(void const *argument)
{
	uint8_t p = 0;
	for (;;)
	{
		p = rand() % 7;   
		HC595_SendData(1 << p);
		Led_App(1 << p);
		osDelay(200);
	}
}
//按键注册
static uint8_t common_btn_read(void *arg)
{
	flex_button_t *btn = (flex_button_t *)arg; //类型强制转换
	uint8_t value = 0;
	switch (btn->id)
	{
	case USER_BUTTON_0:
		value = HAL_GPIO_ReadPin(BUTTON_PORT_A, KEY_0); break;   //按键id=3
	case USER_BUTTON_1:
		value = HAL_GPIO_ReadPin(BUTTON_PORT, KEY_4); break;//按键id=2
	case USER_BUTTON_2:
		value = HAL_GPIO_ReadPin(BUTTON_PORT, KEY_3); break;//按键id=1
	case USER_BUTTON_3:
		value = HAL_GPIO_ReadPin(BUTTON_PORT, KEY_2); break;//按键id=0
	default:
		break;
	}
}
 void button_init(void)
{
	memset(&user_button[0], 0x0, sizeof(user_button));//清空结构体变量数组
	for (uint8_t i = 0; i < USER_BUTTON_MAX; i++)
	{
		user_button[i].id = i;
		user_button[i].usr_button_read = common_btn_read;
		user_button[i].cb = common_btn_evt_cb;
		user_button[i].pressed_logic_level = 0;
		user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(1500);
		user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(3000);
		user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(4500);

		flex_button_register(&user_button[i]);
	}
}
static void common_btn_evt_cb(void *arg)
{
	flex_button_t *btn = (flex_button_t *)arg;
	//Uart_printf(&huart1, "FLEX_BTN_PRESS_CLICK id=%d\r\n", btn->id);
	
	if (flex_button_event_read(&user_button[USER_BUTTON_3]) == FLEX_BTN_PRESS_CLICK)
	{
		Uart_printf(&huart1, "FLEX_BTN_PRESS_CLICK id=%d\r\n", btn->id);
	}
	if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_LONG_START)
	{
		Uart_printf(&huart1, "FLEX_BTN_PRESS_LONG_START id=%d\r\n", btn->id);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
