#include "SPI.h"
#include "stm32f10x_spi.h"


/*
#define CS_Set GPIO_SetBits(GPIOX,GPIO_Pin_x)
#define CS_Reset GPIO_ResetBits(GPIOX,GPIO_Pin_x)

#define MOSI_Set GPIO_SetBits(GPIOX,GPIO_Pin_x)
#define MOSI_Reset GPIO_ResetBits(GPIOX,GPIO_Pin_x)

#define MISO_Get GPIO_ReadBits(GPIOX,GPIO_Pin_x)

#define CLCK_High GPIO_SetBits(GPIOX,GPIO_Pin_x)
#define CLCK_Low GPIO_ResetBits(GPIOX,GPIO_Pin_x)

#define LOGICALIN GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//逻辑输入
*/



#define CS_Set GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define CS_Reset GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define MOSI_Set GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define MOSI_Reset GPIO_ResetBits(GPIOB,GPIO_Pin_9)

#define MISO_Set GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define MISO_Reset GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define CLCK_High GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define CLCK_Low GPIO_ResetBits(GPIOB,GPIO_Pin_11)

#define MOSI GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//逻辑输入
#define CS GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)//逻辑输入
#define CLCK GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)//逻辑输入

void delay(int a)
{
	int x=100;
	while(a--)
	{
		x=100;
		while(x--);
	}
		
}

void SPI_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	//RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_10 |GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB8-10推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
}


void SPIInit(void)
{
	
	SPI_GPIO_Init();
	SPI2WriteByte(0xff);//启动传输		 
 

}
void SPI2WriteByte(char data)//启动传输	
{
	char couter=0;
	CS_Reset;
	CLCK_High;
	for(couter=0;couter<8;couter++)
	{
		CLCK_Low;
		if(data>>7&0x01)
		{
			MOSI_Set;
		}
		else
		{
			MOSI_Reset;
		}
		delay(10);//wait signal statable
		data<<=1;
		CLCK_High;
		delay(100);
	}
	CLCK_High;
	CS_Set;
}

char SPI2ReadByte()//启动传输	
{
	char in=0xff,time=0;
	char couter=0;
	while(CS)//wait signal
	{
		time++;
		delay(10);
		if(time>20)return 0;
	}
	for(couter=0;couter<8;couter++)
	{
		in<<=1;
		while(CLCK)
		{
				while(CS)//wait signal and wait limit
				{
					time++;
					delay(10);
					if(time>20)return 0;
				}
		}
		in|=MOSI;
		delay(10);
	}
	
	return in;
}

/*
//in this position I use GPIO to insted spi interface to finish this work.



void SPI_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13| GPIO_Pin_15 |GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	
}

void SPI2_Init(void)
{
	
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_GPIO_Init();
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_12);  //PB13/14/15上拉
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输		 
 

}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 
	
} 


u8 SPI2_ReadWriteByte(u8 Byte)
{
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPI2->DR=Byte;	 	//发送一个byte   
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte  
	return SPI2->DR;          	     //返回收到的数据			
}



//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}

*/
