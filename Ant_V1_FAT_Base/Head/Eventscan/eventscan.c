#include "eventscan.h"

/***
此处需要使用到adc
PB0	ADC_IN8
PB1 ADC_IN9

以及一个逻辑输出判断管脚
PA7 logicalin
***/
void delay(int x)
{
	while(x--);
}
void Event_Pin_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

char IsKeyPushed(void)
{
	char isPushed=0;
	if(LOGICALIN)
	{
		delay(500);
		if(LOGICALIN)
			isPushed=1;
	}
	return isPushed;
}



