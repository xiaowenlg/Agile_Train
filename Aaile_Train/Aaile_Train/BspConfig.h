#ifndef __BSPCONFIG_H
#define __BSPCONFIG_H
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "system.h"

//���贮�ڶ���
//�豸���ڲ���������
#define BLE_BAND				115200        //��������
#define DEBUG_BAND				115200        //�����ô���
#define HMI_BAND				115200		//HML������(�Ծ���)

//���ڷ���

#define  UART_BLE				&huart1			//����
#define  UART_ULTRASONIC		&huart2			//�������ӿ�	


//��ѹ�������
#define V_TEST_Pin GPIO_PIN_4
#define V_TEST_GPIO_Port GPIOA




//оƬflash�ռ䶨��
#define CHIP_CAPACITY				128			//оƬflash������λKb
#define CHIP_START_ADDRESS			0x08000000

////������ַ
//#define HUAN_YING					43							//��ӭʹ�����������Ŀ
//#define BEN_XIANG_MU					26                        //����ĿΪ������
//#define CESHI						27						  //����
//#define TI_ZHI_ZHI_SHU				28						//����ָ��
//#define QING_ZHAN_LI				37						//����վ���ڲ���λ�����־�ֹ�����������Ӳ��Խ��׼ȷ��
//#define YIN_XIAO_61					61						//��Ч61
//#define QING_AN_KAISHI				41						//�밴��ʼ����ʼ
//#define CESHI_KAISHI				44						//���Կ�ʼ
//#define CESHI_JIESHU				45						//���Խ���
//#define NINDE						33						//����
//#define SHEN_GAO					35						//���
//#define TI_ZHONG					36						//����
//#define SOUND_BIM					28						//����ָ��
//#define GONG_JIN					16						//����
//#define GONG_FEN					17						//����
//#define	TI_XING						49						//����
//#define PIAN_PANG					50						//ƫ��
//#define PIAN_SHOU					51						//ƫ��
//#define ZHENG_CHANG					52						//����
//#define HUAN_ZAI_CI_YING			57						//��ӭ�ٴ�ʹ��
//#define WO_LI						29						//����
//#define QING_YONG_LI_WO				38						//��������ס������


#define  TIM_LONG				60							//ÿ�ֵ�ʱ����λs
#define  TIM_PERIOD				1000						//ʱ��������1000ms
//��Ļ��ֵ
#define	TFT_ADRESS_TIM_BACK		0x0001						//����ʱ��ַ
#define TFT_ADRESS_SCOERE		0x0002						//���д���
#define TFT_ADRESS_COUNT		0x0004						//�ܵ������
#define TFT_ADRESS_BT			0x0003						//��ص���
#define TFT_ADRESS_DISHU		0x0005						//�������ͼ��
#define TFT_BUTTON				0x4F						//TFT���ϵİ�ť
#define TFT_ADRESS_LAST_SCORE	0x0006						//�ɼ���ַ
#define TFT_ADRESS_SET_LEVEL	0x0007						//�ȼ�����
#define TFT_PAGE_SUCCESS		2							//ʤ��ҳ��
#define TFT_PAGE_FAIL			3							//ʧ��ҳ��
#define TFT_PAGE_SET			4							//����ҳ��
#define TFT_PAGE_DAT			1							//����ҳ��
#define TFT_PAGE_DEBUG			5							//����ҳ��
#define TFT_ADRESS_DEBUG_COUNT	0x0008						//����ҳ������
//��Ƶ��ַ
#define TFT_MUSIC_ADRESS_SCORE	52							//ʤ������
#define TFT_MUSIC_ADRESS_FAIL	50							//ʧ������
#define TFT_MUSIC_VALUE			0x01							//��������
//���Կ���
#define DEBUG_PRINT					1                                
#endif // !__BSPCONFIG_H

