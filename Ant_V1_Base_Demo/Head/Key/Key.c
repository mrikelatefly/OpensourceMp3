#include "Key.h"
/*
void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);  //PB13/14/15����
}
char GetKeyCilk()
{
	char keyValue=0x80;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		keyValue=Key_WakeUp;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		keyValue=Key_Back;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		keyValue=Key_OK;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
	{
		keyValue=Key_Menu;
	}
	
	return keyValue;
	
	
}

*/

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  // PB12 ���� 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��������
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);
}


char GetKeyCilk(void)
{
	char keyValue=0x80;
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		keyValue=KEY_Ok;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		keyValue=KEY_Back;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)==0)
	{
		keyValue=KEY_Up;
	}else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==0)
	{
		keyValue=KEY_Down;
	}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==0)
	{
		keyValue=KEY_Left;
	}else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		keyValue=KEY_Right;
	}
	return keyValue;
}

