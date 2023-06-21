#include "led.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "OLED.h"
#include "dht11.h"
#include "string.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "stmflash.h"
#include "tracklight.h"
#include "handledata.h"
#include "warning.h"
#include "power.h"
#include "parameters.h"
#include "debugger.h"
#include "includes.h"
#include "systask.h"
#include "str.h"



int main(void)
{
    char text[20] = {0};//串口初始化
    delay_init(); //必须要调用，否则延时不准确
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    UART_Init(115200); 
    LED_Init(); //LED初始化
    Power_Init(); //电源管理初始化
    Warning_Init(); //报警初始化
    Adc_Init(); //ADC初始化
    TrackLight_Init();  //追光初始化
    HandleData_Init();  //数据处理初始化
    Parameter_Init(); //各项参数初始化
    DHT11_Init(); //温湿度传感器初始化
    OLED_Init(); //屏幕初始化
    OLED_Fill(0x00);
    ClearStr(text);
    AddStr(text, "HCY TEAM");
    OLED_ShowStr(35, 3, text, 2);
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    OLED_Fill(0x00);
    ClearStr(text);
    AddStr(text, "OrigElev: ");
    AddStr(text, IntToString(HandwareDataStructure.elevPWM));
    OLED_ShowStr(8, 2, text, 2);
    ClearStr(text);
    AddStr(text, "OrigFlat: ");
    AddStr(text, IntToString(HandwareDataStructure.flatPWM));
    OLED_ShowStr(8, 4, text, 2);
    delay_ms(1000);
    delay_ms(1000);
    TIM3_PWM_Init(1999, 719); //PWM波初始化，周期为20ms
    TIM2_Int_Init(5999, 719); //定时器2初始化定时1s
//    Iwdg_Init();
    
    OSInit();
    OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //创建开始任务
    OSStart();
} 

