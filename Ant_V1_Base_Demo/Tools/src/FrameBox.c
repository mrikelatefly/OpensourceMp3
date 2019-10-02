#include "FrameBox.h"
#include "font.h"
#include "LCD.h"
#include "StringOperation.h"
#include "usart.h"
/*
ʱ��:2018 6 13
1.����Ϊһ����ͨ�Ľ������ҳ������
2.д��һЩ�򵥵Ĺ���
	������һ���ı��򣬻���һ���б��
3.�ײ��ṩ�ķ�����Ӧ�ó�������
	�����뵽��ʱsetCuror drawPoint 

*/
#define LCD_H_MAX 320
#define LCD_W_MAX 240

#define FrameMaxValue 10
u16 BackgroundColor=0x00;



// �����ṩĬ�ϵĲ�������ͨ�������ӿ����ı�ֵȻ���ػ����



/*
x,y,w,h,headhight,bottomheight,id
__Frame Frame[10]={
0,0,160,240,15,15,1


};
*/

__FrameBox FrameBoxList[FrameMaxValue]={
//x,y,w,h,headhight,bottomheight,id
0,0,0,0,0,0
};
//����ֻ�ǽ�frame���Ӧ��ֵ����frameveiwlist��
//X Y height width owner
__FrameVeiw FrameVeiwList[FrameMaxValue];
//Ĭ��idΪ-1Ϊδʹ��
void FrameBoxInit()
{
	int i;
	__FrameBox *FrameBox;
	__FrameVeiw *FrameVeiw;
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameBox=&FrameBoxList[i];
		
		FrameBox->BottomHight=0;
		FrameBox->HeadHight=0;
		FrameBox->height=0;
		FrameBox->id=-1;//Ĭ�ϱ��Ϊδʹ��
		FrameBox->width=0;
		FrameBox->X=0;
		FrameBox->Y=0;
		*(FrameBox->title)='\0';
	}
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameVeiw=&FrameVeiwList[i];
		
		FrameVeiw->height=0;
		FrameVeiw->owner=-1;
		FrameVeiw->width=0;
		FrameVeiw->X=0;
		FrameVeiw->Y=0;
	}
}


char FrameVeiw(__FrameBox *FrameBox);
char CrateFrameVeiw(int owner);
char DeleteFrameBox(int id);
char CreateFrameBox(int x,int y,int w,int h,char *title,int id);


char CreateFrameBox(int x,int y,int w,int h,char *title,int id)
{
	int i,isCrate=0;
	__FrameBox *FrameBox;
	__FrameVeiw *FrameVeiw;
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameBox=&FrameBoxList[i];
		if(FrameBox->id==-1)
		{
			//��ʼ��Framebox
			FrameBox->BottomHight=15;
			FrameBox->HeadHight=15;
			FrameBox->height=h;
			FrameBox->id=id;//Ĭ�ϱ��Ϊδʹ��
			FrameBox->width=w;
			FrameBox->X=x;
			FrameBox->Y=y;
			StringCopy(FrameBox->title,title);
			
			//��ʼ��FrameVeiw
			FrameVeiw=&FrameVeiwList[i];
			FrameVeiw->height=h-FrameBox->BottomHight-FrameBox->HeadHight;
			FrameVeiw->owner=id;
			FrameVeiw->width=w;
			FrameVeiw->X=x;
			FrameVeiw->Y=y+FrameBox->HeadHight;
			
			isCrate=1;
			break;
		}
	}
	printf("frameID:%d\ncreated!\n",FrameBox->id);
	printf("Title:%s\n",FrameBox->title);
	return isCrate;
}
char DeleteFrameBox(int id)
{
	char isDelete=0;
	int i=0;
	__FrameBox *FrameBox;
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameBox=&FrameBoxList[i];
		if(FrameBox->id==id)
		{
			__FrameVeiw *FrameVeiw;
			FrameBox->id=-1;
			for(i=0;i<FrameMaxValue;i++)
			{
				FrameVeiw=&FrameVeiwList[i];
				if(FrameVeiw->owner==id)
				{
					FrameVeiw->owner=-1;
					break;
				}
			}
			isDelete=1;
			break;
		}
	}
	return isDelete;
}
char CrateFrameVeiw(int owner)
{
	char isCrate=0;
	int i=0;
	__FrameBox *FrameBox;
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameBox=&FrameBoxList[i];
		if(FrameBox->id==owner)
		{
			__FrameVeiw *FrameVeiw;
			for(i=0;i<FrameMaxValue;i++)
			{
				FrameVeiw=&FrameVeiwList[i];
				if(FrameVeiw->owner==-1)
				{
					FrameVeiw->owner=owner;
					isCrate=1;
					break;
				}
			}
			break;
		}
	}
	return isCrate;
}


//�ж��Ƿ��ڻ���������
char IsInDrawLimittedAera(int *x,int *y)
{
	char isLimitted=0;
	
	return isLimitted;
}


