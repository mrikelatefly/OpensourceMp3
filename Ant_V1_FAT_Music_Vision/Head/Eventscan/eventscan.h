#ifndef __EVENTSCAN_H
#define __EVENTSCAN_H

#include "sys.h"

/***
此处需要使用到adc
PB0	ADC_IN8
PB1 ADC_IN9

以及一个逻辑输出判断管脚
PA7 logicalin
***/
#define JUMP 1
#define ROLL 2
#define BALENCE 3
#define LOGICALIN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//逻辑输入


/*

#define A 	GPIOA
#define B		GPIOB

#define Pin_1 GPIO_Pin_1
#define Pin_2 GPIO_Pin_1
#define Pin_3 GPIO_Pin_1
#define Pin_4 GPIO_Pin_1
#define Pin_5 GPIO_Pin_1
#define Pin_6 GPIO_Pin_1
#define Pin_7 GPIO_Pin_1
#define Pin_8 GPIO_Pin_1
#define Pin_9 GPIO_Pin_1
#define Pin_10 GPIO_Pin_1
#define Pin_11 GPIO_Pin_1
#define Pin_1 GPIO_Pin_1
#define Pin_1 GPIO_Pin_1
#define Pin_1 GPIO_Pin_1
#define Pin_0 GPIO_Pin_1

*/

void Event_Pin_Init(void);
char IsKeyPushed(void);


#endif

