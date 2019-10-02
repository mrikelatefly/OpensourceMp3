#ifndef __PLAYER_H
#define __PLAYER_H
#include "sys.h"
/*******************************************
时间：2019 10 1 
作者：latefly
功能：
1.实现0：/music目录下多个歌曲的播放
2.实现一个播放界面
********************************************/




void mp3_play(void);
u16 mp3_get_tnum(u8 *path);
u8 mp3_play_song(u8 *pname);
void mp3_index_show(u16 index,u16 total);
void mp3_msg_show(u32 lenth);
#endif




