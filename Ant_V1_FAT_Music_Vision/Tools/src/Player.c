#include "Player.h"
#include "vs10xx.h"	 
#include "delay.h"
#include "Key.h"
#include "FrameBox.h"
#include "LCD_2_0.h"	
#include "malloc.h"
#include "string.h"
#include "exfuns.h"  
#include "ff.h"   
#include "flac.h"	
#include "usart.h"	
#include "StringOperation.h"


extern char StringOperation_Buff[5][50];
//���ڱ�������
int volume=220;


//��ʾ��Ŀ����
//index:��ǰ����
//total:���ļ���
void mp3_index_show(u16 index,u16 total)
{
//��ʾ��ǰ��Ŀ������,������Ŀ��
	StringInt(index);
	FrameShowString(0,20,40,16,16,(u8 *)"Now:",BLACK);
	FrameShowString(40,20,50,16,16,(u8 *)StringOperation_Buff[0],BLACK);
	StringInt(total);
	FrameShowString(90,20,40,16,16,(u8 *)"All:",BLACK);
	FrameShowString(130,20,50,16,16,(u8 *)StringOperation_Buff[0],BLACK);
//	LCD_ShowxNum(30+0,230,index,3,16,0X80);		//����
//	LCD_ShowChar(30+24,230,'/',16,0);
//	LCD_ShowxNum(30+32,230,total,3,16,0X80); 	//����Ŀ		
	printf("the music index:%d  \n",index);
}
//��ʾ��ǰ����
void mp3_vol_show(u8 vol)
{			    
	FrameShowString(0,0,50,16,16,(u8 *)"VOL:",BLACK);
	StringInt(vol);
	FrameShowString(50,0,50,16,16,(u8 *)StringOperation_Buff[0],BLACK);
	
	printf("The volume:%d  \n",vol);
}
u16 f_kbps=0;//�����ļ�λ��	
//��ʾ����ʱ��,������ ��Ϣ 
//lenth:�����ܳ���
void mp3_msg_show(u32 lenth)
{	
	static u16 playtime=0;//����ʱ����	     
 	u16 time=0;// ʱ�����
	u16 temp=0;	  
	if(f_kbps==0xffff)//δ���¹�
	{
		playtime=0;
		f_kbps=VS_Get_HeadInfo();	   //��ñ�����
	}	 	 
	time=VS_Get_DecodeTime(); //�õ�����ʱ��
	if(playtime==0)playtime=time;
	else if((time!=playtime)&&(time!=0))//1sʱ�䵽,������ʾ����
	{
		playtime=time;//����ʱ�� 	 				    
		temp=VS_Get_HeadInfo(); //��ñ�����	   				 
		if(temp!=f_kbps)
		{
			f_kbps=temp;//����KBPS	  				     
		}			 
		//��ʾ����ʱ��
		printf("played %d m %d s\n\n",time/60,time%60);
		
		
		StringInt(time/60);
		FrameShowString(0,40,30,16,16,(u8 *)StringOperation_Buff[0],BLACK);
		FrameShowString(30,40,20,16,16,(u8 *)"m",BLACK);
		FrameShowString(50,40,10,16,16,(u8 *)":",BLACK);
		StringInt(time%60);
		FrameShowString(70,40,30,16,16,(u8 *)StringOperation_Buff[0],BLACK);
		FrameShowString(100,40,50,16,16,(u8 *)"s",BLACK);
//		LCD_ShowxNum(30,210,time/60,2,16,0X80);		//����
//		LCD_ShowChar(30+16,210,':',16,0);
//		LCD_ShowxNum(30+24,210,time%60,2,16,0X80);	//����		
// 		LCD_ShowChar(30+40,210,'/',16,0);	    	 
		//��ʾ��ʱ��
		if(f_kbps)time=(lenth/f_kbps)/125;//�õ�������   (�ļ�����(�ֽ�)/(1000/8)/������=����������    	  
		else time=0;//�Ƿ�λ��	  
// 		LCD_ShowxNum(30+48,210,time/60,2,16,0X80);	//����
//		LCD_ShowChar(30+64,210,':',16,0);
//		LCD_ShowxNum(30+72,210,time%60,2,16,0X80);	//����	  		    
		//��ʾλ��	
		printf("Fre:%d fkbps\n\n",f_kbps);
		StringInt(f_kbps);
		FrameShowString(0,60,40,16,16,(u8 *)"Fre:",BLACK);
		FrameShowString(40,60,50,16,16,(u8 *)StringOperation_Buff[0],BLACK);
		FrameShowString(90,60,40,16,16,(u8 *)"fkps",BLACK);
		
//   	LCD_ShowxNum(30+110,210,f_kbps,3,16,0X80); 	//��ʾλ��	 
//		LCD_ShowString(30+134,210,200,16,16,"Kbps");	  	  
//		LED0=!LED0;		//DS0��ת
	}   		 
}			  		 
//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 mp3_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
	u8 *fn; 			 			   			     
  res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
  tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	    res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	    if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     	fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	myfree(SRAMIN,tfileinfo.lfname);
	return rval;
}


