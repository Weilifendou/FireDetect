#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

void TIM3_PWM_Init(u16 arr,u16 psc);
void GeneratePWM(void);
void SetPWM(u8 value, u8 which);

#endif


