#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f4xx_hal.h"
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

//VOID ABC(void)
//{
//	GPIOB->MODER  &=~ 0X0000C000;
//  GPIOB->PUPDR  &=~ 0X00000000;	
//	GPIOB->PUPDR  |=  0X00004000;
//	
//	GPIOB->MODER  &=~ 0X0000C000;
//	GPIOB->MODER  |=  0X00004000;
//	GPIOB->PUPDR  &=~ 0X00000000;	
//	GPIOB->PUPDR  |=  0X00004000;
//	GPIOB->OTYPER &=~ 0X00000080;
//	GPIOB->OSPEEDR&=~ 0X0000C000;
//	GPIOB->OSPEEDR|=  0X00008000;
//	
//	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
//	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)
//}

   	   		   
//IO方向设置  B7
#define SDA_IN()  {	GPIOB->MODER  &=~ 0X0000C000;\
										GPIOB->PUPDR  &=~ 0X00000000;}
										//GPIOB->PUPDR  |=  0X00004000;}

#define SDA_OUT() {	GPIOB->MODER  &=~ 0X0000C000;\
										GPIOB->MODER  |=  0X00004000;}
										//GPIOB->PUPDR  &=~ 0X00000000;\
										//GPIOB->PUPDR  |=  0X00004000;\
										//GPIOB->OTYPER &=~ 0X00000080;\
										//GPIOB->OSPEEDR&=~ 0X0000C000;\
										//GPIOB->OSPEEDR|=  0X00008000;}


//IO操作函数	 
#define IIC_SCL_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET)
#define IIC_SCL_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET)
										
#define IIC_SDA_H  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)  
#define IIC_SDA_L  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
										
#define READ_SDA   HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7) 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















