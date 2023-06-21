#include "systask.h"
#include "delay.h"
#include "led.h"
#include "parameters.h"
#include "dht11.h"
#include "adc.h"
#include "stmflash.h"
#include "oled.h"
#include "uart.h"
#include "debugger.h"
#include "handledata.h"
#include "tracklight.h"
#include "power.h"


//任务任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务堆栈
OS_STK LED_TASK_STK[LED_STK_SIZE];
//任务堆栈
OS_STK HANDLE_TASK_STK[LED_STK_SIZE];
//任务堆栈
OS_STK TRACK_TASK_STK[LED_STK_SIZE];
//任务堆栈
OS_STK DISPLAY_TASK_STK[LED_STK_SIZE];

//开始任务
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
    pdata=pdata;
    OSStatInit();  //开启统计任务
    
    OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
    OSTaskCreate(led_task,(void*)0,(OS_STK*)&LED_TASK_STK[LED_STK_SIZE-1],LED_TASK_PRIO); //创建LED任务
    OSTaskCreate(handle_task,(void*)0,(OS_STK*)&HANDLE_TASK_STK[LED_STK_SIZE-1],HANDLE_TASK_PRIO); //创建HANDLE任务
    OSTaskCreate(track_task,(void*)0,(OS_STK*)&TRACK_TASK_STK[LED_STK_SIZE-1],TRACK_TASK_PRIO); //创建HANDLE任务
    OSTaskCreate(display_task,(void*)0,(OS_STK*)&DISPLAY_TASK_STK[LED_STK_SIZE-1],DISPLAY_TASK_PRIO); //创建HANDLE任务
    OSTaskSuspend(START_TASK_PRIO); //挂起开始任务
    OS_EXIT_CRITICAL();  //退出临界区(开中断)
}

void led_task(void *pdata)
{
    while(1) {
        LED = 0;
        delay_ms(40);
        LED = 1;
        delay_ms(1000);
    }
    
}

void handle_task(void *pdata)
{
    static u8 taskSuspendFlag = 0;
    static u8 delayCounter = 0xff;
    while(1)
    {
        if (!DebugMode) {
            OSTaskSuspend(LED_TASK_PRIO);
            OSTaskSuspend(TRACK_TASK_PRIO);
            OSTaskSuspend(DISPLAY_TASK_PRIO);
            HandleData();
            SendData();
            SendData();
            OSTaskResume(LED_TASK_PRIO);
            OSTaskResume(TRACK_TASK_PRIO);
            OSTaskResume(DISPLAY_TASK_PRIO);
            taskSuspendFlag = 0;
        } else {
            if (!taskSuspendFlag) {
                taskSuspendFlag = 1;
                OSTaskSuspend(LED_TASK_PRIO);
                OSTaskSuspend(TRACK_TASK_PRIO);
                OSTaskSuspend(DISPLAY_TASK_PRIO);
            }
            if (delayCounter >= 5) {
                delayCounter = 0;
                HandWareDebug();
            }
            delayCounter++;
        }
        if (WriteFlashFlag) {
            WriteFlashFlag = 0;
            if (DebugMode) {
                WriteHandwareDataToFlash();
            } else {
                WriteSoftwareDataToFlash();
                NVIC_SystemReset();
            }
        }
        delay_ms(200);
    }
}

void track_task(void *pdata)
{
    while(1)
    {
        Track();
        delay_ms(200);
    }
}

void display_task(void *pdata)
{
    while(1)
    {
//        if (DetectDataStructure.warnIndex > SoftwareDataStructure.yellowWarn) {
//            SoftwareDataStructure.page = 3;
//        }
//        if (DetectDataStructure.light >= SoftwareDataStructure.lightThreshold) {
//            OLED_ON();
//            ControlElevSteer(1);
//            ControlFlatSteer(1);
//            ControlBluetooth5V(1);
//            ControlBluetooth3V(1);
//        } else {
//            OLED_OFF();
//            ControlElevSteer(0);
//            ControlFlatSteer(0);
//            ControlBluetooth5V(0);
//            ControlBluetooth3V(0);
//        }
        SoftwareDataStructure.page = 0;
        DisplayInfo();
        delay_ms(2000);
    }
}


