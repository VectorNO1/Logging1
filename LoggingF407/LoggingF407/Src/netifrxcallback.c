#include "ethernetif.h"
#include "stm32f4xx_hal.h"
#include "lwip.h"
extern struct netif gnetif;

///**
//  * @brief  Get the size of received the received packet.
//  * @param  None
//  * @retval framelength: received packet size 
//  */
//uint32_t ETH_GetRxPktSize(ETH_HandleTypeDef *heth)
//{
//  uint32_t frameLength = 0;
//  if(((heth->RxDesc->Status & ETH_DMARXDESC_OWN) == (uint32_t)RESET) &&
//     ((heth->RxDesc->Status & ETH_DMARXDESC_ES) == (uint32_t)RESET) &&
//     ((heth->RxDesc->Status & ETH_DMARXDESC_LS) != (uint32_t)RESET) &&
//     ((heth->RxDesc->Status & ETH_DMARXDESC_FS) != (uint32_t)RESET))
//  {
//    /* Get the size of the packet: including 4 bytes of the CRC */
//    frameLength = ((heth->RxDesc->Status&ETH_DMARXDESC_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
//  }

// /* Return Frame Length */ 
// return frameLength;
//}


///**
//  * @brief  Rx Transfer completed callbacks.
//  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
//  *         the configuration information for ETHERNET module
//  * @retval None
//  */
//__inline void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
//{
//  /* Prevent unused argument(s) compilation warning */
//  UNUSED(heth);
//	while(ETH_GetRxPktSize(heth)!=0)
//	ethernetif_input(&gnetif);
//  /* NOTE : This function Should not be modified, when the callback is needed,
//  the HAL_ETH_TxCpltCallback could be implemented in the user file
//  */ 
//}

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);
	sys_check_timeouts();
  /* NOTE : This function Should not be modified, when the callback is needed,
            the __HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}
