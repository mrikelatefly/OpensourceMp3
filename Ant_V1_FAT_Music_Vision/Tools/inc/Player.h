#ifndef __PLAYER_H
#define __PLAYER_H
#include "sys.h"
/*******************************************
ʱ�䣺2019 10 1 
���ߣ�latefly
���ܣ�
1.ʵ��0��/musicĿ¼�¶�������Ĳ���
2.ʵ��һ�����Ž���
********************************************/




void mp3_play(void);
u16 mp3_get_tnum(u8 *path);
u8 mp3_play_song(u8 *pname);
void mp3_index_show(u16 index,u16 total);
void mp3_msg_show(u32 lenth);
#endif