//��������
void mp3_play(void)
{
	u8 res;
 	DIR mp3dir;	 		//Ŀ¼
	FILINFO mp3fileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totmp3num; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *mp3indextbl;	//���������� 
	
	FrameClear(WHITE);
	delay_ms(10);
 	while(f_opendir(&mp3dir,"0:/MUSIC"))//��ͼƬ�ļ���
 	{	    
		printf("MUSIC�ļ��д���!\n\n");
		delay_ms(200);		  
	} 									  
	totmp3num=mp3_get_tnum((u8 *)"0:/MUSIC"); //�õ�����Ч�ļ���
  	while(totmp3num==NULL)//�����ļ�����Ϊ0		
 	{	    
		printf("û�������ļ�!");
		delay_ms(200);			  
	}										   
  mp3fileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	mp3fileinfo.lfname=mymalloc(SRAMIN,mp3fileinfo.lfsize);	//Ϊ���ļ������������ڴ�
 	pname=mymalloc(SRAMIN,mp3fileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
 	mp3indextbl=mymalloc(SRAMIN,2*totmp3num);				//����2*totmp3num���ֽڵ��ڴ�,���ڴ�������ļ�����
 	while(mp3fileinfo.lfname==NULL||pname==NULL||mp3indextbl==NULL)//�ڴ�������
 	{	    
		printf("�ڴ����ʧ��!");
		delay_ms(200);
	}  	
	VS_HD_Reset();
	VS_Soft_Reset();
	vsset.mvol=volume;						//Ĭ����������Ϊ220.
	mp3_vol_show((vsset.mvol-100)/5);	//����������:100~250,��ʾ��ʱ��,���չ�ʽ(vol-100)/5,��ʾ,Ҳ����0~30   
	//��¼����
    res=f_opendir(&mp3dir,"0:/MUSIC"); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=mp3dir.index;								//��¼��ǰindex
	        res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				mp3indextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}   
  curindex=0;											//��0��ʼ��ʾ
  res=f_opendir(&mp3dir,(const TCHAR*)"0:/MUSIC"); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&mp3dir,mp3indextbl[curindex]);			//�ı䵱ǰĿ¼����	   
    res=f_readdir(&mp3dir,&mp3fileinfo);       		//��ȡĿ¼�µ�һ���ļ�
    if(res!=FR_OK||mp3fileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
    fn=(u8*)(*mp3fileinfo.lfname?mp3fileinfo.lfname:mp3fileinfo.fname);
		/********��ӡ������**********/
		//AddMessage((char *)fn);
		
		FrameShowString(0,80,50,16,16,(u8 *)"Song:",BLACK);
		FrameShowString(0,100,178,16,16,(u8 *)fn,BLACK);
		mp3_vol_show((volume-100)/5);
		
		strcpy((char*)pname,"0:/MUSIC/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
 		//LCD_Fill(30,190,240,190+16,WHITE);				//���֮ǰ����ʾ
		
		mp3_index_show(curindex+1,totmp3num);
		key=mp3_play_song(pname); 				 		//�������MP3    
		if(key==2)		//��һ��
		{
			if(curindex)curindex--;
			else curindex=totmp3num-1;
 		}else if(key<=1)//��һ��
		{
			curindex++;		   	
			if(curindex>=totmp3num)curindex=0;//��ĩβ��ʱ��,�Զ���ͷ��ʼ
 		}else break;	//�����˴��� 	 
	} 											  
	myfree(SRAMIN,mp3fileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,mp3indextbl);			//�ͷ��ڴ�	 
}

//����һ��ָ���ĸ���				     	   									    	 
//����ֵ:0,�����������
//		 1,��һ��
//       2,��һ��
//       0XFF,���ִ�����
u8 mp3_play_song(u8 *pname)
{	 
 	FIL* fmp3;
   u16 br;
	u8 res,rval;
	u8 *databuf;
	u16 i=0; 
	u8 key;	   
	rval=0;	    
	fmp3=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	databuf=(u8*)mymalloc(SRAMIN,4096);		//����4096�ֽڵ��ڴ�����
	if(databuf==NULL||fmp3==NULL)rval=0XFF ;//�ڴ�����ʧ��.
	if(rval==0)
	{	  
	  VS_Restart_Play();  					//�������� 
		VS_Set_All();        					//������������Ϣ 			 
		VS_Reset_DecodeTime();					//��λ����ʱ�� 	  
		res=f_typetell(pname);	 	 			//�õ��ļ���׺	 			  	 						 
		if(res==0x4c)//�����flac,����patch
		{	
			VS_Load_Patch((u16*)vs1053b_patch,VS1053B_PATCHLEN);
		}  				 		   		 						  
		res=f_open(fmp3,(const TCHAR*)pname,FA_READ);//���ļ�	 
 		if(res==0)//�򿪳ɹ�.
		{ 
			VS_SPI_SpeedHigh();	//����						   
			while(rval==0)
			{
				res=f_read(fmp3,databuf,4096,(UINT*)&br);//����4096���ֽ�  
				i=0;
				do//������ѭ��
			    {  	
					if(VS_Send_MusicData(databuf+i)==0)//��VS10XX������Ƶ����
					{
						i+=32;
					}else   
					{
						key=GetKeyCilk();
						switch(key)
						{
							case KEY_Right:
								rval=1;		//��һ��
								break;
							case KEY_Left:
								rval=2;		//��һ�� 
							 break;
							case KEY_Up:
								if(volume<255)volume++;		//��һ��
								 VS_Set_Vol(volume);
								 mp3_vol_show((volume-100)/5);
							 break;
							case KEY_Down:
								if(volume>0)volume--;		//��һ��
								 VS_Set_Vol(volume);
								 mp3_vol_show((volume-100)/5);
							 break;
						}
						vsset.mvol=volume;
						mp3_msg_show(fmp3->fsize);//��ʾ��Ϣ	    
					}	    	    
				}while(i<4096);//ѭ������4096���ֽ� 
				if(br!=4096||res!=0)
				{
					rval=0;
					break;//������.		  
				} 							 
			}
			f_close(fmp3);
		}else rval=0XFF;//���ִ���	   	  
	}						     	 
	myfree(SRAMIN,databuf);	  	 		  	    
	myfree(SRAMIN,fmp3);
	return rval;	  	 		  	    
}

