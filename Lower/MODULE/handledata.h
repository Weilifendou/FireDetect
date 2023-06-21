#ifndef __HANDLEDATA_H
#define __HANDLEDATA_H
#include "sys.h"

#define BUZZER PBout(5)
#define GREEN_LED PBout(12)
#define BLUE_LED PBout(14)
#define YELLOW_LED PBout(13)
#define RED_LED PBout(15)


void HandleData_Init(void);
void HandleData(void);

#endif



