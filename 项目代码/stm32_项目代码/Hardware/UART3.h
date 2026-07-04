#ifndef __UART3_H
#define __UART3_H
#include "stm32f10x_usart.h"
#include "stdint.h"
void Usart3Init(unsigned int uiBaud);
void Uart3Send(unsigned char *p_data, unsigned int uiSize);


#endif

//------------------End of File----------------------------

