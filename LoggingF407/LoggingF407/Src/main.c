/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "i2c.h"
#include "iwdg.h"
#include "lwip.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "server.h"
#include "exit.h"
#include "user.h"
#include "usbd_customhid.h"
#include "lcd.h"
#include "24cxx.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

extern USERPRV m_userprv;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim5;
extern IWDG_HandleTypeDef hiwdg;
uint8_t data[64] = {0xff,0xff,0xff,0x7f,0x00,0x09,0x27,0xc0,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x50,0x51,0x53,0x54};
uint8_t data1[64];
uint8_t ALWAYS_FLAY = 0;
uint8_t  Test5000Meter_Flag = 0;
//uint8_t data[] = "hello the world";
/* USER CODE END PV hh*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void Send_Fixed_Fpga_Data(uint16_t num,uint8_t * buff)
{
	TCP_server_out(buff,num*2);
	Delay_us(800);
}

/* USER CODE END 0 */
	uint8_t qwe=0;
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint16_t cmd;
  static uint16_t temp = 0x1200;
	static uint16_t FpgaNumCunt = 0;
	uint8_t qwe=0;
	static uint32_t TEMP = 0;
	Data_Init(&m_userprv);//数据初始化  里面 有 每个模块的 ID 从0x3001 开始 .
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init(); 
  MX_USB_DEVICE_Init();  //虽然没有用 但是也初始化了。
  MX_LWIP_Init();        //以太网初始化
  MX_ADC1_Init();        //adc 初始化 虽然没有用
  MX_TIM4_Init();        //定时器初始化
  MX_USART1_UART_Init(); //串口初始化
  MX_SPI3_Init();        //SPI 总线初始化
  MX_TIM3_Init();        
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM1_Init();
 // MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
	AT24CXX_Init();             //存储初始化
  //Read_MoRen_Val(&m_userprv); //读取一次存储默认值
  TCP_server_init();           //服务器初始化
  HAL_TIM_Base_Start_IT(&htim3); //开启定时器3中断
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim5);
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);  //开启正交编码的两个通道
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);  //
	htim4.Instance->CNT = 0x0;
	HAL_TIM_Base_Start_IT(&htim4);
	m_userprv.m_uiBMQVal -=0xffff;               
	HAL_TIM_Base_Start_IT(&htim1);               //开启定时器 1中断              
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	// 开始接收中断
	uint8_t USART1_RX_TEMP;
	HAL_UART_Receive_IT(&huart1,&USART1_RX_TEMP,1);
	// 初始化状态灯
	HAL_GPIO_WritePin(LED1_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET);
	//初始化 解码频率。
	HAL_GPIO_WritePin(KA_GPIO_Port,KA_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(KB_GPIO_Port,KB_Pin,GPIO_PIN_SET);
	//初始化WEN
	HAL_GPIO_WritePin(WEN_GPIO_Port,WEN_Pin,GPIO_PIN_SET);	
	Lcd_Init();
	Judge_A_B_Moudel(&m_userprv);
	//设定单字节 还是双字节  默认单字节
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
	//将时间基值设定为0
	htim5.Instance->CNT = 0x0;
	htim4.Instance->CNT = 0x0;
  //关闭SYSTIC 中断 如果不关闭 会影响外部中断
	SysTick->CTRL = 0x0000;
	Zhongzi_Yama_Data_Init(m_userprv.m_tTest_Y,m_userprv.m_tTest_Z);
  Show_IP_Config(&m_userprv,RED,BLUE);//显示10s
  Welcome_Page(&m_userprv);
	// REN 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin =  GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin =  GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_1,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

	//m_userprv.m_tModelDes.type = 0x02;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	 htim4.Instance->CNT = 0x0;
	 m_userprv.m_iTimer4UpdateCnt = 0;
	 m_userprv.m_iTimerCount = 0;
	 m_userprv.m_tModelDes.type = 2;
  while (1)
  {
    A_Moudel_Function(&m_userprv);
		
		//调试5000米线缆专用程序
//		if(Test5000Meter_Flag == 1)
//		{
//			Test5000Meter_Flag = 0;
//		  while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == 1)
//			{
//  			Send_Data_To_FPGA_OneCmd(0x69a4);
//				Delay_ms(100); 
//			}
//		}
		
		//调试专用程序
//		if(ALWAYS_FLAY == 1)
//		{
//			ALWAYS_FLAY = 0;
//		  while(HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_9) == 0)
//			{
//				Send_Data_To_FPGA_DoubleCmd(0x3000,0x5758);
//				Delay_ms(100); 
//			}
//		}
	//Send_Data_To_FPGA_DoubleCmd(0x3000,0x0758);
	//Delay_ms(100); 
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI
                              |RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
		
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
