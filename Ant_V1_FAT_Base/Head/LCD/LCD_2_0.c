#include "LCD_2_0.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"
#include "SPI.h"
//#include "font.h"


//�����ɸ��Ľӿ�IO���ã�ʹ����������4 IO������ɱ���Һ��������ʾ
/***************************************************************************************
#define LCD_CS_SET		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET	GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_RST_SET		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LCD_RST_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define LCD_DC_SET		GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LCD_DC_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

CS	PA8
DC	PC0
RST	PC1

**********************************************************************************************/
/**************************************����ܽŲ���****************************************/

#define LCD_CS_SET		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET	GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_DC_SET		GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LCD_DC_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define LCD_RST_SET		GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LCD_RST_RESET	GPIO_ResetBits(GPIOC,GPIO_Pin_1)




//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

//������ɫ,������ɫ
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
u16 DeviceCode;	 


/****************************************************************************
* ��    �ƣ�void LCD_GPIO_Init(void)
* ��    �ܣ�STM32_ģ��SPI���õ���GPIO��ʼ��
* ��ڲ�������
* ���ڲ�������
* ˵    ������ʼ��ģ��SPI���õ�GPIO
****************************************************************************/
void LCD_GPIOInit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	/*CS	PA8
		DC	PC0
		RST	PC1
	*/
	//����SPI2�ܽ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
}


/****************************************************************************
* ��    �ƣ�Lcd_WriteIndex(u8 Index)
* ��    �ܣ���Һ����дһ��8λָ��
* ��ڲ�����Index   �Ĵ�����ַ
* ���ڲ�������
* ˵    ��������ǰ����ѡ�п��������ڲ�����
****************************************************************************/
void LCD_WR_REG(u8 Index)
{
   LCD_CS_RESET;
   LCD_DC_RESET;
	 delay_us(10);
   SPI_WriteByte(SPI2,Index);
	 delay_us(10);
   LCD_CS_SET;
}

/****************************************************************************
* ��    �ƣ�Lcd_WriteData(u8 Data)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   LCD_CS_RESET;
   LCD_DC_SET;
	 delay_us(10);
   SPI_WriteByte(SPI2,Data);
	 delay_us(10); 
	 LCD_CS_SET;
}


//******************************************************************
//��������  LCD_WR_DATA_16Bit
//���ܣ�    8λ���������д��һ��16λ����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_DATA(u16 data)
{	
   LCD_CS_RESET;
   LCD_DC_SET;
   SPI_WriteByte(SPI2,data>>8);
   SPI_WriteByte(SPI2,0xff&data);
   LCD_CS_SET;
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�    ronview
//���ڣ�    2015-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);  		 
}	


//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�    ronview
//���ڣ�    2015-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
	//LCD_WR_REG(0x2c);
}	 

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�    ronview
//���ڣ�    2015-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);//LCD_SetCursor(x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 
}

//******************************************************************
//��������  LCD_Clear
//���ߣ�    ronview
//���ڣ�    2015-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);  
	for(index=0;index<lcddev.width*lcddev.height;index++)
	{
		LCD_WR_DATA(Color);    
	}
} 

//******************************************************************
//��������  LCD_Init
//���ߣ�    ronview
//���ڣ�    2015-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{      										 

	LCD_GPIOInit();
	LCD_RST_SET;
	delay_us(100);
	LCD_RST_RESET;
	delay_ms(200);	
	LCD_RST_SET;
	delay_ms(200);
	//LCD Init For 2.0inch LCD Panel with ILI9225G.	
//************* Start Initial Sequence **********//		
//ILI9225G
	LCD_WR_REG(0x0028);
	LCD_WR_DATA(0x00CE);//SOFTWVAE RESET
	delay_ms(100);
	LCD_WR_REG(0x0028);
	LCD_WR_DATA(0x01CE);
    
  LCD_WR_REG(0x0000);
	LCD_WR_DATA(0x0001);
	LCD_WR_REG(0x0001);  //Driver output control
	LCD_WR_DATA(0x011C);	
	LCD_WR_REG(0x0002);  //LCD Driving waveform control
	LCD_WR_DATA(0x0100);	
	LCD_WR_REG(0x0003);  //Entry Mode
	LCD_WR_DATA(0x1030);		
		
  LCD_WR_REG(0x0008);	//set BP and FP
	LCD_WR_DATA(0x0303);		

	LCD_WR_REG(0x000C);	//18bit RGB
	LCD_WR_DATA(0x0000);		
	LCD_WR_REG(0x000F);	//osc control
	LCD_WR_DATA(0x0601);		
	LCD_WR_REG(0x0020);
	LCD_WR_DATA(0x0000); 
	LCD_WR_REG(0x0021);
	LCD_WR_DATA(0x0000); 
	delay_ms(100);
