# Agile_Train
敏捷训练器 
主芯片:Stm32CBT6  
## 74HC595引脚分配  
|Pin Number   |Be used  |  
|:----------  |:------  |  
|PB12          |HC595_DS_Pin     |  
|PB13          |HC595_RCK_Pin |  
|PB14          |HC595_SCK_Pin |  
## KEY引脚  
 |Be used   |Pin Number   |  
|:----------  |:------  |
 |K0_3_PORT	|GPIOB|
 |K0_PIN|		GPIO_PIN_11|
 |K1_PIN		|GPIO_PIN_10|
 |K2_PIN|		GPIO_PIN_1|
 |K3_PIN		|GPIO_PIN_0|
 |K4_6_PORT	|GPIOA|
| K4_PIN		|GPIO_PIN_7|
 |K5_PIN		|GPIO_PIN_6|
 |K6_PIN		|GPIO_PIN_5|  
 
## 敏捷训练器最终成绩算法:  
 设：每一局的时间为T.  
	击中的个数为n.  
	灯亮周期为t1.  
	用户按动按键的次数为m.  
没扣除错按前成绩C=(n/m)*[m/(T/t1)],化简得:C=(n*t1)/T  
解释:J = n/m,为基本的成绩数  
	 T/t1,为在规定的时间内可亮灯的个数  
	 K = m/T/t1,为用户按下的占应该按下的比值  
	 然后用  =J*K 得成绩  
	 最后成绩为:J*J*K  扣除错按得成绩  