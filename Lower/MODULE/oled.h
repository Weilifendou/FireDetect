#ifndef __OLED_H
#define __OLED_H

#include "sys.h"


//IO操作函数 
#define OLED_IIC_SCL    PBout(6) //SCL
#define OLED_IIC_SDA    PBout(7) //SDA
#define READ_SDA   PBin(7)  //输入SDA 

//IIC所有操作函数
void OLED_SDA_OUT(void);
void OLED_SDA_IN(void);
void OLED_IIC_Init(void);                //初始化IIC的IO口 
void OLED_IIC_Start(void);//发送IIC开始信号
void OLED_IIC_Stop(void);  //发送IIC停止信号
void OLED_IIC_Send_Byte(u8 txd);//IIC发送一个字节
u8 OLED_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 OLED_IIC_Wait_Ack(void); //IIC等待ACK信号
void OLED_IIC_Ack(void);//IIC发送ACK信号
void OLED_IIC_NAck(void);//IIC不发送ACK信号


void OLED_Init(void);
void OLED_Write(u8 addr, u8 data);

#define OLED_ADDRESS 0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_ClearContent(unsigned char fill_Data);
void OLED_ClearLine(unsigned char line);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, char* ch, unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ShowCNS(u8 x, u8 y, u8 cns[], u8 num);

void DisplayBattery(void);
void DisplayInfo(void);
#endif


