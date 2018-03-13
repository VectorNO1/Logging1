#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f4xx_hal.h"
#include "user.h"

void Write_FLASH(USERPRV * _userprv);
uint32_t Read_Flash(USERPRV * _userprv);
#endif


