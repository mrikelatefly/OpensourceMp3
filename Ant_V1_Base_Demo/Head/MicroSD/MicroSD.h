#ifndef __SDIO_SDCARD_H
#define __SDIO_SDCARD_H																			   
#include "sys.h"

char SD_Init(void);

char WriteSectorToMicroSD(long addr,char *buff);
char ReadSectorFromMicroSD(long sector,char *buffer);






#endif 





