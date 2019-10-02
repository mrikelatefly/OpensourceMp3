#include "stm32f10x_it.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "SPI.h"
#include "LCD_2_0.h"
#include "FrameBox.h"
#include "w25qxx.h"
#include "MicroSD.h"
#include "Key.h"
#include "malloc.h" 
#include "StringOperation.h"
#include "vs10xx.h"	
#include "ff.h"  
#include "exfuns.h"
/***************************************************************************************
���������Ӳ����ʵ�鵥����Թ����������ｫ�������е�ʵ���Լ����ֿ�ļ���fat�ļ�������Ҳ������
һ�������Ĺ���
Ӳ�����б����£�
LED
LCD
time
uart
key
W25Q128
MicroSD
VS1053B

����Fat����
��Ҫ
��Ӧ�ӿ�

1.flash�Լ�MicroSD 			�ӿڲ�������
2.�޸�ffconf�������ļ� 	���ø�������ԭ��
3.�����Ƿ������� 				��д������

���Ϲ��ܻ���ʵ��
2019 9 30

ʵ�����ֵĲ���

1.�ļ��Ķ�ȡ
2.���ݴ���
3.��������
4.���Ź��ܵ�ʵ��





****************************************************************************************/
u16 color=0x000;
extern char MicroSDDataBuff[512];

#define LED		PCout(13) 




/***************************************************************************************

mp3��ȡ����
Ӳ����ʼ��
fat��ʼ��

���ļ�
f_opendir(&mp3dir,"0:/MUSIC")
 �õ�����Ч�ļ���
totmp3num=mp3_get_tnum("0:/MUSIC");
//Ϊ���ļ������������ڴ�
mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	

vs1053b��λ
VS_HD_Reset();
VS_Soft_Reset();

��ȡ������ϢȻ�󲥷�
mp3_index_show(curindex+1,totmp3num);
key=mp3_play_song(pname); 

��������
VS_Send_MusicData(databuf+i)==0	


***************************************************************************************/
char MusicNameBuffer[4][50]={
"Wannabe.mp3",
"500Miles.mp3",
"Spongebob.mp3",
"Sailing.mp3"
};

void MusicPlay()
{
	//int musicIndex=0;//���ڱ�ʾ���ŵ�
	//char isPaly=0x00;
	FRESULT res;
	FIL file1;     //�ļ�1
	u8 buffer[4096];
	u32 counter=0;
	
	res=f_open(&file1, "0:/Spongebob.mp3", FA_OPEN_EXISTING|FA_READ);
	printf("Opening music 0:/music/Spongebob.mp3  \n\n");//Spongebob.mp3
	delay_ms(10);
	VS_HD_Reset();
	VS_Soft_Reset();
		
	VS_Set_Vol(220);
	VS_Restart_Play();  					//�������� 
	VS_Set_All();        					//������������Ϣ 			 
	VS_Reset_DecodeTime();					//��λ����ʱ�� 
	VS_SPI_SpeedHigh();	//����
	
	while(1)
	{
		if(res==0x00){
			
			counter=0;
			res=f_read(&file1,buffer,4096,&br);
			do{ //������ѭ��
			   if(VS_Send_MusicData(buffer+counter)==0)//��VS10XX������Ƶ����
				 {
						counter+=32;
					  LED=~LED;
				 }
			}while(counter<4096);//ѭ������4096���ֽ� 
			if(br!=4096||res!=0){
					f_close(&file1);
					printf("Music played over!!!\n\n");
					break;//������.		  
			}
		}else{
			printf("Open music failed\n\n");
			break;
		}
			
		
	}
	
}


void FatTest(){
	
	 /*******��������*****/
	FRESULT res;
	FIL file1;     //�ļ�1
	char buffer[512];
	
	res=f_open(&file1, "0:/message.txt", FA_OPEN_EXISTING|FA_READ);
	res=f_read (&file1,buffer,512,&br);
	f_close(&file1);
  printf("buffer:%s",buffer);
	
}
void LED_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );//PORTAʱ��ʹ�� 
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //PA 5/6/7����������� 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}


/**************************
���µ�����
1.�ļ���ȡ������ һ�����ļ��� ���������ֲ�ͬ�ĸ�ʽ��һ���ļ��оͻ�򲻿������

2.�ļ����Ļ�ȡ
3.�ļ�����ʾ����
4.�����л�������

**************************/
int main(void) 
{  
  u32 total,free;
  //char arr[100]="this is a test about frameveiw and you will see this display about in this windows!";
	FRESULT res;
	FATFS fs0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SPI2_Init();
	SPI1_Init();
	uart_init(115200);
	delay_init();
	KeyInit();
	//LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KeyInit();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD     
	W25QXX_Init();				//��ʼ��W25Q128
 	VS_Init();	  				//��ʼ��VS1053 
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
	
 	//f_mount(fs[0],"0:",1); 		//����SD�� 
 	//f_mount(fs[1],"1:",1); 		//����FLASH.
	res= f_mount(&fs0,"0:",1);    /* Mount a logical drive */;
	//f_mkfs(0,1,4096);
  res=exf_getfree((u8 *)"0:",&total,&free) ;//�õ�SD������������ʣ������
	printf(" Error Code:%d   \n",res);
  printf(" ��������%d MB,ʣ��������%d MB     \n\n",total>>10,free>>10);
	FrameStart();
	//FatTest();
	MusicPlay();
	//ж���ڴ濨
  res= f_mount(NULL,(TCHAR*)1,1);
  while(1);
}



