#ifndef __SYSTASK_H
#define __SYSTASK_H
#include "sys.h"
#include "includes.h"

//START 任务
//设置任务优先级
#define START_TASK_PRIO            10  ///开始任务的优先级为最低
//设置任务堆栈大小
#define START_STK_SIZE            128
//任务任务堆栈
extern OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//设置任务优先级
#define LED_TASK_PRIO           7
//设置任务堆栈大小
#define LED_STK_SIZE            128
//任务堆栈
extern OS_STK LED_TASK_STK[LED_STK_SIZE];
//任务函数
void led_task(void *pdata);


//设置任务优先级
#define HANDLE_TASK_PRIO        1
//设置任务堆栈大小
#define HANDLE_STK_SIZE         128
//任务堆栈
extern OS_STK HANDLE_TASK_STK[HANDLE_STK_SIZE];
//任务函数
void handle_task(void *pdata);


//设置任务优先级
#define TRACK_TASK_PRIO        2
//设置任务堆栈大小
#define TRACK_STK_SIZE         128
//任务堆栈
extern OS_STK TRACK_TASK_STK[TRACK_STK_SIZE];
//任务函数
void track_task(void *pdata);


//设置任务优先级
#define DISPLAY_TASK_PRIO        3
//设置任务堆栈大小
#define DISPLAY_STK_SIZE         128
//任务堆栈
extern OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//任务函数
void display_task(void *pdata);

void DisplayBattery(void);
void DisplayInfo(void);
#endif


