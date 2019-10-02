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


****************************************************************************************/
u16 color=0x000;
extern char MicroSDDataBuff[512];


FATFS fs0;
FIL file1;     //�ļ�1
FIL ftemp1;     //�ļ�2.
UINT br1,bw1;   //��д����
FILINFO fileinfo1; //�ļ���Ϣ
DIR dir1;  
FILINFO fileInfo2;
UINT br2,bw2;
 

FRESULT res;
u32 nCapacity;
char buffer[512];


void FatTest(){
	
	 /*******��������*****/
	FIL fil;
	FRESULT res;
	UINT bww;
	res=f_open (&fil,"0:/message.txt", FA_CREATE_ALWAYS|FA_WRITE);	
	 
	f_write (&fil, " SD_CARD test1 \r\n", sizeof("SD_CARD test1 ")+2, &bww);
	f_write (&fil, " SD_CARD test2 \r\n", sizeof("SD_CARD test2 ")+2, &bww);
	f_write (&fil, " SD_CARD test3 \r\n", sizeof("SD_CARD test3 ")+2, &bww);
	f_write (&fil, " SD_CARD test4 \r\n", sizeof("SD_CARD test4 ")+2, &bww);
	f_write (&fil, " SD_CARD test5 \r\n", sizeof("SD_CARD test5 ")+2, &bww);
	
	//f_close(&fil);
	
	//res=f_open (&fil,"0:/message.txt", FA_CREATE_ALWAYS|FA__WRITTEN);	
	
	f_write (&fil, " SD_CARD test6 \r\n", sizeof("SD_CARD test6 ")+2, &bww+100);
	f_write (&fil, " SD_CARD test7 \r\n", sizeof("SD_CARD test7 ")+2, &bww+100);
	f_write (&fil, " SD_CARD test8 \r\n", sizeof("SD_CARD test8 ")+2, &bww+100);
	f_write (&fil, " SD_CARD test9 \r\n", sizeof("SD_CARD test9 ")+2, &bww+100);
	f_write (&fil, " SD_CARD test0 \r\n", sizeof("SD_CARD test0 ")+2, &bww+100);
	
	f_close(&fil);
}

int main(void) 
{  
  u32 total,free;
  //char arr[100]="this is a test about frameveiw and you will see this display about in this windows!";
		char key=0x80;
		//FATFS fats;
	
	
		//u32 retry=0;	 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		SPI2_Init();
		SPI1_Init();
		uart_init(115200);
		delay_init();
		KeyInit();
		res=MicroSDInit();
	 if(res==0)
	 {
		printf(" sd����ʼ���ɹ�!");
	 }

 res= f_mount(&fs0,"0:",1);    /* Mount a logical drive */;
 //f_mkfs(0,1,4096);
  res=exf_getfree((u8 *)"0:",&nCapacity,&free) ;//�õ�SD������������ʣ������
	printf(" Error Code:%d   \n",res);
	 
  printf(" ��������%d MB,ʣ��������%d MB",nCapacity>>10,free>>10);
	FatTest();
 res=f_open(&file1, "0:/message.txt", FA_OPEN_EXISTING|FA_READ);
	res=f_read (
	&file1,   /* Pointer to the file object */
	 buffer,  /* Pointer to data buffer */
	 512,  /* Number of bytes to read */
	 &br  /* Pointer to number of bytes read */
	);
  f_close(&file1);
  printf("buffer:%s",buffer);
	
	 
	 
	 
  res= f_mount(NULL,(TCHAR*)1,1); 
  while(1);


   

}