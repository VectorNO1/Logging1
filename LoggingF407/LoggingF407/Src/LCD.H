#ifndef  __LCD_H
#define  __LCD_H
#include "stm32f4xx_hal.h"
#include "user.h"


//-------------------------屏幕物理像素设置--------------------------------------//
#define LCD_X_SIZE	        176
#define LCD_Y_SIZE	        220

/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#define USE_HORIZONTAL  		1	//定义是否使用横屏 		0,不使用.1,使用.

#ifdef USE_HORIZONTAL//如果定义了横屏 
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE
#endif

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111


#define LCD_CTRL   	  	GPIOD		//定义TFT数据端口
#define LCD_RS         	GPIO_PIN_0//PB11--->>TFT --RS/DC
#define LCD_CS        	GPIO_PIN_2 //MCU_PB11--->>TFT --CS/CE
#define LCD_RST     	GPIO_PIN_1	//PB10--->>TFT --RST

//液晶控制口置1操作语句宏定义
#define	LCD_CS_SET  	LCD_CTRL->BSRR=LCD_CS    
#define	LCD_RS_SET  	LCD_CTRL->BSRR=LCD_RS    
//#define	LCD_SDA_SET  	LCD_CTRL->BSRR=LCD_SDA    
//#define	LCD_SCL_SET  	LCD_CTRL->BSRR=LCD_SCL    
#define	LCD_RST_SET  	LCD_CTRL->BSRR=LCD_RST    
//#define	LCD_LED_SET  	LCD_CTRL->BSRR=LCD_LED   

//液晶控制口置0操作语句宏定义
#define	LCD_CS_CLR  	LCD_CTRL->BSRR=LCD_CS<<16U;    
#define	LCD_RS_CLR  	LCD_CTRL->BSRR=LCD_RS<<16U;    
//#define	LCD_SDA_CLR  	LCD_CTRL->BSRR=LCD_SDA<<16U;    
//#define	LCD_SCL_CLR  	LCD_CTRL->BSRR=LCD_SCL<<16U;    
#define	LCD_RST_CLR  	LCD_CTRL->BSRR=LCD_RST<<16U;    
//#define	LCD_LED_CLR  	LCD_CTRL->BSRR=LCD_LED<<16U; 

struct typFNT_GB162
{
       unsigned char Index[2];	
       char Msk[32];
};

struct typFNT_GB242
{
       unsigned char Index[2];	
       char Msk[72];
};

void Lcd_Init(void);
void Lcd_Clear(uint16_t Color);    
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s);
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num);
void Show_IP_Config(USERPRV * _userprv,uint16_t fc,uint16_t bc);
void showimage(const unsigned char *p,uint8_t xpos,uint8_t ypos); 
void Show_One_Line_Parameter(uint16_t xpos,uint16_t ypos,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4,uint16_t fc,uint16_t bc);
void Show_One_Minate(USERPRV * _userprv,uint8_t delaytime);
void Show_Num(uint16_t xpos,uint16_t ypos,uint16_t fc,uint16_t bc,uint32_t VAL);
void Show_Num_float(uint16_t xpos,uint16_t ypos,uint16_t fc,uint16_t bc,float VAL);
#endif

