#include "LCD.h"
#include "spi.h"
#include "font.h"
#include <stdlib.h>
#include <cstdlib>
extern SPI_HandleTypeDef hspi3;
/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd_Reset(void)
{
	LCD_RST_CLR;
  HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(50);
}

/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd_WriteIndex(uint8_t Index)
{
   LCD_CS_CLR;
   LCD_RS_CLR;
	 HAL_SPI_Transmit(&hspi3,&Index,1,10);
   LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(uint8_t Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;
	 HAL_SPI_Transmit(&hspi3,&Data,1,10);
   LCD_CS_SET;
}
/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data)
{	
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);	
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD_WriteReg(uint8_t Index,uint16_t Data)
{
	Lcd_WriteIndex(Index);
  Lcd_WriteData_16Bit(Data);
}

/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(uint16_t Xpos, uint16_t Ypos)
{	
#if USE_HORIZONTAL//如果定义了横屏  	    	
	LCD_WriteReg(0x21,Xpos);
	LCD_WriteReg(0x20,Ypos);
#else//竖屏	
	LCD_WriteReg(0x20,Xpos);
	LCD_WriteReg(0x21,Ypos);
#endif
	Lcd_WriteIndex(0x22);		
} 

/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化_ILI9225_176X220
****************************************************************************/
void Lcd_Init(void)
{	
	Lcd_Reset(); //Reset before LCD Init.
	//LCD Init For 2.2inch LCD Panel with ILI9225.	
	LCD_WriteReg(0x10, 0x0000); // Set SAP,DSTB,STB
	LCD_WriteReg(0x11, 0x0000); // Set APON,PON,AON,VCI1EN,VC
	LCD_WriteReg(0x12, 0x0000); // Set BT,DC1,DC2,DC3
	LCD_WriteReg(0x13, 0x0000); // Set GVDD
	LCD_WriteReg(0x14, 0x0000); // Set VCOMH/VCOML voltage
	//delay_ms(40); // Delay 20 ms
	HAL_Delay(40);
	
	// Please follow this power on sequence
	LCD_WriteReg(0x11, 0x0018); // Set APON,PON,AON,VCI1EN,VC
	LCD_WriteReg(0x12, 0x1121); // Set BT,DC1,DC2,DC3
	LCD_WriteReg(0x13, 0x0063); // Set GVDD
	LCD_WriteReg(0x14, 0x3961); // Set VCOMH/VCOML voltage
	LCD_WriteReg(0x10, 0x0800); // Set SAP,DSTB,STB
//	delay_ms(10); // Delay 10 ms
  HAL_Delay(10);
	LCD_WriteReg(0x11, 0x1038); // Set APON,PON,AON,VCI1EN,VC
//	delay_ms(30); // Delay 30 ms
  HAL_Delay(30);
	
	
	LCD_WriteReg(0x02, 0x0100); // set 1 line inversion

#if USE_HORIZONTAL//如果定义了横屏
	//R01H:SM=0,GS=0,SS=0 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x01, 0x031C); // set the display line number and display direction
	//R03H:BGR=1,ID0=1,ID1=1,AM=1 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x03, 0x1038); // set GRAM write direction .
#else//竖屏
	//R01H:SM=0,GS=0,SS=1 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x01, 0x011C); // set the display line number and display direction 
	//R03H:BGR=1,ID0=1,ID1=1,AM=0 (for details,See the datasheet of ILI9225)
	LCD_WriteReg(0x03, 0x1030); // set GRAM write direction.
#endif

	LCD_WriteReg(0x07, 0x0000); // Display off
	LCD_WriteReg(0x08, 0x0808); // set the back porch and front porch
	LCD_WriteReg(0x0B, 0x1100); // set the clocks number per line
	LCD_WriteReg(0x0C, 0x0000); // CPU interface
	LCD_WriteReg(0x0F, 0x0501); // Set Osc
	LCD_WriteReg(0x15, 0x0020); // Set VCI recycling
	LCD_WriteReg(0x20, 0x0000); // RAM Address
	LCD_WriteReg(0x21, 0x0000); // RAM Address
	
	//------------------------ Set GRAM area --------------------------------//
	LCD_WriteReg(0x30, 0x0000); 
	LCD_WriteReg(0x31, 0x00DB); 
	LCD_WriteReg(0x32, 0x0000); 
	LCD_WriteReg(0x33, 0x0000); 
	LCD_WriteReg(0x34, 0x00DB); 
	LCD_WriteReg(0x35, 0x0000); 
	LCD_WriteReg(0x36, 0x00AF); 
	LCD_WriteReg(0x37, 0x0000); 
	LCD_WriteReg(0x38, 0x00DB); 
	LCD_WriteReg(0x39, 0x0000); 
	
	
	// ---------- Adjust the Gamma 2.2 Curve -------------------//
	LCD_WriteReg(0x50, 0x0603); 
	LCD_WriteReg(0x51, 0x080D); 
	LCD_WriteReg(0x52, 0x0D0C); 
	LCD_WriteReg(0x53, 0x0205); 
	LCD_WriteReg(0x54, 0x040A); 
	LCD_WriteReg(0x55, 0x0703); 
	LCD_WriteReg(0x56, 0x0300); 
	LCD_WriteReg(0x57, 0x0400); 
	LCD_WriteReg(0x58, 0x0B00); 
	LCD_WriteReg(0x59, 0x0017); 
	
	
	
	LCD_WriteReg(0x0F, 0x0701); // Vertical RAM Address Position
	LCD_WriteReg(0x07, 0x0012); // Vertical RAM Address Position
	//delay_ms(50); // Delay 50 ms
	HAL_Delay(50);
	LCD_WriteReg(0x07, 0x1017); // Vertical RAM Address Position  
}

