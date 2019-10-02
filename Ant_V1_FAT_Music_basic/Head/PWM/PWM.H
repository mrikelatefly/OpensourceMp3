#ifndef __PWM_H
#define __PWM_H
#include"sys.h"


void TIM3_PWM_Init(u16 arr,u16 psc);//传入分频系数，自动重装载值
void SetCCR2(u16 ccr);

#endif

