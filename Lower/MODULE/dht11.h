#ifndef __DHT11_H_
#define __DHT11_H_

#include "sys.h"

#define DHT PAin(8)
#define DHT_UP GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define DHT_DOWN GPIO_ResetBits(GPIOA, GPIO_Pin_8)

void DatLineOut(void);
void DatLineIn(void);


void DHT11_Init(void);
void DHT11_Start(void);

void DHT11_ReadData(u16* temp, u16* humidty) ;


#endif



