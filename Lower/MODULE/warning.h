#ifndef __WARNING_H
#define __WARNING_H
#include "sys.h"

#define BUZZER PBout(5)
#define GREEN_LED PBout(12)
#define BLUE_LED PBout(14)
#define YELLOW_LED PBout(13)
#define RED_LED PBout(15)


extern u8 WarnTime;
extern u8 WarnSpeed;

void Warning_Init(void);
void GreenAlarm(void);
void BlueWarn(void);
void YellowWarn(void);
void RedWarn(void);

#endif


