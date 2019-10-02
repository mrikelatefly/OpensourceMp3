#include "MicroSD.h" 
#include "usart.h"	 
#include "SPI.h"

//�����ڴ濨��Ϣ�����ڳ�ʼ����ʱ��ͽ��г�ʼ��
__MicroSDInfo MicroSDInfo={0};

/*
PB0 CS
PA5 SCK
PA6 MISO
PA7 MOSI
SPIģʽ��ʼ������Ժ�Ͳ���ҪcrcУ����
*/
#define MicroSD_CS_SET GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define MicroSD_CS_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_0)


char MicroSDDataBuff[512]={0};//һ�������Ĵ�С


void MicroSDGPIOInit(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB0
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
}


/*********************************һ���ǻ��������ĸ�д*********************************************/
void MicroSDDisSelect(void)
{
	MicroSD_CS_SET;
  SPI1_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
}
//�ȴ���׼����
//����ֵ:0,׼������;����,�������
u8 MicroSDWaitReady(void)
{
	u32 t=0;
	do
	{
		if(SPI1_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0xFFFF);//�ȴ� 
	return 1;
}
//����ֵ:0,�ɹ�;1,ʧ��;
u8 MicroSDSelect(void)
{
	MicroSD_CS_RESET;
	if(MicroSDWaitReady()==0)return 0;//�ȴ��ɹ�
	MicroSDDisSelect();
	return 1;//�ȴ�ʧ��
}
//�ȴ�SD����Ӧ
//Response:Ҫ�õ��Ļ�Ӧֵ
//����ֵ:0,�ɹ��õ��˸û�Ӧֵ
u8 MicroSDGetResponse(u8 Response)
{
	u16 Count=0xFFFF;//�ȴ�����	   						  
	while ((SPI1_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}
//��sd����ȡһ�����ݰ�������
//buf:���ݻ�����
//len:Ҫ��ȡ�����ݳ���.
//����ֵ:0,�ɹ�;����,ʧ��;	
u8 MicroSDRecvData(u8*buf,u16 len)
{			  	  
	if(MicroSDGetResponse(0xFE))return 1;//�ȴ�SD������������ʼ����0xFE
    while(len--)//��ʼ��������
    {
        *buf=SPI1_ReadWriteByte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    SPI1_ReadWriteByte(0xFF);
    SPI1_ReadWriteByte(0xFF);									  					    
    return 0;//��ȡ�ɹ�
}
//��sd��д��һ�����ݰ������� 512�ֽ�
//buf:���ݻ�����
//cmd:ָ��
//����ֵ:0,�ɹ�;����,ʧ��;	
u8 MicroSDSendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	if(MicroSDWaitReady())return 1;//�ȴ�׼��ʧЧ
	
	SPI1_ReadWriteByte(cmd);
	if(cmd!=0XFD)//���ǽ���ָ��
	{
		for(t=0;t<512;t++)SPI1_ReadWriteByte(buf[t]);//����ٶ�,���ٺ�������ʱ��
	    SPI1_ReadWriteByte(0xFF);//����crc
	    SPI1_ReadWriteByte(0xFF);
			t=SPI1_ReadWriteByte(0xFF);//������Ӧ
			if((t&0x1F)!=0x05)
				return 2;//��Ӧ����									  					    
	}						 									  					    
    return 0;//д��ɹ�
}

//��SD������һ������
//����: u8 cmd   ���� 
//      u32 arg  �������
//      u8 crc   crcУ��ֵ	   
//����ֵ:SD�����ص���Ӧ															  
u8 MicroSDSendCmd(u8 cmd, u32 data, u8 crc)
{
  int result=0,times=0;
	
	MicroSD_CS_SET;//��ֹSD��Ƭѡ ͬ��ʱ��
	SPI1_ReadWriteByte(0xff);   
	MicroSD_CS_RESET;//��ʼ����
	SPI1_ReadWriteByte(cmd|0x40);
	for(times=0;times<4;times++)    
	{    
      SPI1_ReadWriteByte((data>>24)&0xff);
			data<<=8;
	}

  SPI1_ReadWriteByte(crc);
	SPI1_ReadWriteByte(0xff); //�˸�ʱ�� 
	times=0;
	do    
	{  //��ȡ��8λ   
		result = SPI1_ReadWriteByte(0xFF);   
		times++;   
	}
	while((result==0xff)&&(times<200));
	return (char)result;
}






//��ʼ��SD��
char MicroSDInit(void)
{
  u8 r1;      // ���SD���ķ���ֵ
  u16 retry;  // �������г�ʱ����
  u8 buf[4];  
	u16 i;
	u32 SDSize=0;
	MicroSDGPIOInit();
	MicroSD_CS_SET;
 	for(i=0;i<100;i++)SPI1_ReadWriteByte(0XFF);//��������74������
	retry=20;
	do
	{
		r1=MicroSDSendCmd(CMD0,0,0x95);//����IDLE״̬
	}while((r1!=0X01) && retry--);
	
	if(r1==0X01)
	{
		if(MicroSDSendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SPI1_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					MicroSDSendCmd(CMD55,0,0X01);	//����CMD55
					r1=MicroSDSendCmd(CMD41,0x40000000,0X01);//����CMD41
				}while(r1&&retry--);
				if(retry&&MicroSDSendCmd(CMD58,0,0X01)==0)//����SD2.0���汾��ʼ
				{
					for(i=0;i<4;i++)buf[i]=SPI1_ReadWriteByte(0XFF);//�õ�OCRֵ
					if(buf[0]&0x40)MicroSDInfo.MicroSDType=SD_TYPE_V2HC;    //���CCS
					else MicroSDInfo.MicroSDType=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			MicroSDSendCmd(CMD55,0,0X01);		//����CMD55
			r1=MicroSDSendCmd(CMD41,0,0X01);	//����CMD41
			if(r1<=1)
			{		
				MicroSDInfo.MicroSDType=SD_TYPE_V1;
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{
					MicroSDSendCmd(CMD55,0,0X01);	//����CMD55
					r1=MicroSDSendCmd(CMD41,0,0X01);//����CMD41
				}while(r1&&retry--);
			}else//MMC����֧��CMD55+CMD41ʶ��
			{
				MicroSDInfo.MicroSDType=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{											    
					r1=MicroSDSendCmd(CMD1,0,0X01);//����CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||MicroSDSendCmd(CMD16,512,0X01)!=0)MicroSDInfo.MicroSDType=SD_TYPE_ERR;//����Ŀ�
		}
	}
	MicroSDDisSelect();//ȡ��Ƭѡ
  /*****************************������Ҫ���ڴ濨����Ϣ��ȡ����*******************************/
	
	SDSize=MicroSDGetSectorCount();
	printf("MicroSD init completed the Size is:%d       ",SDSize);
	printf("MicroSD type is:%d       ",MicroSDInfo.MicroSDType);
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);
	if(MicroSDInfo.MicroSDType)return 0;
	else if(r1)return r1; 	   
	return 0xaa;//��������
}







//��SD��
//buf:���ݻ�����
//sector:����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
u8 MicroSDReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(MicroSDInfo.MicroSDType!=SD_TYPE_V2HC)sector <<= 9;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=MicroSDSendCmd(CMD17,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=MicroSDRecvData(buf,512);//����512���ֽ�	   
		}
	}else
	{
		r1=MicroSDSendCmd(CMD18,sector,0X01);//����������
		do
		{
			r1=MicroSDRecvData(buf,512);//����512���ֽ�	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		MicroSDSendCmd(CMD12,0,0X01);	//����ֹͣ����
	}   
	MicroSDDisSelect();//ȡ��Ƭѡ
	return r1;//
}
//дSD��
//buf:���ݻ�����
//sector:��ʼ����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
u8 MicroSDWriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(MicroSDInfo.MicroSDType!=SD_TYPE_V2HC)sector *= 512;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=MicroSDSendCmd(CMD24,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=MicroSDSendBlock(buf,0xFE);//д512���ֽ�	   
		}
	}else
	{
		if(MicroSDInfo.MicroSDType!=SD_TYPE_MMC)
		{
			MicroSDSendCmd(CMD55,0,0X01);	
			MicroSDSendCmd(CMD23,cnt,0X01);//����ָ��	
		}
 		r1=MicroSDSendCmd(CMD25,sector,0X01);//����������
		if(r1==0)
		{
			do
			{
				r1=MicroSDSendBlock(buf,0xFC);//����512���ֽ�	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=MicroSDSendBlock(0,0xFD);//����512���ֽ� 
		}
	}   
	MicroSDDisSelect();//ȡ��Ƭѡ
	return r1;//
}	






/***************************************************��ȡ��Ϣ�ķ�����*********************************************************/

//��ȡSD����CID��Ϣ��������������Ϣ
//����: u8 *cid_data(���CID���ڴ棬����16Byte��	  
//����ֵ:0��NO_ERR
//		 1������														   
u8 MicroSDGetCID(u8 *cid_data)
{
    u8 r1;	   
    //��CMD10�����CID
    r1=MicroSDSendCmd(CMD10,0,0x01);
    if(r1==0x00)
		{
			r1=MicroSDRecvData(cid_data,16);//����16���ֽڵ�����	 
    }
	MicroSDDisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}																				  
//��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
//����:u8 *cid_data(���CID���ڴ棬����16Byte��	    
//����ֵ:0��NO_ERR
//		 1������														   
u8 MicroSDGetCSD(u8 *csd_data)
{
    u8 r1;	 
    r1=MicroSDSendCmd(CMD9,0,0x01);//��CMD9�����CSD
    if(r1==0)
	{
    	r1=MicroSDRecvData(csd_data, 16);//����16���ֽڵ����� 
    }
	MicroSDDisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}  
//��ȡSD����������������������   
//����ֵ:0�� ȡ�������� 
//       ����:SD��������(������/512�ֽ�)
//ÿ�������ֽ�����Ϊ512����Ϊ�������512�����ʼ������ͨ��.														  
u32 MicroSDGetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
		u16 csize;  					    
	//ȡCSD��Ϣ������ڼ��������0
    if(MicroSDGetCSD(csd)!=0) return 0;	    
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)	 //V2.00�Ŀ�
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;//�õ�������	 		   
    }else//V1.XX�Ŀ�
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9);//�õ�������   
    }
    return Capacity;
}




