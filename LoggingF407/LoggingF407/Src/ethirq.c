#include "ethirq.h"

 extern struct netif gnetif;
 extern ETH_HandleTypeDef heth;
/**
  * @brief  Rx Transfer completed callbacks.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_TxCpltCallback could be implemented in the user file
  */ 
// while(HAL_ETH_GetReceivedFrame_IT(heth)==HAL_OK)
  ethernetif_input(&gnetif);
}

