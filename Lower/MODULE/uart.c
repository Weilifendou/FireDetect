#include "sys.h"
#include "uart.h"
#include "parameters.h"
#include "debugger.h"
#include "str.h"


void UART_Init(u32 bound){
  //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);    //ʹ��USART1��GPIOAʱ��

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

    //USART1_RX      GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);    //����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void SendByte(u8 byte)
{
    USART_SendData(USART1, byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
}

void PrintFloat(double value, u8 precision) //���ڴ�ӡ������������precisionΪ��ȷλ��
{   
    u8 i = 0;
    char* text = FloatToString(value, precision);
    for(i = 0; *(text + i) != 0; i++) {
        SendByte(*(text + i));
    }
}
void PrintInt(long value)              //���ڴ�ӡ����������
{
    u8 i = 0;
    char* text = IntToString(value);
    for(i = 0; *(text + i) != 0; i++) {
        SendByte(*(text + i));
    }
}
void PrintText(char* text)           //���ڴ�ӡ�ַ���
{
    u8 i = 0;
    for(i = 0; *(text + i) != 0; i++) {
        SendByte(*(text + i));
    }
}


u16 CRCCheckout(u8 *dat, u16 length) {
    u8 temp = 0;
    u16 i = 0;
    u16 j = 0;
    u16 crc = 0xffff;
    for (i = 0; i < length; i++) {
        temp = *(dat + i) & 0x00ff;
        crc ^= temp;
        for (j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xa001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}


void SendData(void) {
    u8 i = 0;
    u16 crc = 0;
    u8 sendBuff[19];
    
    sendBuff[0] = 0x55;
    sendBuff[1] = 0xaa;
    sendBuff[2] = 17;
    sendBuff[3] = DetectDataStructure.temperature >> 8;
    sendBuff[4] = DetectDataStructure.temperature;
    sendBuff[5] = DetectDataStructure.humidty >> 8;
    sendBuff[6] = DetectDataStructure.humidty;
    sendBuff[7] = DetectDataStructure.light >> 8;
    sendBuff[8] = DetectDataStructure.light;
    sendBuff[9] = DetectDataStructure.rainIndex >> 8;
    sendBuff[10] = DetectDataStructure.rainIndex;
    sendBuff[11] = DetectDataStructure.fireIndex >> 8;
    sendBuff[12] = DetectDataStructure.fireIndex;
    sendBuff[13] = DetectDataStructure.warnIndex >> 8;
    sendBuff[14] = DetectDataStructure.warnIndex;
    sendBuff[15] = DetectDataStructure.battery >> 8;
    sendBuff[16] = DetectDataStructure.battery;
    
    crc = CRCCheckout(sendBuff, 17);
    
    sendBuff[17] = crc >> 8;
    sendBuff[18] = crc;
    
    for (i = 0; i < TX_LENGTH; i++) {
        SendByte(sendBuff[i]);
    }
}


//��ת��Ӧ�ó����
//addr:�û�������ʼ��ַ.
//void JumpToAddr(u32 addr)
//{
//    JumpFun jumpAddr;
//    if(((*(vu32*)addr)&0x2FFE0000)==0x20000000) //���ջ����ַ�Ƿ�Ϸ�.
//    { 
//        jumpAddr=(JumpFun)*(vu32*)(addr+4); //�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)
//        MSR_MSP(*(vu32*)addr); //��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
//        jumpAddr(); //��ת��APP.
//    }
//}

u8 WriteFlashFlag;
u8 RecBuff[RX_LENGTH];
void USART1_IRQHandler(void)                    //����1�жϷ������
{
    u8 i = 0;
    u8 s = 0;
    u16 recCRC = 0;
    u16 calCRC = 0;
    u16 datLength = 0;
    static u8 buffCounter = 0;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
        RecBuff[buffCounter++] = USART_ReceiveData(USART1);
        if (buffCounter >= RX_LENGTH) {
            buffCounter = 0;
            for (s = 0; s < RX_LENGTH; s++) {
                if (RecBuff[s] == 0x55 && RecBuff[s + 1] == 0xaa) {
                    datLength = RecBuff[s + 2];
                    break;
                }
            }
            if (s + datLength < RX_LENGTH) {
                calCRC = CRCCheckout(RecBuff + s, datLength);
                recCRC = RecBuff[s + datLength];
                recCRC <<= 8;
                recCRC |= RecBuff[s + datLength + 1];
                if (calCRC == recCRC) {
                    WriteFlashFlag = 1;
                    s += 3;
                    DebugMode = RecBuff[s++];
                    if (DebugMode) {
                        //Ӳ������
                        HandwareDataStructure.WarnControl = RecBuff[s++];
                        HandwareDataStructure.PowerControl = RecBuff[s++];
                        HandwareDataStructure.elevPWM = RecBuff[s++];
                        HandwareDataStructure.flatPWM = RecBuff[s++];
                        HandwareDataStructure.screenX = RecBuff[s++];
                        HandwareDataStructure.screenY = RecBuff[s++];
                        HandwareDataStructure.textSize = RecBuff[s++];
                        for (i = s; RecBuff[s] != 0; s++) {
                            HandwareDataStructure.str[s - i] = RecBuff[s];
                        }
                        HandwareDataStructure.str[s - i - 2] = 0; //Ϊ�ַ�����ӽ�β��ʶ��
                    } else {
                        //�������
                        SoftwareDataStructure.page = RecBuff[s++];
                        SoftwareDataStructure.fullBatteryADC = RecBuff[s++];
                        SoftwareDataStructure.fullBatteryADC <<= 8;
                        SoftwareDataStructure.fullBatteryADC |= RecBuff[s++];
                        
                        SoftwareDataStructure.lightThreshold = RecBuff[s++];
                        SoftwareDataStructure.lightThreshold <<= 8;
                        SoftwareDataStructure.lightThreshold |= RecBuff[s++];
                        
                        SoftwareDataStructure.lightSense = RecBuff[s++];
                        SoftwareDataStructure.lightSense <<= 8;
                        SoftwareDataStructure.lightSense |= RecBuff[s++];
                        
                        SoftwareDataStructure.offset = RecBuff[s++];
                        
                        SoftwareDataStructure.trackADCTimes = RecBuff[s++];
                        SoftwareDataStructure.handleADCTimes = RecBuff[s++];
                        
                        SoftwareDataStructure.tempWeight = RecBuff[s++];
                        SoftwareDataStructure.tempWeight <<= 8;
                        SoftwareDataStructure.tempWeight |= RecBuff[s++];
                        
                        SoftwareDataStructure.humiWeight = RecBuff[s++];
                        SoftwareDataStructure.humiWeight <<= 8;
                        SoftwareDataStructure.humiWeight |= RecBuff[s++];
                        
                        SoftwareDataStructure.lightWeight = RecBuff[s++];
                        SoftwareDataStructure.lightWeight <<= 8;
                        SoftwareDataStructure.lightWeight |= RecBuff[s++];
                        
                        SoftwareDataStructure.rainWeight = RecBuff[s++];
                        SoftwareDataStructure.rainWeight <<= 8;
                        SoftwareDataStructure.rainWeight |= RecBuff[s++];
                        
                        SoftwareDataStructure.fireWeight = RecBuff[s++];
                        SoftwareDataStructure.fireWeight <<= 8;
                        SoftwareDataStructure.fireWeight |= RecBuff[s++];
                        
                        SoftwareDataStructure.tempMax = RecBuff[s++];
                        SoftwareDataStructure.tempMax <<= 8;
                        SoftwareDataStructure.tempMax |= RecBuff[s++];
                        
                        SoftwareDataStructure.humiMax = RecBuff[s++];
                        SoftwareDataStructure.humiMax <<= 8;
                        SoftwareDataStructure.humiMax |= RecBuff[s++];
                        
                        SoftwareDataStructure.lightMax = RecBuff[s++];
                        SoftwareDataStructure.lightMax <<= 8;
                        SoftwareDataStructure.lightMax |= RecBuff[s++];
                        
                        SoftwareDataStructure.rainMax = RecBuff[s++];
                        SoftwareDataStructure.rainMax <<= 8;
                        SoftwareDataStructure.rainMax |= RecBuff[s++];
                        
                        SoftwareDataStructure.fireMax = RecBuff[s++];
                        SoftwareDataStructure.fireMax <<= 8;
                        SoftwareDataStructure.fireMax |= RecBuff[s++];
                        
                        SoftwareDataStructure.greenWarn = RecBuff[s++];
                        SoftwareDataStructure.blueWarn = RecBuff[s++];
                        SoftwareDataStructure.yellowWarn = RecBuff[s++];
                        SoftwareDataStructure.redWarn = RecBuff[s++];
                    }
                }
            }
        }
    }
}
