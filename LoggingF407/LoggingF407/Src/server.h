#ifndef __SERVER_H
#define __SERVER_H

#include "stm32f4xx_hal.h"
#include "lwip/tcp.h"
#include "server.h" 
#include "string.h"


//struct TCP_DATA_Z {
//	unsigned char DATA_Buff[1024];//存放数据包最多100个
//	unsigned int LEN;          //存放有效数据的长度
//};


void TCP_server_init(void);
void TCP_server_out(unsigned char DATA_Buff[], unsigned int LEN);
void TCP_server_IN(struct pbuf *p,unsigned char DATA_Buff[], unsigned short *LEN);


#endif


