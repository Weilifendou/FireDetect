#ifndef __SYSTASK_H
#define __SYSTASK_H
#include "sys.h"
#include "includes.h"

//START ����
//�����������ȼ�
#define START_TASK_PRIO            10  ///��ʼ��������ȼ�Ϊ���
//���������ջ��С
#define START_STK_SIZE            128
//���������ջ
extern OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//�����������ȼ�
#define LED_TASK_PRIO           7
//���������ջ��С
#define LED_STK_SIZE            128
//�����ջ
extern OS_STK LED_TASK_STK[LED_STK_SIZE];
//������
void led_task(void *pdata);


//�����������ȼ�
#define HANDLE_TASK_PRIO        1
//���������ջ��С
#define HANDLE_STK_SIZE         128
//�����ջ
extern OS_STK HANDLE_TASK_STK[HANDLE_STK_SIZE];
//������
void handle_task(void *pdata);


//�����������ȼ�
#define TRACK_TASK_PRIO        2
//���������ջ��С
#define TRACK_STK_SIZE         128
//�����ջ
extern OS_STK TRACK_TASK_STK[TRACK_STK_SIZE];
//������
void track_task(void *pdata);


//�����������ȼ�
#define DISPLAY_TASK_PRIO        3
//���������ջ��С
#define DISPLAY_STK_SIZE         128
//�����ջ
extern OS_STK DISPLAY_TASK_STK[DISPLAY_STK_SIZE];
//������
void display_task(void *pdata);

void DisplayBattery(void);
void DisplayInfo(void);
#endif


