#include "hc595.h"
#include "sys.h"
#include "delay.h"
#include "eventscan.h"
/*
管脚定义
12				RCK PB15 输出时钟
11				SCK PB14 输入时钟
14				SI  PB13 输入数据线
10				OE PB12

13				SCLR1-SCLR4	PB9-PB6

*/

void Pin_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
		//这里需要初始化4个使能端，以及三个输控制输出端
		//OE1-OE4	PB6-PB9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6; //初始化OE1-4
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	
		//RCK PB15 ;SCK PB14; SI  PB13; SCLR PB12;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
		
		//不使能所有的595
		GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);
		//输出拉低
		GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13);
		//使能输出
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		
}

void SentBit(char data)
{
	int i;
	delay_us(10);//等待电平稳定
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	for(i=0;i<8;i++)
	{
		if(data>>7)
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_13);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		}
		//产生上升沿 注意这里的速度可能会过快导致传输数据失败
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		delay_us(10);
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		data<<=1;
	}
	//产生上升沿输出数据
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	delay_us(10);
	//拉低所有的输出管脚
	GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13);
	delay_us(10);
}
void SentData(char n,char data)
{
	/*
	注意这里是否首先要清除原有的输出数据
	在这里将以前的数据进行了清零
	*/
	
	//	int i=0;
	//打开使能
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	/*
	//清零
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	i++;
	i--;//延时
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	//释放输出
	*/
	switch(n)
	{
		case 1:GPIO_SetBits(GPIOB,GPIO_Pin_9);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					 
			break;
		case 2:GPIO_SetBits(GPIOB,GPIO_Pin_8);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			break;
		case 3:GPIO_SetBits(GPIOB,GPIO_Pin_7);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
			break;
		case 4:GPIO_SetBits(GPIOB,GPIO_Pin_6);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			break;
	}
	SentBit(data);
}
void SentDataAll(char data)
{
	//int i=0;
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	//清零所有的输出
	SentBit(data);
}
void Hc595_Init(void)
{
	Pin_Init();
}

/*
如果这里的返回值是则应该注意在主函数的处理的逻辑思路并且在多次没有返回正确的
位置时做出相应的提示

*/
char Find(void)
{
	char i=0,local=0,j,data=1;;
	for(i=1;i<5;i++)
	{
			SentData(i,0xff);
			if(LOGICALIN)//判断逻辑输入的值
			{
				local=i;
				break;
			}
	}
	if(local==0)
		return 0;
	for(j=0;j<8;j++)
	{
		SentData(i,data);
		data<<=1;
		if(LOGICALIN)
		{
			break;
		}
	}
	switch(j)
	{
		case 0:j=1;break;
		case 1:j=8;break;
		case 2:j=7;break;
		case 3:j=6;break;
		case 4:j=5;break;
		case 5:j=4;break;
		case 6:j=3;break;
		case 7:j=2;break;
	}
	local=(i-1)*8+j;
	return local;
}




char Hc595_Dataprocess()
{
	//在这里应该先确定大体的位置然后再进行检测
	int sum=0;
	char local[10]={0},blocks[3]={0},r,i,j,data=0x01;
	
	//找到确定的位置
	for(i=0;i<3;i++)
	{
		for(j=0;j<4;j++)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			switch(j+1)
			{
				case 1:GPIO_SetBits(GPIOB,GPIO_Pin_9);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
							 
					break;
				case 2:GPIO_SetBits(GPIOB,GPIO_Pin_8);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					break;
				case 3:GPIO_SetBits(GPIOB,GPIO_Pin_7);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					break;
				case 4:GPIO_SetBits(GPIOB,GPIO_Pin_6);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
							 GPIO_ResetBits(GPIOB,GPIO_Pin_9);
					break;
			}
			SentBit(0xff);
			if(LOGICALIN)
			{
				blocks[i]=j;
				break;
			}
		}
	}
	if(blocks[0]==0&&blocks[1]==0&&blocks[2]==0)
	{
		return 0;
	}
	j=0;
	for(i=0;i<3;i++)
	{
		if(blocks[i])
		{
			j++;
		}
		blocks[0]+=blocks[i];
	}
	
	blocks[0]/=j;
	for(i=0;i<10;i++)
	{
		for(j=0;j<8;j++)
		{
			
			SentData(blocks[0],data);
			data<<=1;
			if(LOGICALIN)
			{
				local[i]=j;
				//数据处理
				switch(local[i])
				{
					case 0:local[i]=1;break;
					case 1:local[i]=8;break;
					case 2:local[i]=7;break;
					case 3:local[i]=6;break;
					case 4:local[i]=5;break;
					case 5:local[i]=4;break;
					case 6:local[i]=3;break;
					case 7:local[i]=2;break;
				}
				break;
			}
		}
	}
	j=0;
	for(i=0;i<20;i++)
	{
		if(local[i]!=0)
		{
			j++;
		}
		sum+=local[i];
	}
	local[0]=sum/j;
	r=blocks[0]*4+local[0];
	return r;
}


/*
char FindPosition(void)
{
	char i=0,position=0,local=0,progress;

	//这里可能会找不到小球的位置
	//但是这也可能是没有放置小球的结果
	//所以在主函数内要有一个识别小球是不是被放置的逻辑
	//这里也将有处理找不到小球的处理方法

	for(i=1;i<5;i++)
	{
			SentData(i,0xff);
			if(LOGICALIN)//判断逻辑输入的值
			{
				position=i;
				break;
			}
	}
	if(position==0)
		return 0;
	SentData(i,0x0f);
	position<<=3;
	if(LOGICALIN)
	{
			position|=(1<<2);
			SentData(i,0x03);
			if(LOGICALIN)
			{
						position|=(1<<1);
						SentData(i,0x01);
						if(LOGICALIN)
						{
							position|=1;
						}
						else
						{
							position|=0;
						}
			}
			else
			{
						position|=(0<<1);
						SentData(i,0x04);
						if(LOGICALIN)
						{
							position|=1;
						}
						else
						{
							position|=0;
						}
			}
	}
	else
	{
			position|=(0<<2);
			SentData(i,0x30);
			if(LOGICALIN)
			{
						position|=(1<<1);
						SentData(i,0x10);
						if(LOGICALIN)
						{
							position|=1;
						}
						else
						{
							position|=0;
						}
			}
			else
			{
						position|=(0<<1);
						SentData(i,0x04);
						if(LOGICALIN)
						{
							position|=1;
						}
						else
						{
							position|=0;
						}
			}
	}
	progress=position&0x38;
	progress>>=3;
	local=(progress-1)*8+(~(position&0x07))+1;
	return position&0x07;
}

char Hc595_Dataprocess(char p)
{
	char local[10],t=0,times=0,nowPosition=p;
	if(p==0)
	{
		return 0;//没有第一次位置记录
	}
	//调用十次取稳定的数据
	for(t=0;t<10;t++)
	{
		local[t]=Find();
		if(local[t]==0)
		{
			t--;
			times++;
			//防止跳不出来
			if(times>5)
			{
				break;
			}
		}
	}
	times=0;
	p=0;
	for(t=0;t<20;t++)
	{
		//排除不可能的位置
		if(local[t]-nowPosition>3||local[t]-nowPosition<-3)
		{
			//清零便于下一步的统计
			local[t]=0;
			times++;
		}
		//准备求均值
		p+=local[t];
	}
	nowPosition=p/(t-times);
	return nowPosition;
}

*/

