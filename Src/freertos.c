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
#include "TFT_Driver.h"
#include "tim.h"
#include "adc.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
osThreadId StartTaskHandle; //ϵͳ�����߳�
osThreadId LEDDriveHandle; //LED�����߳�
osThreadId ButtonScanStaskHandle;//�����߳�
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
/***************tim.c�е�ȫ�ֱ���*******************/
int Game_Tim_Long = 0;//��Ϸʱ��
_Bool Notice_flg = 0;
uint16_t Led_period;//��������
int debug_count = 0;//����led����
/***************************************************/
uint8_t Move_Index = 0;//��������λ��
uint16_t User_score = 0;//�÷�
uint16_t GradeArr[5] = {5000,2000,1000,600,300};//��Ϸ�ȼ�300,600,1000,2000,5000
uint16_t Press_Count = 0;//���µ��ܴ���(�������ԺͰ����)
_Bool GameOver_flg = 0;//��Ϸ������־λ 1���� 0��ʼ
uint8_t Press_User_num = 0;//���û�����ʱֻ��һ�μƷ֣���ֹ��ΰ��¼Ʒ�
uint8_t Last_score = 0;//���ճɼ�
_Bool set_flg = 0;//���ð�����־λ
uint8_t Current_page_ID = 1;//��ǰҳ��id
uint8_t game_level = 0;//��Ϸ�ȼ� ȡֵ��Χ0---4
uint8_t tft_count = 0;//��tft�������ݵĴ���
_Bool debug_run_flg = 0;//0�������У�1����led
//�߳��ڲ�����
void Run_Task(void);//�������к���
void Debug_Task(void);//���Ժ���
/* USER CODE END Variables */

//��Ϸ���ݳ�ʼ
void Game_Data_Init();
//���������ͺ���

static flex_button_t user_button[USER_BUTTON_MAX];//��ť����
static uint8_t common_btn_read(void *arg);//������ֵ
static void common_btn_evt_cb(void *arg);// �����¼�������

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
  Game_Data_Init();
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
	taskENTER_CRITICAL();//�����ٽ���
	//������ܽ����ͣ��5-10s
	//Current_page_ID = Turen_Pic(TFT_PAGE_DAT);
	//����ʱ�����������ݽ���
	//����LED�����߳�
	osThreadDef(LED_Drive, LED_Drive_CallBack, 4, 0, 128);
	LEDDriveHandle = osThreadCreate(osThread(LED_Drive), NULL);
	//��ťɨ���߳�
	osThreadDef(ButtonScan_Task, ButtonScan_Task_CallBack, 3, 0, 128);
	ButtonScanStaskHandle = osThreadCreate(osThread(ButtonScan_Task), NULL);
#if DEBUG_PRINT
	Uart_printf(&huart1, "Start sub stask\r\n");
