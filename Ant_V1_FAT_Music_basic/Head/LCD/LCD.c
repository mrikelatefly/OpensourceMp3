#include "LCD.h"
#include "delay.h"
#include "sys.h"
#include "SPI.h"
//#include "font.h"
/*
SPI2
只会用到三根线 
MOSI PB15
MISO PB14
SCK PB13 

CS PB7
RESET PB8
D/C PB9

这里准备重新写入SPI
防止频繁的更改这里提供原始的设置信息
#define LCD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define LCD_CS_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define LCD_RESET_SET GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define LCD_RESET_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define LCD_DC_SET GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define LCD_DC_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_9)

void LCD_Pin_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOB,GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_8);  //上拉所有
}
*/

#define LCD_CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define LCD_CS_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_8)

#define LCD_RESET_SET GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define LCD_RESET_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define LCD_DC_SET GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define LCD_DC_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_11)

void LCD_Pin_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );//PORTB时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_11| GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB

	GPIO_SetBits(GPIOA,GPIO_Pin_8| GPIO_Pin_11| GPIO_Pin_12);  //上拉所有
}

//这里就是要改写的方法
void SentBit(char data)
{
	SPI_WriteByte(SPI2,data);
}
void LCD_WR_CMD(char cmd)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_RESET;//dc=0 cmd
	SentBit(cmd);
	LCD_CS_SET;//cs=1
	
}
void LCD_WR_DATA8(char data)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_SET;//dc=1 date
	SentBit(data);
	LCD_CS_SET;//cs=1
}
void LCD_WR_DATA(u16 data)
{
	LCD_CS_RESET;//cs=0
	LCD_DC_SET;//dc=0 cmd
	SentBit(data>>8);
	SentBit(0x00ff&data);
	LCD_CS_SET;//cs=1
}

void LCD_WR_CMD_DATA(char LCD_CMD, u16 LCD_DATA)
{
	LCD_WR_CMD(LCD_CMD);
	LCD_WR_DATA(LCD_DATA);
}

void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_CMD(0x2c);  
}

void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WR_CMD(0x2a); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_CMD(0x2b); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
	LCD_WriteRAM_Prepare();	
} 
void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(Color);//此处默认为深蓝色
	//LCD_WR_CMD(0x2c);
} 
void LCD_Init(void)
{
	LCD_Pin_Init();
	delay_us(100);
	
	LCD_RESET_RESET;		//reset=0 
 	delay_ms(50); // delay 20 ms 
	LCD_RESET_SET;		 
 	delay_ms(50); // delay 20 ms 
/*
	SPILCD_RST_RESET ;	//LCD_RST=0	 //SPI接口复位
	delay_ms(20); // delay 20 ms 
	SPILCD_RST_SET ;	//LCD_RST=1		
	delay_ms(20);	
	
		lcddev.width=240;
		lcddev.height=320;
		lcddev.wramcmd=0X2C;
		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B; 	
*/

		
				LCD_WR_CMD(0xCB);  
        LCD_WR_DATA8(0x39); 
        LCD_WR_DATA8(0x2C); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x34); 
        LCD_WR_DATA8(0x02); 

        LCD_WR_CMD(0xCF);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0XC1); 
        LCD_WR_DATA8(0X30); 
 
        LCD_WR_CMD(0xE8);  
        LCD_WR_DATA8(0x85); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x78); 
 
        LCD_WR_CMD(0xEA);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_CMD(0xED);  
        LCD_WR_DATA8(0x64); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0X12); 
        LCD_WR_DATA8(0X81); 

        LCD_WR_CMD(0xF7);  
        LCD_WR_DATA8(0x20); 
  
        LCD_WR_CMD(0xC0);    //Power control 
        LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
        LCD_WR_CMD(0xC1);    //Power control 
        LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WR_CMD(0xC5);    //VCM control 
        LCD_WR_DATA8(0x3e); //对比度调节
        LCD_WR_DATA8(0x28); 
 
        LCD_WR_CMD(0xC7);    //VCM control2 
        LCD_WR_DATA8(0x86);  //--
 
        LCD_WR_CMD(0x36);    // Memory Access Control 
        LCD_WR_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_WR_CMD(0x3A);    
        LCD_WR_DATA8(0x55); 

        LCD_WR_CMD(0xB1);    
        LCD_WR_DATA8(0x00);  
        LCD_WR_DATA8(0x18); 
 
        LCD_WR_CMD(0xB6);    // Display Function Control 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x82);
        LCD_WR_DATA8(0x27);  
 
        LCD_WR_CMD(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_CMD(0x26);    //Gamma curve selected 
        LCD_WR_DATA8(0x01); 
 
        LCD_WR_CMD(0xE0);    //Set Gamma 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x2B); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x4E); 
        LCD_WR_DATA8(0xF1); 
        LCD_WR_DATA8(0x37); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x10); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x09); 
        LCD_WR_DATA8(0x00); 

        LCD_WR_CMD(0XE1);    //Set Gamma 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x14); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x11); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0xC1); 
        LCD_WR_DATA8(0x48); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x36); 
        LCD_WR_DATA8(0x0F); 
 
        LCD_WR_CMD(0x11);    //Exit Sleep 
        delay_ms(120);
				
        LCD_WR_CMD(0x29);    //Display on 
        LCD_WR_CMD(0x2c); 
		LCD_Clear(WHITE);
}

