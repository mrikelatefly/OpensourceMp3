#ifndef __SPI_H
#define __SPI_H
#include "sys.h"


void SPIInit(void);
void SPI2WriteByte(char data);
char SPI2ReadByte(void);
#endif