//=========POWER CONTROL================	
	LCD_WR_REG(0x0010);	//SET SAP,DSTB,STB
	LCD_WR_DATA(0x0A00);			
	LCD_WR_REG(0x0011);	//SET APON,PON,AON,VI1EN,VC
	LCD_WR_DATA(0x1037);		
	LCD_WR_REG(0x0012);  //DDVDH,VCL,VGH,VGL
	LCD_WR_DATA(0x5121);		
	LCD_WR_REG(0x0013);	//SOURCE VOLTAGE
	LCD_WR_DATA(0x0066);
  delay_ms(100);	
	LCD_WR_REG(0x0014); //Vcom , VCOMLamplitude
	LCD_WR_DATA(0x5a62);	   //Vcomh

	delay_ms(100);
	
	LCD_WR_REG(0x0030);//GATE SCAN CONTROL
	LCD_WR_DATA(0x0000); //
	
	LCD_WR_REG(0x0031);// Vertical scrol control(end)
	LCD_WR_DATA(0x00DB);//
	
  LCD_WR_REG(0x0032);//Vertical scrol control(stard)
	LCD_WR_DATA(0x0000);//
	
	LCD_WR_REG(0x0033);  //
	LCD_WR_DATA(0x0000);  //
	
	LCD_WR_REG(0x0036); //
	LCD_WR_DATA(0x00AF); //
	
  LCD_WR_REG(0x0037);//
	LCD_WR_DATA(0x0000);//	
	
  LCD_WR_REG(0x0038);//
	LCD_WR_DATA(0x00DB);//
	
	LCD_WR_REG(0x0039);//
	LCD_WR_DATA(0x0000);//
		
  //==========gamma
	LCD_WR_REG(0x0050);//
	LCD_WR_DATA(0x0000);//pv8;v1
	LCD_WR_REG(0x0051);//
	LCD_WR_DATA(0x050C);//pv43,v20
	LCD_WR_REG(0x0052);//
	LCD_WR_DATA(0x0C09);//pv62,v55
	LCD_WR_REG(0x0053);//
	LCD_WR_DATA(0x0204);//
	LCD_WR_REG(0x0054);//
	LCD_WR_DATA(0x090C);//nv55,v62
	LCD_WR_REG(0x0055);//
	LCD_WR_DATA(0x0C05);//nv20,v43
  LCD_WR_REG(0x0056);//
	LCD_WR_DATA(0x0404);//nv1,v8
	LCD_WR_REG(0x0057);//
	LCD_WR_DATA(0x0402);//
	LCD_WR_REG(0x0058);//
	LCD_WR_DATA(0x0A00);//
	LCD_WR_REG(0x0059);//
	LCD_WR_DATA(0x000A);//
	//=====display on
	LCD_WR_REG(0x0007);
	LCD_WR_DATA(0x0012);	
	delay_ms(100);
	LCD_WR_REG(0x0007);
	LCD_WR_DATA(0x1017);  
	delay_ms(200);
	//LCD_LED_SET;//��������	 
	LCD_SetParam();//��������в����ĳ�ʼ��
	LCD_Clear(BLACK);
}
  		  
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
#if USE_HORIZONTAL
	LCD_WriteReg(0x38,xEnd);
	LCD_WriteReg(0x39,xStar);
	LCD_WriteReg(0x36,yEnd);
	LCD_WriteReg(0x37,yStar);
#else	
	LCD_WriteReg(0x36,xEnd);
	LCD_WriteReg(0x37,xStar);
	LCD_WriteReg(0x38,yEnd);
	LCD_WriteReg(0x39,yStar);
#endif
	LCD_WriteReg(lcddev.setxcmd,xStar);
	LCD_WriteReg(lcddev.setycmd,yStar);
	
	LCD_WriteRAM_Prepare();				
} 

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);  //HS
	LCD_WR_DATA(Xpos); 		 
	LCD_WR_REG(lcddev.setycmd);  //VS
	LCD_WR_DATA(Ypos);
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM	
} 

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x22;
#if USE_HORIZONTAL==1	//ʹ�ú���	  
	lcddev.dir=1;//����
	lcddev.width=220;
	lcddev.height=176;
	lcddev.setxcmd=0x21;
	lcddev.setycmd=0x20;
	LCD_WriteReg(0x01, 0x001C);	
	LCD_WriteReg(0x03,0x1038);
	LCD_WriteReg(0x0060,0x2700);

#else//����
	lcddev.dir=0;//����				 	 		
	lcddev.width=176;
	lcddev.height=220;
	lcddev.setxcmd=0x20;
	lcddev.setycmd=0x21;
	LCD_WriteReg(0x01, 0x011C);
	LCD_WriteReg(0x03,0x1030);
	LCD_WriteReg(0x0060,0xA700);
#endif
}	  

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
		LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//���ù��λ�� 	    
	}
}