/*

void LCD_DisplayOn(void)
{
	LCD_WR_CMD(0x11);    //Exit Sleep 
}
void LCD_DisplayOff(void)
{
	LCD_WR_CMD(0x29);    //Display on 
}
*/
void LCD_Clear(u16 Color)
{
	u32 index=0;      
	u32 totalpoint=240;
	totalpoint*=320; 	//得到总点数
	LCD_SetCursor(0x0000,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     //开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATA(Color);
	}
}

void LCDrawPoint(int Color)
{
	//LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(Color);//此处默认为深蓝色
}
void LCDSetCursor(int Xpos,int Ypos)
{
	LCD_WR_CMD(0x2a); 
	LCD_WR_DATA8(Xpos>>8); 
	LCD_WR_DATA8(Xpos&0XFF);	 
	LCD_WR_CMD(0x2b); 
	LCD_WR_DATA8(Ypos>>8); 
	LCD_WR_DATA8(Ypos&0XFF);
	LCD_WriteRAM_Prepare();	
}

/*

void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//设置光标位置 	    
	}
}


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,Color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	LCD_DrawLine(x1,y1,x2,y1,Color);
	LCD_DrawLine(x1,y1,x1,y2,Color);
	LCD_DrawLine(x1,y2,x2,y2,Color);
	LCD_DrawLine(x2,y1,x2,y2,Color);
}

void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b,BLACK);             //5
 		LCD_DrawPoint(x0+b,y0-a,BLACK);             //0           
		LCD_DrawPoint(x0+b,y0+a,BLACK);             //4               
		LCD_DrawPoint(x0+a,y0+b,BLACK);             //6 
		LCD_DrawPoint(x0-a,y0+b,BLACK);             //1       
 		LCD_DrawPoint(x0-b,y0+a,BLACK);             
		LCD_DrawPoint(x0-a,y0-b,BLACK);             //2             
  	LCD_DrawPoint(x0-b,y0-a,BLACK);             //7     	         
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}

void ShowChinese16(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi16;    
	temp+=index*32;	
	for(j=0;j<16;j++)
	{
		LCD_SetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<2;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(BLACK);
				} 
				else
				{
					LCD_WR_DATA(WHITE);
				}   
			}
			temp++;
		}
	 }
}

void ShowChinese32(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi32;    
	temp+=index*128;	
	for(j=0;j<32;j++)
	{
		LCD_SetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//开始写入GRAM
		for(k=0;k<4;k++)
		{
			for(i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)
				{
					LCD_WR_DATA(BLACK);
				} 
				else
				{
					LCD_WR_DATA(WHITE);
				}   
			}
			temp++;
		}
	 }
}

//采用叠加的方式
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	
	for(t=0;t<size;t++)
	 {   
				if(size==12)temp=asc2_1206[num][t];  //调用1206字体
				else temp=asc2_1608[num][t];		 //调用1608字体 	                          
				for(t1=0;t1<8;t1++)
				{			    
						if(temp&0x80)LCD_DrawPoint(x,y,BLACK); 
						temp<<=1;
						y++;
						if(y>=320)return;//超区域了
						if((y-y0)==size)
						{
							y=y0;
							x++;
							if(x>=240)return;//超区域了
							break;
						}
				}
		}			
}   
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,1);
        x+=size/2;
        p++;
    }  
}


*/
