#include "sys.h"
#include "uart.h"
#include "parameters.h"
#include "debugger.h"
#include "str.h"


void UART_Init(u32 bound){
  //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);    //使能USART1，GPIOA时钟

    //USART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

    //USART1_RX      GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

    //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);    //根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    //收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
    USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void SendByte(u8 byte)
{
    USART_SendData(USART1, byte);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC )==RESET);
}

void PrintFloat(double value, u8 precision) //串口打印浮点数函数，precision为精确位数
{   
    u8 i = 0;
    char* text = FloatToString(value, precision);
    for(i = 0; *(text + i) != 0; i++) {
        SendByte(*(text + i));
    }
}
void PrintInt(long value)              //串口打印整型数函数
{
    u8 i = 0;
    char* text = IntToString(value);
    for(i = 0; *(text + i) != 0; i++) {
        SendByte(*(text + i));
    }
}
void PrintText(char* text)           //串口打印字符串
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


//跳转到应用程序段
//addr:用户代码起始地址.
//void JumpToAddr(u32 addr)
//{
//    JumpFun jumpAddr;
//    if(((*(vu32*)addr)&0x2FFE0000)==0x20000000) //检查栈顶地址是否合法.
//    { 
//        jumpAddr=(JumpFun)*(vu32*)(addr+4); //用户代码区第二个字为程序开始地址(复位地址)
//        MSR_MSP(*(vu32*)addr); //初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
//        jumpAddr(); //跳转到APP.
//    }
//}

u8 WriteFlashFlag;
u8 RecBuff[RX_LENGTH];
void USART1_IRQHandler(void)                    //串口1中断服务程序
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
                        //硬件调试
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
                        HandwareDataStructure.str[s - i - 2] = 0; //为字符串添加结尾标识符
                    } else {
                        //软件调试
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