#endif	
	vTaskDelete(StartTaskHandle); //ɾ������
	taskEXIT_CRITICAL();//�Ƴ��ٽ���
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void LED_Drive_CallBack(void const* argument)  //LED�����߳�
{
	
	for (;;)
	{
		if (debug_run_flg==0)
		{
			Run_Task();
		}
		else
		{
			Debug_Task();
		}
		
		//��TFT����������
		osDelay(200);
	}
}
void ButtonScan_Task_CallBack(void const *argument)  //����ɨ���߳�
{
	
	for (;;)
	{
		flex_button_scan();//����ɨ��
		osDelay(20);
	}
}
//����ע��
static uint8_t common_btn_read(void *arg) //����������
{
	flex_button_t *btn = (flex_button_t *)arg; //����ǿ��ת��
	uint8_t value = 0;
	switch (btn->id)
	{
	case USER_BUTTON_0:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K0_PIN); break;   //����id=0
	case USER_BUTTON_1:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K1_PIN); break;//����id=1
	case USER_BUTTON_2:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K2_PIN); break;//����id=2
	case USER_BUTTON_3:
		value = HAL_GPIO_ReadPin(K0_3_PORT, K3_PIN); break;//����id=3
	case USER_BUTTON_4:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K4_PIN); break;//����id=4
	case USER_BUTTON_5:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K5_PIN); break;//����id=5
	case USER_BUTTON_6:
		value = HAL_GPIO_ReadPin(K4_6_PORT, K6_PIN); break;//����id=6
	default:
		break;
	}
	return value;
}
 void button_init(void)//������ʼ
{
	memset(&user_button[0], 0x0, sizeof(user_button));//��սṹ���������
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
static void common_btn_evt_cb(void *arg)//�����¼��ص�����
{
	flex_button_t *btn = (flex_button_t *)arg;
	uint8_t value = 0,key_id = 50;
	static uint8_t i = 0;
	//Uart_printf(&huart1, "Button id=%d\r\n", btn->id);
	
	if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_CLICK)
	{
		key_id = btn->id; //���������µİ���id
		Press_Count++;   //�������ش���+1
		
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
		 Press_Count++;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_2]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
		 Press_Count++;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_3]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
		 Press_Count++;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_4]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
		 Press_Count++;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_5]) == FLEX_BTN_PRESS_CLICK)
	{
		 key_id = btn->id;
		 Press_Count++;
	}
	 if (flex_button_event_read(&user_button[USER_BUTTON_6]) == FLEX_BTN_PRESS_CLICK)
	 {
		 key_id = btn->id;
		 Press_Count++;
	 }
	
	 
	 if (key_id == Move_Index)//ͨ������id�͵Ƶ�id�ж��Ƿ񰴶ԣ�������رյƣ�User_score��1��¼����
	{
		if (!GameOver_flg)//��Ϸ�������Ʒ�
		{
			if (Press_User_num<=0)//��ֹ�û���ΰ���
			{
				Press_User_num++;
				value = Move_Index&(~Move_Index);
				HC595_SendData(value);
				write_variable_store_82_1word(TFT_ADRESS_DISHU, value);
				playmusic(TFT_MUSIC_60, TFT_MUSIC_VALUE); //���е���Ч
				User_score++;
				//Uart_printf(&huart1, "FLEX_BTN_PRESS_CLICK id=%d,The Score = %d\r\n", key_id, User_score);
			}
			else
			{
				//�ڴ���ʾ�û��ఴ��û��**************************
				if (i++>2)
				{
					i = 0;
					playmusic(TFT_MUSIC_53, TFT_MUSIC_VALUE);
				}
				
			}
			
		
		}
		
	}
	//��������
	 if (flex_button_event_read(&user_button[USER_BUTTON_0]) == FLEX_BTN_PRESS_LONG_START)//���¿�ʼ��ť����
	 {
		// Uart_printf(&huart1, "FLEX_BTN_PRESS_LONG_START \r\n");
		 Current_page_ID = Turen_Pic(TFT_PAGE_DAT);//�������ݽ���
		 //�ڴ˼��������bip.....**********************************
		 Game_Tim_Long = TIM_LONG;//����ʱ�䣬���¿�ʼ��һ��
		 User_score = 0;//��շ���
		 GameOver_flg = 0;//��Ϸ��������
		 Press_Count = 0;//���´�������
		 HAL_TIM_Base_Start_IT(&htim2);
		 
	 }
	 if (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_LONG_START)//���ð�ť����
	 {
		 
		 if (GameOver_flg) //��Ϸ������ſ�����
		 {
			 set_flg = !set_flg;
			 if (set_flg)
			 {
				 Current_page_ID = Turen_Pic(TFT_PAGE_SET);//�������ý���
				 HAL_TIM_Base_Stop_IT(&htim2);
			 }
			 else
			 {
				 Current_page_ID = Turen_Pic(TFT_PAGE_DAT);//�������ݽ���
				 Game_Tim_Long = TIM_LONG;//����ʱ�䣬���¿�ʼ��һ��
				 User_score = 0;//��շ���
				 GameOver_flg = 0;//��Ϸ��������
				 Press_Count = 0;//���´�������
				 HAL_TIM_Base_Start_IT(&htim2);
			 }
		 }
		// Uart_printf(&huart1, "FLEX_BTN_PRESS_LONG_START_BUTTON1=%d \r\n",set_flg);
	 }
	 if (flex_button_event_read(&user_button[USER_BUTTON_1]) == FLEX_BTN_PRESS_CLICK)//���ð�ť����
	 {
		if (set_flg==1&&Current_page_ID==TFT_PAGE_SET)
		{
			
			if (game_level < 4)
			{
				
				game_level++;
			}
			else
				game_level = 0;
			
			Led_period = GradeArr[game_level];
			write_variable_store_82_1word(TFT_ADRESS_SET_LEVEL, game_level+1);
			//Uart_printf(&huart1, "FLEX=%d \r\n", game_level);
			
		}
		
	 }
	 if (flex_button_event_read(&user_button[USER_BUTTON_2]) == FLEX_BTN_PRESS_LONG_START)//���Ժ����������л���ť_����
	 {
		 debug_run_flg = !debug_run_flg;
		 debug_count = 0;
		 if (debug_run_flg)//������Խ���
		 {
			 Turen_Pic(TFT_PAGE_DEBUG);//�������ҳ��
		 }
		 else
		 {
			 Turen_Pic(TFT_PAGE_DAT);//��������ҳ��
			 Game_Tim_Long = TIM_LONG;
			 HAL_TIM_Base_Start_IT(&htim2);
		 }

	 }
	 write_register_80_1byte(TFT_BUTTON, 1);//���ڿ���
	
}
//��Ϸ���ݳ�ʼ
void Game_Data_Init()
{
	debug_run_flg = 0;//��������
	Current_page_ID = Turen_Pic(TFT_PAGE_DAT);//�������ݽ���
	Game_Tim_Long = TIM_LONG;//����ʱ�䣬���¿�ʼ��һ��
	User_score = 0;//��շ���
	GameOver_flg = 0;//��Ϸ��������
	Press_Count = 0;//���´�������
	game_level = 0;//��Ϸ�ȼ����
	Led_period = GradeArr[game_level];
	write_variable_store_82_1word(TFT_ADRESS_SET_LEVEL, game_level+1);
}
//�������к���  //��LED_Drive_CallBack������
void Run_Task(void)
{
	if (Game_Tim_Long >= 0)//�ж��Ƿ񵽽���ʱ��
	{
		tft_count = 0;
		if (Notice_flg)  //��ʱ�����ƵƱ�־λ�ж�
		{
			Move_Index = rand() % 7;		//
			HC595_SendData(1 << Move_Index);
			Led_App(1 << Move_Index);
			write_variable_store_82_1word(TFT_ADRESS_DISHU, Move_Index + 1);//�������
			playmusic(TFT_MUSIC_55, TFT_MUSIC_VALUE);
			write_register_80_1byte(TFT_BUTTON, 1);
			//Uart_printf(&huart1, "LED_Value=%d,period=%d\r\n", Move_Index, Game_Tim_Long);
			Notice_flg = 0;
			Press_User_num = 0;
		}
		write_variable_store_82_1word(TFT_ADRESS_TIM_BACK, Game_Tim_Long);//���͵���ʱ
		write_variable_store_82_1word(TFT_ADRESS_SCOERE, User_score);//���ͻ�����
		write_variable_store_82_1word(TFT_ADRESS_COUNT, Press_Count);
		write_variable_store_82_1word(TFT_ADRESS_BT, ADC_GetValue(&hadc1, 10));			//��ص���)
	}
	else
	{
		//��ʱ��Ĵ�����Ϸ������
		GameOver_flg = 1;//��Ϸ����
		if (tft_count < 2)  //��tft������������
		{
			tft_count++;
			//set_flg = 0;//���Խ������ý�����
			if (Press_Count < (TIM_LONG*1000.00 / Led_period))   //���������С�ڵƵĸ���ʱ �١��������������ܵ����ı�ֵ    Last_score * (��������/�Ƶĸ���)
			{
				Last_score = (User_score * 100 * Led_period) / (TIM_LONG*1000.00);//ȡ���аٷֱ�
				Last_score = Last_score*(User_score / Press_Count); //����۷�
			}
			else
				Last_score = User_score * 100 / Press_Count;//ȡ���аٷֱ�
			if (Last_score >= SUCCESS_SCOERE)
			{
				Current_page_ID = Turen_Pic(TFT_PAGE_SUCCESS);//����ɹ�ҳ��
				SetSountValue(TFT_MUSIC_VALUE);//��������
				playmusic(TFT_MUSIC_ADRESS_SCORE, TFT_MUSIC_VALUE);
				//���ųɹ�����
			}
			else
			{
				Current_page_ID = Turen_Pic(TFT_PAGE_FAIL);//����ʧ��ҳ��
				SetSountValue(TFT_MUSIC_VALUE);//��������
				playmusic(TFT_MUSIC_ADRESS_FAIL, TFT_MUSIC_VALUE);
				//����ʧ������
			}
			write_variable_store_82_1word(TFT_ADRESS_LAST_SCORE, Last_score);
		}
		
	}
}
void Debug_Task(void)//���Ժ���
{
	if (debug_count<7)
	{
		HC595_SendData(1 << debug_count);//led�ƴ�0-6����һ��
		write_variable_store_82_1word(TFT_ADRESS_DEBUG_COUNT, debug_count);//����debue_count����Ļ
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
