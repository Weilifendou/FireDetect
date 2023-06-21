#include "debugger.h"
#include "parameters.h"
#include "warning.h"
#include "power.h"
#include "oled.h"

u8 DebugMode;
void Iwdg_Init(void)
{
    //1、 取消寄存器写保护：
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    //2、设置独立看门狗的预分频系数，确定时钟:125HZ
    IWDG_SetPrescaler(IWDG_Prescaler_256);
    //3、设置看门狗重装载值，确定溢出时间:2s
    IWDG_SetReload(250);
    //4、使能看门狗
    IWDG_Enable();
    
    IWDG_ReloadCounter();

}

void HandWareDebug(void) {
    
    if ((HandwareDataStructure.WarnControl & 0x01) == 0x01) {
        GREEN_LED = 0;
    } else {
        GREEN_LED = 1;
    }
    if ((HandwareDataStructure.WarnControl & 0x02) == 0x02) {
        BLUE_LED = 0;
    } else {
        BLUE_LED = 1;
    }
    if ((HandwareDataStructure.WarnControl & 0x04) == 0x04) {
        YELLOW_LED = 0;
    } else {
        YELLOW_LED = 1;
    }
    if ((HandwareDataStructure.WarnControl & 0x08) == 0x08) {
        RED_LED = 0;
    } else {
        RED_LED = 1;
    }
    if ((HandwareDataStructure.WarnControl & 0x10) == 0x10) {
        BUZZER = 1;
    } else {
        BUZZER = 0;
    }

    if ((HandwareDataStructure.PowerControl & 0x01) == 0x01) {
        ControlBluetooth5V(1);
    } else {
        ControlBluetooth5V(0);
    }
    if ((HandwareDataStructure.PowerControl & 0x02) == 0x02) {
        ControlBluetooth3V(1);
    } else {
        ControlBluetooth3V(0);
    }
    if ((HandwareDataStructure.PowerControl & 0x04) == 0x04) {
        ControlElevSteer(1);
    } else {
        ControlElevSteer(0);
    }
    if ((HandwareDataStructure.PowerControl & 0x08) == 0x08) {
        ControlFlatSteer(1);
    } else {
        ControlFlatSteer(0);
    }
    TIM_SetCompare3(TIM3, HandwareDataStructure.elevPWM);
    TIM_SetCompare4(TIM3, HandwareDataStructure.flatPWM); //最小50最大为200
    OLED_Fill(0x00);
    OLED_ShowStr(HandwareDataStructure.screenX, HandwareDataStructure.screenY,
    HandwareDataStructure.str, HandwareDataStructure.textSize);
}






