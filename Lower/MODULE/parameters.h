#ifndef __PARAMETERS_H
#define __PARAMETERS_H
#include "sys.h"

#define ADC_FULL_VALUE 4096
#define HUNDRED 100
#define BIT_16 65535


#define UP_LIGHT_CHANNEL 1
#define DOWN_LIGHT_CHANNEL 2
#define LEFT_LIGHT_CHANNEL 3
#define RIGHT_LIGHT_CHANNEL 4

#define RAIN_LIGHT_CHANNEL 5
#define FIRE_LIGHT_CHANNEL 6
#define BATTERY_LIGHT_CHANNEL 7

#define TRACK_SENSERS 4
#define ELEV_MAX_PWM 135
#define ELEV_MIN_PWM 75

#define FLAT_MAX_PWM 250
#define FLAT_MIN_PWM 80


typedef struct {
    u16 temperature;
    u16 humidty;
    u16 upLight;
    u16 downLight;
    u16 leftLight;
    u16 rightLight;
    u16 light;
    u16 rainIndex;
    u16 fireIndex;
    u16 smokeIndex;
    u16 warnIndex;
    u16 battery;
} DetectDataTypeDef;

typedef struct {
    u8 WarnControl;
    u8 PowerControl;
    u8 elevPWM;  //舵机上下的占空比设置量
    u8 flatPWM;  //舵机左右的占空比设置量
    u8 screenX;     //OLED显示屏X坐标
    u8 screenY;     //OLED显示屏Y坐标
    u8 textSize;    //字符大小设置
    char str[20];     //显示的字符串
} HandwareDataTypeDef;


typedef struct {
    u8 page;         //OLED显示的页面
    u16 fullBatteryADC; //满电量的ADC值
    u16 lightThreshold;
    u16 lightSense;  //光敏灵敏度
    u16 offset;       //舵机转动的步进量
    u8 trackADCTimes;
    u8 handleADCTimes;
    u16 tempWeight;
    u16 humiWeight;
    u16 lightWeight;
    u16 rainWeight;
    u16 fireWeight;
    u16 tempMax;
    u16 humiMax;
    u16 lightMax;
    u16 rainMax;
    u16 fireMax;
    u8 greenWarn;
    u8 blueWarn;
    u8 yellowWarn;
    u8 redWarn;
    
} SoftwareDataTypeDef;

extern DetectDataTypeDef DetectDataStructure;
extern HandwareDataTypeDef HandwareDataStructure;
extern SoftwareDataTypeDef SoftwareDataStructure;


void Parameter_Init(void);
void WriteHandwareDataToFlash(void);
void WriteSoftwareDataToFlash(void);



#endif
