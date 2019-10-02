#ifndef __MONITOR_H
#define __MONITOR_H
#include "stm32f10x.h"


void Monitor_Control(char offset,char defaultvaule);
void TIM3_PWM_Init(u16 arr,u16 psc);
void SetCCR2(u16 ccr);

#endif

