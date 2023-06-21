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
    char text[20] = {0};//���ڳ�ʼ��
    delay_init(); //����Ҫ���ã�������ʱ��׼ȷ
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    UART_Init(115200); 
    LED_Init(); //LED��ʼ��
    Power_Init(); //��Դ�����ʼ��
    Warning_Init(); //������ʼ��
    Adc_Init(); //ADC��ʼ��
    TrackLight_Init();  //׷���ʼ��
    HandleData_Init();  //���ݴ����ʼ��
    Parameter_Init(); //���������ʼ��
    DHT11_Init(); //��ʪ�ȴ�������ʼ��
    OLED_Init(); //��Ļ��ʼ��
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
    TIM3_PWM_Init(1999, 719); //PWM����ʼ��������Ϊ20ms
    TIM2_Int_Init(5999, 719); //��ʱ��2��ʼ����ʱ1s
//    Iwdg_Init();
    
    OSInit();
    OSTaskCreate(start_task,(void*)0,(OS_STK*)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO); //������ʼ����
    OSStart();
} 

