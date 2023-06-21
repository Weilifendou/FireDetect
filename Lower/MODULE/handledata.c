#include "handledata.h"
#include "adc.h"
#include "dht11.h"
#include "warning.h"
#include "power.h"
#include "parameters.h"


void HandleData_Init(void) {
    
}

void HandleData(void) {
    u8 i = 0;
    float index = 0.0;
    
    float temp1, temp2, temp3, temp4, temp5;
    
    float tempWeight = SoftwareDataStructure.tempWeight / 1000.0;
    float humiWeight = SoftwareDataStructure.humiWeight / 1000.0;
    float lightWeight = SoftwareDataStructure.lightWeight / 1000.0;
    float rainWeight = SoftwareDataStructure.rainWeight / 1000.0;
    float fireWeight = SoftwareDataStructure.fireWeight / 1000.0;
    
    float tempMax = SoftwareDataStructure.tempMax * 10.0;
    float humiMax = SoftwareDataStructure.humiMax * 10.0;
    float lightMax = SoftwareDataStructure.lightMax;
    float rainMax = SoftwareDataStructure.rainMax;
    float fireMax = SoftwareDataStructure.fireMax;
    
    
    u16 upSum = 0;
    u16 downSum = 0;
    u16 leftSum = 0;
    u16 rightSum = 0;
    u16 rainSum = 0;
    u16 fireSum = 0;
    u16 batterySum = 0;
    u16 sum = 0;

    for (i = 0; i < ADC_DATA_LENGTH; i += 7)
    {
        upSum += ADC_Value[i + 0];
        downSum += ADC_Value[i + 1];
        leftSum += ADC_Value[i + 2];
        rightSum += ADC_Value[i + 3];
        rainSum += ADC_Value[i + 4];
        fireSum += ADC_Value[i + 5];
        batterySum += ADC_Value[i + 6];
    }
    
    DetectDataStructure.upLight = upSum / 10;
    DetectDataStructure.downLight = downSum / 10;
    DetectDataStructure.leftLight = leftSum / 10;
    DetectDataStructure.rightLight = rightSum / 10;
    
    DetectDataStructure.rainIndex = ADC_FULL_VALUE - rainSum / 10;
    DetectDataStructure.fireIndex = ADC_FULL_VALUE - fireSum / 10;
    DetectDataStructure.battery = batterySum / 10;
    
    sum = DetectDataStructure.upLight + DetectDataStructure.downLight
    + DetectDataStructure.leftLight + DetectDataStructure.rightLight;
    
    DetectDataStructure.light = ADC_FULL_VALUE - sum / TRACK_SENSERS;
    
    if (DetectDataStructure.battery / SoftwareDataStructure.fullBatteryADC >= 1) {
        DetectDataStructure.battery = HUNDRED;
    } else {
        DetectDataStructure.battery = DetectDataStructure.battery * HUNDRED / SoftwareDataStructure.fullBatteryADC;
    }
    
    DHT11_ReadData(&(DetectDataStructure.temperature), &(DetectDataStructure.humidty));
    
    temp1 = (DetectDataStructure.temperature + 400) / tempMax;
    temp2 = DetectDataStructure.humidty / humiMax;
    temp3 = DetectDataStructure.light / lightMax;
    temp4 = DetectDataStructure.rainIndex / rainMax;
    temp5 = DetectDataStructure.fireIndex / fireMax;
    
    index = tempWeight * temp1 - humiWeight * temp2 + lightWeight * temp3 - rainWeight * temp4 + fireWeight * temp5;
    DetectDataStructure.warnIndex = index * HUNDRED;
//    SysDataStructure.warnIndex = 79;
    
    if (DetectDataStructure.warnIndex < SoftwareDataStructure.greenWarn) {
        GreenAlarm();
    } else if (DetectDataStructure.warnIndex < SoftwareDataStructure.blueWarn) {
        BlueWarn();
    } else if (DetectDataStructure.warnIndex < SoftwareDataStructure.yellowWarn) {
        YellowWarn();
    } else if (DetectDataStructure.warnIndex < SoftwareDataStructure.redWarn) {
        RedWarn();
    } else {
        RedWarn();
    }
}



