#ifndef __POWER_H
#define __POWER_H
#include "sys.h"  


#define BLUETOOTH_3V PBout(8)
#define BLUETOOTH_5V PBout(9)
#define ELEV_STEER PBout(10)
#define FLAT_STEER PBout(11)

void Power_Init(void);
void BMS(u8 state);
void ControlBluetooth3V(u8 state);
void ControlBluetooth5V(u8 state);
void ControlElevSteer(u8 state);
void ControlFlatSteer(u8 state);

#endif