/*frameVeiw��һ�·���
List�򵥵��б�
textField
drawaera


����������Ҫ����һ����Ҫһ��drawLimittedAera�ĸ���
�����Ժ�Ļ�ͼ
*/




char TextFeild(int owner,char *s)
{
	char isCreate=1;
	/*
	��Ҫ���frameVeiw��h��w
	�������ʱ�û���
	*/
	int i;
	int w,h,x,y;
	__FrameVeiw *FrameVeiw;
	for(i=0;i<FrameMaxValue;i++)
	{
			FrameVeiw=&FrameVeiwList[i];
			if(FrameVeiw->owner==owner)
			{
					w=FrameVeiw->height;
					h=FrameVeiw->width;
					x=FrameVeiw->X;
					y=FrameVeiw->Y;
					//FrameVeiw(&(FrameBoxList[i]));
					break;
			}
	}
	i=0;
	while(*s!='\0')
	{
		FrameShowChar(x,y,*s,16,BLACK);
		x+=8;
		if(x>(FrameVeiw->X+w+15))
		{
			x=FrameVeiw->X;
			y+=16;
		}
		if(y>(FrameVeiw->Y+h))
		{
			break;
		}
		s++;
		//FrameShowString(Frame.X+(Frame.HeadHight)*3,Frame.Y,200,16,16,(u8 *)Frame.title,BLACK);
	}
	return isCreate;
}
char FrameVeiw(__FrameBox *FrameBox)
{
	char isCreate=1;
	__FrameBox Frame=*FrameBox;
	FrameFull(Frame.X,Frame.Y+Frame.HeadHight,Frame.X+Frame.width,Frame.Y+Frame.height-Frame.BottomHight,LIGHTGREEN);
	return isCreate;
}

char FrameBox()
{
	
	//int x,int y,int width,int height,char *title
	char isCreate=1;
	int i;
	__FrameBox *FrameBox;
	__FrameBox Frame;//={0,0,160,240,15,15}
	for(i=0;i<FrameMaxValue;i++)
	{
		FrameBox=&FrameBoxList[i];
		if(FrameBox->id!=-1)
		{
			Frame=*FrameBox;
			printf("frameID:%d\ncreated!\n",Frame.id);
			printf("Title:%s\n",Frame.title);
			FrameFull(Frame.X,Frame.Y,Frame.X+Frame.width,Frame.Y+Frame.HeadHight,CYAN);
			//����ѡ��
			FrameFull(Frame.X,Frame.Y,Frame.X+Frame.HeadHight,Frame.Y+Frame.HeadHight,RED);
			FrameFull(Frame.X+Frame.HeadHight,Frame.Y,Frame.X+(Frame.HeadHight)*2,Frame.Y+Frame.HeadHight,BLUE);
			FrameFull(Frame.X+(Frame.HeadHight)*2,Frame.Y,Frame.X+(Frame.HeadHight)*3,Frame.Y+Frame.HeadHight,YELLOW);
			//������Ϣ
			FrameShowString(Frame.X+(Frame.HeadHight)*3,Frame.Y,200,16,16,(u8 *)Frame.title,BLACK);
			
			FrameVeiw(FrameBox);
			
			//���Ƶײ�
			//LCD_Fill(0,145,240,160,YELLOW);
			FrameFull(Frame.X,Frame.Y+Frame.height-Frame.BottomHight,Frame.X+Frame.width,Frame.Y+Frame.height,BROWN);
			
			FrameShowString(Frame.X+Frame.width/4,Frame.Y+Frame.height-Frame.BottomHight,200,16,16,(u8*)"ok",BLACK);
			FrameShowString(Frame.X+Frame.width/4*3,Frame.Y+Frame.height-Frame.BottomHight,200,16,16,(u8*)"cancel",BLACK);
			//���߿�
			FrameDrawLine(Frame.X,Frame.Y,Frame.X,Frame.Y+Frame.height,BLACK);
			FrameDrawLine(Frame.X,Frame.Y,Frame.X+Frame.width,Frame.Y,BLACK);
			FrameDrawLine(Frame.X+Frame.width,Frame.Y,Frame.X+Frame.width,Frame.Y+Frame.height,BLACK);
			FrameDrawLine(Frame.X,Frame.Y+Frame.height,Frame.X+Frame.width,Frame.Y+Frame.height,BLACK);
		}
	}
	
	/*
	Frame.X=x;
	Frame.Y=y;
	Frame.height=height;
	Frame.width=width;
	*/
	//����ͷ��
	/*
	ע��LCD_FillӦ�� ��Ҫһ���װ�����ڻ���ʱ���˷���Դ���ҿ��ܳ���
	*/
	//���ڵĶ���
	
	
	
	return isCreate;
}



