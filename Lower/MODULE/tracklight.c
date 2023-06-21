#include "tracklight.h"
#include "adc.h"
#include "power.h"
#include "parameters.h"


void TrackLight_Init(void){

}
void Track(void) {
    int diff = 0;
    static u8 lastElevPWM = 0;
    static u8 lastFlatPWM = 0;
    
    u16 upLight = DetectDataStructure.upLight;
    u16 downLight = DetectDataStructure.downLight;
    u16 leftLight = DetectDataStructure.leftLight;
    u16 rightLight = DetectDataStructure.rightLight;
    
    diff = upLight - downLight;
    if (diff > SoftwareDataStructure.lightSense) {
        HandwareDataStructure.elevPWM += SoftwareDataStructure.offset;
        if (HandwareDataStructure.elevPWM > ELEV_MAX_PWM) {
            HandwareDataStructure.elevPWM = ELEV_MAX_PWM;
        }
    }
    if (diff < -SoftwareDataStructure.lightSense) {
        HandwareDataStructure.elevPWM -= SoftwareDataStructure.offset;
        if (HandwareDataStructure.elevPWM < ELEV_MIN_PWM) {
            HandwareDataStructure.elevPWM = ELEV_MIN_PWM;
        }
    }
    diff = leftLight - rightLight;
    if (diff > SoftwareDataStructure.lightSense) {
        HandwareDataStructure.flatPWM += SoftwareDataStructure.offset;
        if (HandwareDataStructure.flatPWM > FLAT_MAX_PWM) {
            HandwareDataStructure.flatPWM = FLAT_MAX_PWM;
        }
    }
    if (diff < -SoftwareDataStructure.lightSense) {
        HandwareDataStructure.flatPWM -= SoftwareDataStructure.offset;
        if (HandwareDataStructure.flatPWM < ELEV_MIN_PWM) {
            HandwareDataStructure.flatPWM = ELEV_MIN_PWM;
        }
    }
    
    if (!lastElevPWM || lastElevPWM != HandwareDataStructure.elevPWM) {
        ControlElevSteer(1);
        TIM_SetCompare3(TIM3, HandwareDataStructure.elevPWM);
    } else {
        ControlElevSteer(0);
    }
    if (!lastFlatPWM || lastFlatPWM != HandwareDataStructure.flatPWM) {
        ControlFlatSteer(1);
        TIM_SetCompare4(TIM3, HandwareDataStructure.flatPWM);
    } else {
        ControlFlatSteer(0);
    }
    
    lastElevPWM = HandwareDataStructure.elevPWM;
    lastFlatPWM = HandwareDataStructure.flatPWM;
    
    
}



