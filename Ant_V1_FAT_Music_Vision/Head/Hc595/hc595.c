#include "hc595.h"
#include "sys.h"
#include "delay.h"
#include "eventscan.h"
/*
�ܽŶ���
12				RCK PB15 ���ʱ��
11				SCK PB14 ����ʱ��
14				SI  PB13 ����������
10				OE PB12

13				SCLR1-SCLR4	PB9-PB6

*/

void Pin_Init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
		//������Ҫ��ʼ��4��ʹ�ܶˣ��Լ���������������
		//OE1-OE4	PB6-PB9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6; //��ʼ��OE1-4
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
	
		//RCK PB15 ;SCK PB14; SI  PB13; SCLR PB12;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12;
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
		
		//��ʹ�����е�595
		GPIO_SetBits(GPIOB,GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);
		//�������
		GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13);
		//ʹ�����
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		
}

void SentBit(char data)
{
	int i;
	delay_us(10);//�ȴ���ƽ�ȶ�
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
		//���������� ע��������ٶȿ��ܻ���쵼�´�������ʧ��
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		delay_us(10);
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		data<<=1;
	}
	//�����������������
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
	delay_us(10);
	//�������е�����ܽ�
	GPIO_ResetBits(GPIOB,GPIO_Pin_15|GPIO_Pin_14|GPIO_Pin_13);
	delay_us(10);
}
void SentData(char n,char data)
{
	/*
	ע�������Ƿ�����Ҫ���ԭ�е��������
	�����ｫ��ǰ�����ݽ���������
	*/
	
	//	int i=0;
	//��ʹ��
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
	/*
	//����
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	i++;
	i--;//��ʱ
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	//�ͷ����
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
	//�������е����
	SentBit(data);
}
void Hc595_Init(void)
{
	Pin_Init();
}

/*
�������ķ���ֵ����Ӧ��ע�����������Ĵ�����߼�˼·�����ڶ��û�з�����ȷ��
λ��ʱ������Ӧ����ʾ

*/
char Find(void)
{
	char i=0,local=0,j,data=1;;
	for(i=1;i<5;i++)
	{
			SentData(i,0xff);
			if(LOGICALIN)//�ж��߼������ֵ
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
	//������Ӧ����ȷ�������λ��Ȼ���ٽ��м��
	int sum=0;
	char local[10]={0},blocks[3]={0},r,i,j,data=0x01;
	
	//�ҵ�ȷ����λ��
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
				//���ݴ���
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

	//������ܻ��Ҳ���С���λ��
	//������Ҳ������û�з���С��Ľ��
	//��������������Ҫ��һ��ʶ��С���ǲ��Ǳ����õ��߼�
	//����Ҳ���д����Ҳ���С��Ĵ�����

	for(i=1;i<5;i++)
	{
			SentData(i,0xff);
			if(LOGICALIN)//�ж��߼������ֵ
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
		return 0;//û�е�һ��λ�ü�¼
	}
	//����ʮ��ȡ�ȶ�������
	for(t=0;t<10;t++)
	{
		local[t]=Find();
		if(local[t]==0)
		{
			t--;
			times++;
			//��ֹ��������
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
		//�ų������ܵ�λ��
		if(local[t]-nowPosition>3||local[t]-nowPosition<-3)
		{
			//���������һ����ͳ��
			local[t]=0;
			times++;
		}
		//׼�����ֵ
		p+=local[t];
	}
	nowPosition=p/(t-times);
	return nowPosition;
}

*/

