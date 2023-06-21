#include "warning.h"

u8 WarnTime;
u8 WarnSpeed;

void Warning_Init(void) {
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    GPIO_SetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void GreenAlarm(void) {

    GREEN_LED = 0;
    BLUE_LED = 1;
    YELLOW_LED = 1;
    RED_LED = 1;
    BUZZER = 0;
    WarnSpeed = 0;
    WarnTime = 0;
}

void BlueWarn(void) {

    GREEN_LED = 1;
    BLUE_LED = 0;
    YELLOW_LED = 1;
    RED_LED = 1;
    WarnSpeed = 50;
    WarnTime = 2;
}

void YellowWarn(void) {

    GREEN_LED = 1;
    BLUE_LED = 1;
    YELLOW_LED = 0;
    RED_LED = 1;
    WarnSpeed = 10;
    WarnTime = 5;
}

void RedWarn(void) {

    GREEN_LED = 1;
    BLUE_LED = 1;
    YELLOW_LED = 1;
    WarnSpeed = 2;
    WarnTime = 1;
}


