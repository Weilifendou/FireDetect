#include "timer.h"
#include "led.h"
#include "tracklight.h"
#include "warning.h"
#include "parameters.h"

//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
    
    //��ʱ��TIM2��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ    
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


    TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx                     
}
//��ʱ��3�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
//    static u16 trackDelayCounter = 0;
    static u16 warnDelayCounter = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
//        LED=!LED;
//        Track();
//        if (trackDelayCounter >= 3) {
//            trackDelayCounter = 0;
//        }
        if (WarnSpeed && WarnTime) {
            if (warnDelayCounter >= WarnSpeed) {
                warnDelayCounter = 0;
            }
            if (warnDelayCounter < WarnTime) {
                BUZZER = 1;
            } else {
                BUZZER = 0;
            }
            if (DetectDataStructure.warnIndex >= 75) {
                if (warnDelayCounter < WarnTime) {
                    RED_LED = 0;
                } else {
                    RED_LED = 1;
                }
            }
            warnDelayCounter++;
        }
//        trackDelayCounter++;
    }
}






