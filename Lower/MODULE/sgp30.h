#ifndef __SGP30_H
#define __SGP30_H
#include "sys.h"

#define  SGP30_IIC_SCL   PBout(6)
#define  SGP30_IIC_SDA   PBout(7)
#define  READ_SDA   PBin(7)


#define SGP30_read  0xb1  //SGP30的读地址
#define SGP30_write 0xb0  //SGP30的写地址


void SGP30_IIC_Start(void);//发送IIC开始信号
void SGP30_IIC_Stop(void);//发送IIC停止信号
void SGP30_IIC_Send_Byte(u8 txd);//IIC发送一个字节
u16 SGP30_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 SGP30_IIC_Wait_Ack(void);//IIC等待ACK信号
void SGP30_IIC_Ack(void);//IIC发送ACK信号
void SGP30_IIC_NAck(void);//IIC不发送ACK信号
void SGP30_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 SGP30_IIC_Read_One_Byte(u8 daddr,u8 addr);
void SGP30_Init(void);
void SGP30_Write(u8 a, u8 b);
u8* SGP30_Read(void);



#endif

