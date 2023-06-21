#include "sgp30.h"
#include "delay.h"



//初始化软件IIC
void SGP30_IIC_Init(void)
{                         
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    
       
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    SGP30_IIC_SCL=1;
    SGP30_IIC_SDA=1;

}


void SGP30_SetSDAOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SGP30_SetSDAIn(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//产生IIC起始信号
void SGP30_IIC_Start(void)
{
    SGP30_SetSDAOut();
    SGP30_IIC_SDA = 1;
    SGP30_IIC_SCL = 1;
    delay_us(20);
    SGP30_IIC_SDA = 0;	//START:when CLK is high,DATA change form high to low
    delay_us(20);
    SGP30_IIC_SCL = 0; 	//钳住I2C总线，准备发送或接收数据
}

//产生IIC停止信号
void SGP30_IIC_Stop(void)
{
    SGP30_SetSDAOut();
    SGP30_IIC_SCL = 0;
    SGP30_IIC_SDA = 0;	//STOP:when CLK is high DATA change form low to high
    delay_us(20);
    SGP30_IIC_SCL = 1;
    SGP30_IIC_SDA = 1;	//发送I2C总线结束信号
    delay_us(20);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 SGP30_IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SGP30_SetSDAIn();
    SGP30_IIC_SDA = 1;
    delay_us(10);
    SGP30_IIC_SCL = 1;
    delay_us(10);
    while(READ_SDA)
    {
    ucErrTime++;
    if(ucErrTime > 250)
    {
      SGP30_IIC_Stop();
      return 1;
    }
    }
    SGP30_IIC_SCL = 0;  		//时钟输出0
    return 0;
}
//产生ACK应答
void SGP30_IIC_Ack(void)
{
    SGP30_IIC_SCL = 0;
    SGP30_SetSDAOut();
    SGP30_IIC_SDA = 0;
    delay_us(20);
    SGP30_IIC_SCL = 1;
    delay_us(20);
    SGP30_IIC_SCL = 0;
}

//不产生ACK应答
void SGP30_IIC_NAck(void)
{
    SGP30_IIC_SCL = 0;
    SGP30_SetSDAOut();
    SGP30_IIC_SDA = 1;
    delay_us(20);
    SGP30_IIC_SCL = 1;
    delay_us(20);
    SGP30_IIC_SCL = 0;
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void SGP30_IIC_Send_Byte(u8 txd)
{
    u8 t;
    SGP30_SetSDAOut();
    SGP30_IIC_SCL = 0; 	    	//拉低时钟开始数据传输
    for(t = 0; t < 8; t++)
    {
    if((txd & 0x80) >> 7)
      SGP30_IIC_SDA = 1;
    else
      SGP30_IIC_SDA = 0;
    txd <<= 1;
    delay_us(20);
    SGP30_IIC_SCL = 1;
    delay_us(20);
    SGP30_IIC_SCL = 0;
    delay_us(20);
    }
    delay_us(20);

}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u16 SGP30_IIC_Read_Byte(u8 ack)
{
    u8 i;
    u16 receive = 0;
    SGP30_SetSDAIn();
    for(i = 0; i < 8; i++ )
    {
    SGP30_IIC_SCL = 0;
    delay_us(20);
    SGP30_IIC_SCL = 1;
    receive <<= 1;
    if(READ_SDA)
      receive++;
    delay_us(20);
    }
    if (!ack)
    SGP30_IIC_NAck();//发送nACK
    else
    SGP30_IIC_Ack(); //发送ACK
    return receive;
}


//初始化IIC接口
void SGP30_Init(void)
{
    SGP30_IIC_Init();
    SGP30_Write(0x20, 0x03);
//	SGP30_ad_write(0x20,0x61);
//	SGP30_ad_write(0x01,0x00);
}


void SGP30_Write(u8 a, u8 b)
{
    SGP30_IIC_Start();
    SGP30_IIC_Send_Byte(SGP30_write); //发送器件地址+写指令
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Send_Byte(a);		//发送控制字节
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Send_Byte(b);
    SGP30_IIC_Wait_Ack();
    SGP30_IIC_Stop();
    delay_ms(100);
}

u8* SGP30_Read(void)
{
    static u8 dat[5] = {0};
    __disable_irq();   // 关闭总中断
    SGP30_Write(0x20,0x08);
    SGP30_IIC_Start();
    SGP30_IIC_Send_Byte(SGP30_read); //发送器件地址+读指令
    SGP30_IIC_Wait_Ack();
    dat[0] = SGP30_IIC_Read_Byte(1);
    dat[1] = SGP30_IIC_Read_Byte(1);
    dat[2] = SGP30_IIC_Read_Byte(1);
    dat[3] = SGP30_IIC_Read_Byte(1);
    dat[4] = SGP30_IIC_Read_Byte(0);
    SGP30_IIC_Stop();
    __enable_irq();   // 关闭总中断
    return dat;
}


//u32 SGP30_Read(void)
//{
//    static u8 dat[4] = {0};
//    u8 crc;
//    SGP30_IIC_Start();
//    SGP30_IIC_Send_Byte(SGP30_read); //发送器件地址+读指令
//    SGP30_IIC_Wait_Ack();
//    dat = SGP30_IIC_Read_Byte(1);
//    dat <<= 8;
//    dat += SGP30_IIC_Read_Byte(1);
//    crc = SGP30_IIC_Read_Byte(1); //crc数据，舍去
//    crc = crc;  //为了不让出现编译警告
//    dat <<= 8;
//    dat += SGP30_IIC_Read_Byte(1);
//    dat <<= 8;
//    dat += SGP30_IIC_Read_Byte(0);
//    SGP30_IIC_Stop();
//    return(dat);
//}


