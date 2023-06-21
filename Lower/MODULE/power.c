#include "power.h"
#include "oled.h"

void Power_Init(void) {
    
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
    ControlBluetooth5V(1);
}

//void BMS(u8 state) {
//    if (state) {
//        OLED_ON();
//        GPIO_SetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
//    } else {
//        OLED_OFF();
//        GPIO_ResetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
//        
//    }
//}

void ControlBluetooth3V(u8 state) {
    if (state) {
        BLUETOOTH_3V = 1;
    } else {
        BLUETOOTH_3V = 0;
    }

}

void ControlBluetooth5V(u8 state) {
    if (state) {
        BLUETOOTH_5V = 1;
    } else {
        BLUETOOTH_3V = 0;
    }

}

void ControlElevSteer(u8 state) {
    if (state) {
        ELEV_STEER = 1;
    } else {
        ELEV_STEER = 0;
    }

}
void ControlFlatSteer(u8 state) {
    if (state) {
        FLAT_STEER = 1;
    } else {
        FLAT_STEER = 0;
    }

}