void Lcd_SetRegion(uint8_t xStar, uint8_t yStar,uint8_t xEnd,uint8_t yEnd)
{
#if USE_HORIZONTAL//如果定义了横屏	
	LCD_WriteReg(0x38,xEnd);
	LCD_WriteReg(0x39,xStar);
	LCD_WriteReg(0x36,yEnd);
	LCD_WriteReg(0x37,yStar);
	LCD_WriteReg(0x21,xStar);
	LCD_WriteReg(0x20,yStar);
#else//竖屏	
	LCD_WriteReg(0x36,xEnd);
	LCD_WriteReg(0x37,xStar);
	LCD_WriteReg(0x38,yEnd);
	LCD_WriteReg(0x39,yStar);
	LCD_WriteReg(0x20,xStar);
	LCD_WriteReg(0x21,yStar);
#endif
	Lcd_WriteIndex(0x22);	
}


/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(uint16_t Color)               
{	
   unsigned int i,m;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	Lcd_WriteData_16Bit(Color);
    }   
}

/*函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);
} 

/*************************************************
函数名：Lcd_Show 中文16
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;
	
	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
			  for(i=0;i<16;i++)
				  for(j=0;j<8;j++) 
					  {
					    if(asc16[k*16+i]&(0x80>>j))	
		            Gui_DrawPoint(x+j,y+i,fc);
							else 
							{
								if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
							}
					  }
				x+=8;
			}
			s++;
		}		
		else 
		{
			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
							for(j=0;j<8;j++) 
								{
							    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
									else {
										if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
									}
								}
							for(j=0;j<8;j++) 
								{
							    if(hz16[k].Msk[i*2+1]&(0x80>>j))	
		                Gui_DrawPoint(x+j+8,y+i,fc);
									else 
									{
										if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
									}
								}
				    }
				}
			}
			s+=2;x+=16;
		} 
	}
}

/*************************************************
函数名：Lcd_Show 中文24
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
	unsigned char i,j;
	unsigned short k;

	while(*s) 
	{
		if( *s < 0x80 ) 
		{
			k=*s;
			if (k>32) k-=32; else k=0;

		  for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    if(asc16[k*16+i]&(0x80>>j))	
						  Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
			s++;x+=8;
		}
		else 
		{
			for (k=0;k<hz24_num;k++) 
			{
			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<24;i++)
				    {
							for(j=0;j<8;j++) 
								{
							    	if(hz24[k].Msk[i*3]&(0x80>>j))
									Gui_DrawPoint(x+j,y+i,fc);
									else 
									{
										if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
									}
								}
							for(j=0;j<8;j++) 
								{
							    if(hz24[k].Msk[i*3+1]&(0x80>>j))	
                    Gui_DrawPoint(x+j+8,y+i,fc);
									else 
                  {
										if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
									}
								}
							for(j=0;j<8;j++) 
								{
							   if(hz24[k].Msk[i*3+2]&(0x80>>j))	
									 Gui_DrawPoint(x+j+16,y+i,fc);
									else 
									 {
										if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
									 }
							  }
				    }
			  }
			}
			s+=2;x+=24;
		}
	}
}
/****************************************************************************
* 名    称：Gui_DrawFont_Num32
* 功    能：显示数字
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
	unsigned char i,j,k,c;
  for(i=0;i<32;i++)
		{
		for(j=0;j<4;j++) 
			{
				c=*(sz32+num*32*4+i*4+j);
				for (k=0;k<8;k++)	
				{
		    	if(c&(0x80>>k))	
            Gui_DrawPoint(x+j*8+k,y+i,fc);
					else 
          {
						if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
					}
				}
			}
		}
}

void showimage(const unsigned char *p,uint8_t xpos,uint8_t ypos) //显示40*40 QQ图片
{
  int i = 0; 
	unsigned char picH,picL; 
	Lcd_SetRegion(xpos,ypos,xpos+220,ypos+176);		//坐标设置
		for(i=0;i<220*176;i++)
	 {	
		picL=*(p+i*2);	//数据低位在前
		picH=*(p+i*2+1);				
		Lcd_WriteData_16Bit(picH<<8|picL);  						
	 }	
}
/****************************************************************************
* 名    称：Show_One_Minate
* 功    能：显示1分钟倒计时
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Show_One_Minate(USERPRV * _userprv,uint8_t delaytime)
{
	uint8_t i = 0,data1 = 0,data2 = 0,delay = 0;
	delay = delaytime;
	//while((!_userprv->m_ucETHCommuFlag)||(!_userprv->m_ucUSBCommuFlag)){}
	for(i = 0;i<delay;i++)
	{
		Delay_ms(1000);
		data1 = delaytime/10;
		data2 = delaytime%10;
		Gui_DrawFont_Num32(160,140,RED,BLUE,data1);
		Gui_DrawFont_Num32(193,140,RED,BLUE,data2);
		delaytime--;
	}
}

/****************************************************************************
* 名    称：Show_One_Line_Parameter
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Show_One_Line_Parameter(uint16_t xpos,uint16_t ypos,uint8_t p1,uint8_t p2,uint8_t p3,uint8_t p4,uint16_t fc,uint16_t bc)
{
	char temp[4][6] = {0x00};
	sprintf(temp[0],"%d",p1);
	sprintf(temp[1],"%d",p2);
	sprintf(temp[2],"%d",p3);
	sprintf(temp[3],"%d",p4);
  Gui_DrawFont_GBK16(xpos,ypos,fc,bc,(uint8_t *)temp[0]);
	Gui_DrawFont_GBK16(xpos+8*3,ypos,fc,bc,".");
	Gui_DrawFont_GBK16(xpos+8*4,ypos,fc,bc,(uint8_t *)temp[1]);
	Gui_DrawFont_GBK16(xpos+8*7,ypos,fc,bc,".");
	Gui_DrawFont_GBK16(xpos+8*8,ypos,fc,bc,(uint8_t *)temp[2]);
	Gui_DrawFont_GBK16(xpos+8*11,ypos,fc,bc,".");
	Gui_DrawFont_GBK16(xpos+8*12,ypos,fc,bc,(uint8_t *)temp[3]);
}

/****************************************************************************
* 名    称： 显示数字
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Show_Num(uint16_t xpos,uint16_t ypos,uint16_t fc,uint16_t bc,uint32_t VAL)
{
	char temp[30] = {0x00};
	sprintf(temp,"%d",VAL);
	Gui_DrawFont_GBK16(xpos,ypos,fc,bc,(uint8_t *)temp);
}
/****************************************************************************
* 名    称： 显示 float
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Show_Num_float(uint16_t xpos,uint16_t ypos,uint16_t fc,uint16_t bc,float VAL)
{
	char temp[30] = {0x00},*p,i;
	p = temp;
	sprintf(temp,"%.3f",VAL);
  for(i = 0;p[i]!='\0';i++)
	{
		if(p[i] != '.')
		{
			Gui_DrawFont_Num32(xpos+i*32,ypos,fc,bc,p[i]-0x30);
		}

    else
      Gui_DrawFont_Num32(xpos+i*32,ypos,fc,bc,0xa);
	}	
  //Gui_DrawFont_Num32(xpos+i*32,ypos,fc,bc,0xc);
}
/****************************************************************************
* 名    称：Show_IP_Config
* 功    能：
* 入口参数：无
* 出口参数：无
* 说    明：
****************************************************************************/
void Show_IP_Config(USERPRV * _userprv,uint16_t fc,uint16_t bc)
{
	Lcd_Clear(bc);
	Gui_DrawFont_GBK16(5,5,fc,bc,"模块  IP: ");
	Gui_DrawFont_GBK16(5,25,fc,bc,"子网掩码: ");
	Gui_DrawFont_GBK16(5,45,fc,bc,"默认网关: ");
	Gui_DrawFont_GBK16(5,65,fc,bc,"服务器IP: ");
	Gui_DrawFont_GBK16(5,85,fc,bc,"DNS 地址: ");
	Gui_DrawFont_GBK16(5,105,fc,bc,"端口号  : ");
	Show_One_Line_Parameter(80,5,_userprv->m_tIPinformation.IP.data[3],
	                             _userprv->m_tIPinformation.IP.data[2],
	                             _userprv->m_tIPinformation.IP.data[1],
	                             _userprv->m_tIPinformation.IP.data[0],fc,bc);
	Show_One_Line_Parameter(80,25,_userprv->m_tIPinformation.SUBNET.data[3],
	                             _userprv->m_tIPinformation.SUBNET.data[2],
	                             _userprv->m_tIPinformation.SUBNET.data[1],
	                             _userprv->m_tIPinformation.SUBNET.data[0],fc,bc);
	Show_One_Line_Parameter(80,45,_userprv->m_tIPinformation.NETWORK.data[3],
	                             _userprv->m_tIPinformation.NETWORK.data[2],
	                             _userprv->m_tIPinformation.NETWORK.data[1],
	                             _userprv->m_tIPinformation.NETWORK.data[0],fc,bc);
	Show_One_Line_Parameter(80,65,_userprv->m_tIPinformation.Severs.data[3],
	                             _userprv->m_tIPinformation.Severs.data[2],
	                             _userprv->m_tIPinformation.Severs.data[1],
	                             _userprv->m_tIPinformation.Severs.data[0],fc,bc);
	Show_One_Line_Parameter(80,85,_userprv->m_tIPinformation.DNS.data[3],
	                             _userprv->m_tIPinformation.DNS.data[2],
	                             _userprv->m_tIPinformation.DNS.data[1],
	                             _userprv->m_tIPinformation.DNS.data[0],fc,bc);
  Show_Num(80,105,fc,bc,_userprv->m_tIPinformation.port);
	Show_One_Minate(_userprv,8);
}

