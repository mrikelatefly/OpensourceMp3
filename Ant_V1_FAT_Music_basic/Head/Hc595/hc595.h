#ifndef __HC595_H
#define __HC595_H
/*ע������ĹܽŵĶ���
�ܽŶ���
12				RCK PB15 ���ʱ��
11				SCK PB14 ����ʱ��
14				SI  PB13 ����������
10				OE PB12

13				SCLR1-SCLR4	PB9-PB6
*/

void Pin_Init(void);
void SentBit(char data);	
void SentData(char n,char data);
void Hc595_Init(void);
//char FindPosition(void);
void SentDataAll(char data);
char Find(void);
char Hc595_Dataprocess(void);
#endif


