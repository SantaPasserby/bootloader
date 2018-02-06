#include "Xmodem_bsp.h"
#include "bsp_systick.h"


void uart_send(u8 ch)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
}

u8 uart_recieve(u8 *ch)
{
	u32 Tcnt = 0;
	while((USART1->SR&(1<<5))==0 && Tcnt < 300000) //等待接收到数据
	{
		Tcnt++;
	  	delay_us(10);
	}
	if(Tcnt >= 300000)
		return 1;
	else
	{
		*ch = USART1->DR;
		return 0;
	}
}


u8 uart_ClearRecieve(void)
{
	return USART1->DR;
}
