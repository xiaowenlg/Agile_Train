#ifndef __BSPCONFIG_H
#define __BSPCONFIG_H
#include "stdint.h"
#include "stm32f1xx_hal.h"
#include "system.h"

//外设串口定义
//设备串口波特率设置
#define BLE_BAND				115200        //蓝牙串口
#define DEBUG_BAND				115200        //调试用串口
#define HMI_BAND				115200		//HML串口屏(淘晶驰)

//串口分配

#define  UART_BLE				&huart1			//蓝牙
#define  UART_ULTRASONIC		&huart2			//超声波接口	


//电压检测引脚
#define V_TEST_Pin GPIO_PIN_4
#define V_TEST_GPIO_Port GPIOA




//芯片flash空间定义
#define CHIP_CAPACITY				128			//芯片flash容量单位Kb
#define CHIP_START_ADDRESS			0x08000000

////语音地址
//#define HUAN_YING					43							//欢迎使用启帆体测项目
//#define BEN_XIANG_MU					26                        //本项目为您进行
//#define CESHI						27						  //测试
//#define TI_ZHI_ZHI_SHU				28						//体质指数
//#define QING_ZHAN_LI				37						//请您站立在测试位并保持静止不动，以增加测试结果准确性
//#define YIN_XIAO_61					61						//音效61
//#define QING_AN_KAISHI				41						//请按开始建开始
//#define CESHI_KAISHI				44						//测试开始
//#define CESHI_JIESHU				45						//测试结束
//#define NINDE						33						//您的
//#define SHEN_GAO					35						//身高
//#define TI_ZHONG					36						//体重
//#define SOUND_BIM					28						//体质指数
//#define GONG_JIN					16						//公斤
//#define GONG_FEN					17						//公分
//#define	TI_XING						49						//体型
//#define PIAN_PANG					50						//偏胖
//#define PIAN_SHOU					51						//偏瘦
//#define ZHENG_CHANG					52						//正常
//#define HUAN_ZAI_CI_YING			57						//欢迎再次使用
//#define WO_LI						29						//握力
//#define QING_YONG_LI_WO				38						//请用力握住握力器


#define  TIM_LONG				60							//每局的时长单位s
#define  TIM_PERIOD				1000						//时间检测周期1000ms
//屏幕地值
#define	TFT_ADRESS_TIM_BACK		0x0001						//倒计时地址
#define TFT_ADRESS_SCOERE		0x0002						//击中次数
#define TFT_ADRESS_COUNT		0x0004						//总点击次数
#define TFT_ADRESS_BT			0x0003						//电池电量
#define TFT_ADRESS_DISHU		0x0005						//地鼠变量图标
#define TFT_BUTTON				0x4F						//TFT屏上的按钮
#define TFT_ADRESS_LAST_SCORE	0x0006						//成绩地址
#define TFT_ADRESS_SET_LEVEL	0x0007						//等级设置
#define TFT_PAGE_SUCCESS		2							//胜利页面
#define TFT_PAGE_FAIL			3							//失败页面
#define TFT_PAGE_SET			4							//设置页面
#define TFT_PAGE_DAT			1							//数据页面
#define TFT_PAGE_DEBUG			5							//调试页面
#define TFT_ADRESS_DEBUG_COUNT	0x0008						//调试页面数据
//音频地址
#define TFT_MUSIC_ADRESS_SCORE	52							//胜利音乐
#define TFT_MUSIC_ADRESS_FAIL	50							//失败音乐
#define TFT_MUSIC_VALUE			0x01							//播放音量
//调试开关
#define DEBUG_PRINT					1                                
#endif // !__BSPCONFIG_H