void FrameClear(u16 Color)
{
	u32 index=0;      
	u32 totalpoint=240;
	totalpoint*=320; 	//?????
	FrameSetCursor(0x0000,0x0000);	//?????? 
	//LCD_WriteRAM_Prepare();     //????GRAM	 	
	BackgroundColor=Color;  
	for(index=0;index<totalpoint;index++)
	{
		FrameDrawPoint(Color);
	}
}
void FrameFull(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	   
	for(i=sy;i<=ey;i++)
	{									   
	 	FrameSetCursor(sx,i);      				//?????? 
		//LCD_WriteRAM_Prepare();     			//????GRAM	  
		for(j=0;j<xlen;j++)
		FrameDrawPoint(color);	//?????? 	    
	}
}

void FrameDrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //?????? 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //?????? 
	else if(delta_x==0)incx=0;//??? 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//??? 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //????????? 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//???? 
	{  
		FrameSetCursor(uRow,uCol);
		FrameDrawPoint(Color);//?? 
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

void FrameDrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 Color)
{
	FrameDrawLine(x1,y1,x2,y1,Color);
	FrameDrawLine(x1,y1,x1,y2,Color);
	FrameDrawLine(x1,y2,x2,y2,Color);
	FrameDrawLine(x2,y1,x2,y2,Color);
}
void FrameDrawCircle(u16 x0,u16 y0,u8 r,u16 Color)
{
	
	int a,b;
	int di;
	r*=2;
	a=0;b=r;	  
	di=3-(r<<1);             //??????????
	while(a<=b)
	{
		FrameSetCursor(x0+a,y0-b);
		FrameDrawPoint(Color); 		//5
		FrameSetCursor(x0+b,y0-a);
 		FrameDrawPoint(Color);             //0 
		FrameSetCursor(x0+b,y0+a);
		FrameDrawPoint(Color);             //4    
		FrameSetCursor(x0+a,y0+b);		
		FrameDrawPoint(Color);             //6 
		FrameSetCursor(x0-a,y0+b);
		FrameDrawPoint(Color);             //1   
		FrameSetCursor(x0-b,y0+a);    
 		FrameDrawPoint(Color);  
		FrameSetCursor(x0-a,y0-b);		
		FrameDrawPoint(Color);             //2  
		FrameSetCursor(x0-b,y0-a);		
  	FrameDrawPoint(Color);             //7   
		a++;
		//??Bresenham????     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
}
//ֱ�Ӳ���������ʾ
void FrameShowChar(u16 x,u16 y,u8 num,u8 size,u16 Color)
{  							  
  u8 temp,t1,t;
	u16 y0=y;      			     
	//????		   
	num=num-' ';//???????
	
	for(t=0;t<size;t++)
	 {   
				if(size==12)temp=asc2_1206[num][t];  //??1206??
				else temp=asc2_1608[num][t];		 //??1608?? 	                          
				for(t1=0;t1<8;t1++)
				{			    
						if(temp&0x80)
						{
							FrameSetCursor(x,y);
							FrameDrawPoint(Color);
						}
						 
						temp<<=1;
						y++;
						if(y>=320)return;//????
						if((y-y0)==size)
						{
							y=y0;
							x++;
							if(x>=240)return;//????
							break;
						}
				}
		}			
}   

void FrameShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u16 Color)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//?????????!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//??
        FrameShowChar(x,y,*p,size,Color);
        x+=size/2;
        p++;
    }  
}

/*******************MessageBox ***********************/
/***********************************************
date:2019 6 19
author:latefly
description:
version:0.0.01
this is basic model to show the message on the LCD screen.
20 messages max and 50 character max for each.



***********************************************/

#define Max_Message 16
#define Max_Character   30


//tips:last byte to mark is used.
char Message[Max_Message][Max_Character]={0};//Init array.
char StatePointer=0;
char LimitedTimes=10;//to show 10 message on screen onece.
char FormatSart[20]={0};


void ClearMessage()
{
		FrameClear(BLACK);
}

void ShowMessage()
{
		int location=0;
		location=(StatePointer-1)*20;
		FrameShowString(0,location,240,16,16,(u8 *)Message[StatePointer-1],WHITE);
		printf("StatePointer:%d\n",StatePointer);
}

char AddMessage(char *message)
{
    char i;
		if(StatePointer==0)ClearMessage();
	  printf("StatePointer:%d\nMessage:%s\n",StatePointer,message);
    for(i=0;i<Max_Character-1;i++)
    {
        Message[StatePointer][i]=*message;
        message++;
    }
    Message[StatePointer][i]=0x01;//mark used.
    StatePointer++;
    if(StatePointer==Max_Message)StatePointer=0;
    ShowMessage();
    return StatePointer;
}

/*



void FrameFont16(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi16;    
	temp+=index*32;	
	for(j=0;j<16;j++)
	{
		FrameSetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//????GRAM
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




void FrameFont32(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j,k;
	const unsigned char *temp=hanzi32;    
	temp+=index*128;	
	for(j=0;j<32;j++)
	{
		FrameSetCursor(x,y+j);
		LCD_WriteRAM_Prepare();	//????GRAM
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

//???????







*/

