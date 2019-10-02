#ifndef _LCD_H
#define _LCD_H
#include "sys.h"

/*
PB9		SCK
PB8	MISO
PB7	MOSI

PB12	D/C
PB13	RESET
PB14	CS



typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

//LCD重要参数集
typedef struct  
{ 					    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8	wramcmd;		//开始写gram指令
	u8  setxcmd;		//设置x坐标指令
	u8  setycmd;		//设置y坐标指令	 
}_lcd_dev; 
*/



//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);													   	//初始化
void LCD_SetCursor(u16 Xpos, u16 Ypos);													//关显示
void LCD_Clear(u16 Color);	 											//设置光标
/*
void LCD_DrawPoint(u16 x,u16 y,u16 Color);											//画点
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);								//快速画点
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void Draw_Circle(u16 x0,u16 y0,u8 r);
void ShowChinese16(unsigned int x,unsigned int y,unsigned char index);
void ShowChinese32(unsigned int x,unsigned int y,unsigned char index);

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);

*/
//FrameBox的方法
void LCDrawPoint(int Color);
void LCDSetCursor(int Xpos,int Ypos);
#endif

