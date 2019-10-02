#include "stm32f10x_it.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "SPI.h"
#include "LCD_2_0.h"
//#include "FrameBox.h"
#include "w25qxx.h"
#include "MicroSD.h"
#include "Key.h"
#include "StringOperation.h"
#include "vs10xx.h"	

/***************************************************************************************
由于这里的硬件的实验单项都测试过了所以这里将进行所有的实验以及文字库的集成fat的集成这里也将会是
一个漫长的过程
硬件的列表如下：
LED
LCD
time
uart
key
W25Q128
MicroSD
VS1053B





****************************************************************************************/
u16 color=0x000;
extern char MicroSDDataBuff[512];
void LED_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
}

void MicroSDTest()
{
	//这是在内存卡初始化以后的测试进行读写如果不成功还是从读写序列号开始吧
	
	 //WriteSectorToMicroSD(2,"Hello I here did you find me!!!");
	 delay_ms(200);
	 ReadSectorFromMicroSD(2,MicroSDDataBuff);
   printf("MicroSD read is:%s!!!!\n\n",MicroSDDataBuff);
	
}
void W25Q128TEST(){
	char arr[100]={"this is a w25q128 test!!!"};
	W25QXX_Write((u8 *)arr,1000,sizeof(arr));
	EmptyString(arr);
	W25QXX_Read((u8 *)arr,100,sizeof(arr));
	printf("W25QRead:%s\n\n",arr);
	
}



int main()
{
		//char arr[100]="this is a test about frameveiw and you will see this display about in this windows!";
		char key=0x80;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		SPI2_Init();
		SPI1_Init();
		uart_init(115200);
		delay_init();
		W25QXX_Init();
		
		SD_Init();
		LED_Init();
		VS_Init();
		KeyInit();
		
		LCD_Init();
		printf("I am working now\n\n");
		LCD_Fill(10,10,150,150,LGRAY);
		LCD_Clear(WHITE);
		FrameDrawCircle(100,100,50,RED);
		FrameShowString(0,150,176,20,16,(u8 *)"this is frame test!!!!",BLACK);
	
	
	
		MicroSDTest();
		W25Q128TEST();	
		delay_ms(200);
		while(1){
				if(USART_RX_STA&0x8000)
				{
					if(StringEqual("test",(char *)USART_RX_BUF))
					{
						printf("Ram Test:0X%04X\r\n",VS_Ram_Test());//打印RAM测试结果	
						printf("This is a StringOperation test!!!\n");
						//LCD_Fill(0,0,50,50,WHITE);
					}else if(StringEqual("low",(char *)USART_RX_BUF))
					{
						printf("GPIO set low!!!\n");
						GPIO_ResetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_0|GPIO_Pin_1);
						GPIO_ResetBits(GPIOA,GPIO_Pin_8);
					}else if(StringEqual("high",(char *)USART_RX_BUF))
					{
						printf("GPIO set high!!!\n");
						GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_0|GPIO_Pin_1);
						GPIO_SetBits(GPIOA,GPIO_Pin_8);
					}else if(StringEqual("reset",(char *)USART_RX_BUF))
					{
						printf("LCD init!!!\n");
						LCD_Init();
					}
					else if(StringEqual("sin",(char *)USART_RX_BUF))
					{
						printf("Vs1053B sin test!!!\n");
						VS_Sine_Test();
						delay_ms(500);
					}
					USART_RX_BUF[0]='\0';
					USART_RX_STA=0;
				}
				delay_ms(10);
				//VS_Sine_Test();
				key=GetKeyCilk();
				switch(key){
					case KEY_Up:printf("you pushed KEY_Up button!\n");printf("Ram Test:0X%04X\r\n",VS_Ram_Test());break;
					case KEY_Down:printf("you pushed KEY_Down button!\n");VS_Sine_Test();break;
					case KEY_Left:printf("you pushed KEY_Left button!\n");LCD_Clear(GREEN);break;
					case KEY_Right:printf("you pushed KEY_Right button!\n");LCD_Clear(YELLOW);break;
					case KEY_Ok:printf("you pushed KEY_Ok button!\n");LCD_Init();break;
					case KEY_Back:printf("you pushed KEY_Back button!\n");break;
					
					
			}
		}
}


/*

int y=170,len=200;
for(;y<320;y+=5)
				{
					LCD_DrawLine(0,y,len,y);
					len+=5;
				}




			char isPushed=0;
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			SPI2_Init();
			Event_Pin_Init();
			uart_init(115200);
			delay_init();
			LCD_Init();
			delay_us(10);
			while(!IsKeyPushed())
			{
				printf("push the key A0 right now\n");
			}
			printf("I am working now\n");
			LCD_Clear(WHITE);
			while(1)
			{
				FrameBox(0,0,240,160,"This is My First Test!");
				//delay_ms(300);
				FrameBox(0,160,240,160,"This is My Second Test!");
				//delay_ms(1000);
				FrameBox(30,140,240,160,"This is My Thrid Test!");
				FrameBox(10,20,240,50,"This is My Fourth Test!");
				delay_ms(1000);
			}
*/

