#ifndef __FRAMEBOX_H
#define __FRAMEBOX_H
#include "sys.h"
/*
ʱ��:2018 6 9
���ߣ�latefly
����:һ�����洰�ڵ���ϰ����

���µķ�ʽ�����ײ������ֱ�ӵ��õײ�����޸�����
������Ҫ�ײ�ķ���
drawpoint
setcursor

*/


typedef struct
{
	int X;
	int Y;
	int height;
	int width;
	int HeadHight;
	int BottomHight;
	char title[10];
	int id;
	
}__FrameBox;

typedef struct
{
	int X;
	int Y;
	int height;
	int width;
	int owner;
	
}__FrameVeiw;


//�˴��ķ�����Ҫ�ײ��ʵ��
#define FrameDrawPoint(Color) LCD_WR_DATA(Color)
#define FrameSetCursor(Xpos,Ypos) LCD_SetCursor(Xpos,Ypos)

/*
ÿ��frame����Ҫid����������
��ʱ��ÿ�����������id�����м����

��ʱ��Frame��δ�����¼������¼��Ժ󻹻���һ��������һ��
������������ȼ�ָ��

*/
//���õĵ��÷�������������������г���
void FrameClear(u16 Color);
void FrameFull(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color);
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color);
void FrameFont16(unsigned int x,unsigned int y,unsigned char index);
void FrameFont32(unsigned int x,unsigned int y,unsigned char index);
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color);
void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color);

//����Լ������ķ���
void FrameBoxInit(void);
char FrameBox(void);
char FrameStart(void);
char FramePlay(void);
char FrameSeting(void);
char FrameTest(void);

//�����ͼ������Ϊ����׼�����Ժ���Ϊ�����Ŀؼ�׼��
char AddMessage(char *message);
void ShowMessage(void);
#endif


