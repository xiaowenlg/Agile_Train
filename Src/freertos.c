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
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
osThreadId LEDDriveHandle; //LED驱动线程
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
Key_Message keys[4] = { 0 };//按键数组
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId StartTaskHandle;


void  Key_CallBack(Key_Message index);//按键回调函数
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void LED_Drive_CallBack(void const* argument);
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
	//按键处理线程
	/*osThreadDef(ButtonProcess, ButtonProcess_CallBack, 5, 0, 128);
	ButtonProcessHandle = osThreadCreate(osThread(ButtonProcess), NULL);*/
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
	uint8_t i = 0;
	for (;;)
	{
		
//		i = rand() % 7 ;      //产生一个30到50的随机数
//#if DEBUG_PRINT
//		Uart_printf(&huart1,"LED Task\r\n");
//#endif	
//		HC595_SendData(1 << i);
		//按键测试
		//Uart_printf(&huart1, "LED Task==%d\r\n",HAL_GPIO_ReadPin(BUTTON_PORT,KEY_2));
		KeyLoop(keys, Key_CallBack);
		osDelay(20);

	}
}

//按键注册
void Key_Regist(void)
{
	//k2
	keys[0].GPIOx = BUTTON_PORT;
	keys[0].GPIO_Pin = KEY_2;
	keys[0].keyvalue = 0x00fe;
	keys[0].Key_count = 4;

	//k3
	keys[1].GPIOx = BUTTON_PORT;
	keys[1].GPIO_Pin = KEY_3;
	keys[1].keyvalue = 0x00fd;
	keys[1].Key_count = 4;

	//k4
	keys[2].GPIOx = BUTTON_PORT;
	keys[2].GPIO_Pin = KEY_4;
	keys[2].keyvalue = 0x00fb;
	keys[2].Key_count = 4;
	//pa0
	keys[3].GPIOx = GPIOA;
	keys[3].GPIO_Pin = GPIO_PIN_0;
	keys[3].keyvalue = 0x00f7;
	keys[3].Key_count = 4;
}
void  Key_CallBack(Key_Message index)
{
	static uint8_t i = 0;
	if (index.GPIO_Pin==KEY_3)
	{
		//Uart_printf(&huart1, "LED Task=====+++++++++++++%d\r\n",index.keyvalue);
		if (index.keyvalue&KEY_UP)
		{
			i++;
			Uart_printf(&huart1, "LED Task%d\r\n",i);
			
		}
		
	}
	if (index.GPIO_Pin == GPIO_PIN_0)
	{
		//Uart_printf(&huart1, "LED Task=====+++++++++++++%d\r\n",index.keyvalue);
		if (index.keyvalue&KEY_DOWN)
		{
			i++;
			Uart_printf(&huart1, "KEY_DOWN\r\n");

		}
		if (index.keyvalue&KEY_LONG)
		{
			i++;
			Uart_printf(&huart1, "KEY_LONG\r\n");

		}
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
