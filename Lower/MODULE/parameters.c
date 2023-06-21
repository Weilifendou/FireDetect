#include "parameters.h"
#include "stmflash.h"


DetectDataTypeDef DetectDataStructure;
HandwareDataTypeDef HandwareDataStructure;
SoftwareDataTypeDef SoftwareDataStructure;

void Parameter_Init(void) {
    u8 i = 0;
    u8 j = 0;
    u8 index = 0;
    u16 flashBuff[40] = {0};
    
    
    STMFLASH_Read(DATA_START_ADDR, flashBuff, 40);
    
    if (flashBuff[index] == BIT_16) {
        HandwareDataStructure.WarnControl = 0x00;
        HandwareDataStructure.PowerControl = 0x00;
    } else {
        HandwareDataStructure.WarnControl = flashBuff[index] >> 8;
        HandwareDataStructure.PowerControl = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        HandwareDataStructure.elevPWM = 90;
        HandwareDataStructure.flatPWM = 110;
    } else {
        HandwareDataStructure.elevPWM = flashBuff[index] >> 8;
        HandwareDataStructure.flatPWM = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        HandwareDataStructure.screenX = 20;
        HandwareDataStructure.screenY = 3;
    } else {
        HandwareDataStructure.screenX = flashBuff[index] >> 8;
        HandwareDataStructure.screenY = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        HandwareDataStructure.textSize = 2;
    } else {
        HandwareDataStructure.textSize = flashBuff[index];
    }
    index++;
    for(i = 0; i < 20; i += 2) {
        HandwareDataStructure.str[i] = flashBuff[index+j] >> 8;
        HandwareDataStructure.str[i+1] = flashBuff[index+j];
        j++;
    }
    index += j;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.page = 0;
    } else {
        SoftwareDataStructure.page = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.fullBatteryADC = 3050;
    } else {
        SoftwareDataStructure.fullBatteryADC = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.lightThreshold = 200;
    } else {
        SoftwareDataStructure.lightThreshold = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.lightSense = 250;
    } else {
        SoftwareDataStructure.lightSense = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.offset = 5;
    } else {
        SoftwareDataStructure.offset = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.trackADCTimes = 3;
        SoftwareDataStructure.handleADCTimes = 3;
    } else {
        SoftwareDataStructure.trackADCTimes = flashBuff[index] >> 8;
        SoftwareDataStructure.handleADCTimes = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.tempWeight = 200;
    } else {
        SoftwareDataStructure.tempWeight = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.humiWeight = 500;
    } else {
        SoftwareDataStructure.humiWeight = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.lightWeight = 200;
    } else {
        SoftwareDataStructure.lightWeight = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.rainWeight = 800;
    } else {
        SoftwareDataStructure.rainWeight = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.fireWeight = 1500;
    } else {
        SoftwareDataStructure.fireWeight = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.tempMax = 800;
    } else {
        SoftwareDataStructure.tempMax = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.humiMax = 1000;
    } else {
        SoftwareDataStructure.humiMax = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.lightMax = 4096;
    } else {
        SoftwareDataStructure.lightMax = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.rainMax = 3000;
    } else {
        SoftwareDataStructure.rainMax = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.fireMax = 2000;
    } else {
        SoftwareDataStructure.fireMax = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.greenWarn = 25;
        SoftwareDataStructure.blueWarn = 50;
    } else {
        SoftwareDataStructure.greenWarn = flashBuff[index] >> 8;
        SoftwareDataStructure.blueWarn = flashBuff[index];
    }
    index++;
    if (flashBuff[index] == BIT_16) {
        SoftwareDataStructure.yellowWarn = 75;
        SoftwareDataStructure.redWarn = 100;
    } else {
        SoftwareDataStructure.yellowWarn = flashBuff[index] >> 8;
        SoftwareDataStructure.redWarn = flashBuff[index];
    }

}


void WriteHandwareDataToFlash(void) {
    u8 i = 0;
    u8 j = 0;
    u16 flashBuff[14] = {0};
    flashBuff[0] = HandwareDataStructure.WarnControl;
    flashBuff[0] <<= 8;
    flashBuff[0] |= HandwareDataStructure.PowerControl;
    flashBuff[1] = HandwareDataStructure.elevPWM;
    flashBuff[1] <<= 8;
    flashBuff[1] |= HandwareDataStructure.flatPWM;
    flashBuff[2] = HandwareDataStructure.screenX;
    flashBuff[2] <<= 8;
    flashBuff[2] |= HandwareDataStructure.screenY;
    flashBuff[3] = HandwareDataStructure.textSize;
    for (i = 0; i < 20; i += 2) {
        flashBuff[4+j] = HandwareDataStructure.str[i];
        flashBuff[4+j] <<= 8;
        flashBuff[4+j] |= HandwareDataStructure.str[i+1];
        j++;
    }
    STMFLASH_Write(DATA_START_ADDR, flashBuff, 14);
    
}


void WriteSoftwareDataToFlash(void) {
    u16 flashBuff[18] = {0};
    flashBuff[0] = SoftwareDataStructure.page;
    flashBuff[1] = SoftwareDataStructure.fullBatteryADC;
    flashBuff[2] = SoftwareDataStructure.lightThreshold;
    flashBuff[3] = SoftwareDataStructure.lightSense;
    flashBuff[4] = SoftwareDataStructure.offset;
    flashBuff[5] = SoftwareDataStructure.trackADCTimes;
    flashBuff[5] <<= 8;
    flashBuff[5] |= SoftwareDataStructure.handleADCTimes;
    flashBuff[6] = SoftwareDataStructure.tempWeight;
    flashBuff[7] = SoftwareDataStructure.humiWeight;
    flashBuff[8] = SoftwareDataStructure.lightWeight;
    flashBuff[9] = SoftwareDataStructure.rainWeight;
    flashBuff[10] = SoftwareDataStructure.fireWeight;
    flashBuff[11] = SoftwareDataStructure.tempMax;
    flashBuff[12] = SoftwareDataStructure.humiMax;
    flashBuff[13] = SoftwareDataStructure.lightMax;
    flashBuff[14] = SoftwareDataStructure.rainMax;
    flashBuff[15] = SoftwareDataStructure.fireMax;
    flashBuff[16] = SoftwareDataStructure.greenWarn;
    flashBuff[16] <<= 8;
    flashBuff[16] |= SoftwareDataStructure.blueWarn;
    flashBuff[17] = SoftwareDataStructure.yellowWarn;
    flashBuff[17] <<= 8;
    flashBuff[17] |= SoftwareDataStructure.redWarn;
    STMFLASH_Write(DATA_START_ADDR | 28, flashBuff, 18);
}



