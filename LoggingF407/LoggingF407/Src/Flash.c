#include "flash.h"
#include "user.h"
/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_7
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_7  +  GetSectorSize(ADDR_FLASH_SECTOR_7) -1 
/**
  * @brief  读取参数设置
  * @param  
  *              
  * @retval 
  */
uint32_t Read_Flash(USERPRV * _userprv)
{
	uint32_t Address = FLASH_USER_START_ADDR;
	_userprv->m_tIPinformation.WriteFlag = *(__IO uint32_t*)Address;
	Address +=4;
	_userprv->m_tIPinformation.IP.val = *(__IO uint32_t*)Address;
	Address +=4;
	_userprv->m_tIPinformation.SUBNET.val = *(__IO uint32_t*)Address;
	Address +=4;
	_userprv->m_tIPinformation.NETWORK.val = *(__IO uint32_t*)Address;	 
	Address +=4;
	_userprv->m_tIPinformation.DNS.val = *(__IO uint32_t*)Address;	 
	Address +=4;
	_userprv->m_tIPinformation.Severs.val = *(__IO uint32_t*)Address;
	Address +=4;
	_userprv->m_tIPinformation.port = *(__IO uint32_t*)Address;
	return _userprv->m_tIPinformation.WriteFlag;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
	else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
   else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_5))*/
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}

/**
  * @brief  写 FLASH
  * @param  
  *              
  * @retval 
  */
void Write_FLASH(USERPRV * _userprv)
{
  uint32_t FirstSector = 0, NbOfSectors = 0;
	uint32_t Address = 0;
  uint32_t SectorError = 0;
	/* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();
  /* Get the 1st sector to erase */
  FirstSector = GetSector(FLASH_USER_START_ADDR);
	  /* Get the number of sector to erase from 1st sector*/
  NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
 /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
	
	 if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /* 
      Error occurred while sector erase. 
      User can add here some code to deal with this error. 
      SectorError will contain the faulty sector and then to know the code error on this sector,
      user can call function 'HAL_FLASH_GetError()'
    */
    /*
      FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
    */
    Error_Handler();
  }
	 /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  __HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();

  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();
	Address = FLASH_USER_START_ADDR;
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.WriteFlag) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.IP.val) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.SUBNET.val) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.NETWORK.val) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.DNS.val) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.Severs.val) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, _userprv->m_tIPinformation.port) == HAL_OK)
	{
		Address = Address + 4;
	}
	else
	{ 
		Error_Handler();
	}
  HAL_FLASH_Lock(); 
}

