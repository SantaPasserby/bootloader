#ifndef __XMODEM_BSP_H
#define __XMODEM_BSP_H
#include "stm32f10x.h"

void uart_send(u8 ch); 
u8 uart_recieve(u8 *ch);
u8 uart_ClearRecieve(void);


#endif
