
#include "myiic.h"
//#include "delay.h"
#include "user.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

//初始化IIC
void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;	
  __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	IIC_SDA_H;
	IIC_SCL_H;
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_H;	  	  
	IIC_SCL_H;
	Delay_us(4);
 	IIC_SDA_L;//START:when CLK is high,DATA change form high to low 
	Delay_us(4);
	IIC_SCL_L;//钳住I2C总线，准备发送或接收数据 
	Delay_us(4);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	//SDA_OUT();//sda线输出
	IIC_SCL_L;
	IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
 	Delay_us(4);
	IIC_SCL_H; 
	Delay_us(4);
	IIC_SDA_H;//发送I2C总线结束信号
	Delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	//uint8_t ucErrTime=0;  
	IIC_SCL_H;
	Delay_us(2);	 
	IIC_SCL_L; 
	Delay_us(2);
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	Delay_us(2);
	IIC_SCL_H;
	Delay_us(2);
	IIC_SCL_L;
	
	Delay_us(2);
	
	IIC_SDA_H;
	Delay_us(2);
	IIC_SDA_L;
	Delay_us(2);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SDA_H;
	Delay_us(2);
	IIC_SCL_H;
	Delay_us(2);
	IIC_SCL_L;
	Delay_us(2);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;   
	for(t=0;t<8;t++)
	{
			if((txd&0x80)>>7)
				IIC_SDA_H;
			else
				IIC_SDA_L;
			txd<<=1; 	  
			Delay_us(2);   //对TEA5767这三个延时都是必须的
			IIC_SCL_H;
			Delay_us(2); 
			IIC_SCL_L;	
			Delay_us(2);
	}	 
	IIC_SDA_L;
} 	    
//读1个字节，ack_H时，发送ACK，ack_L，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
  for(i=0;i<8;i++ )
	 {

		IIC_SCL_H;
		Delay_us(2);
		receive<<=1;
		if(READ_SDA)receive++;   
		Delay_us(2); 
		IIC_SCL_L; 
		Delay_us(2);
    }		
	 return receive;
}



























