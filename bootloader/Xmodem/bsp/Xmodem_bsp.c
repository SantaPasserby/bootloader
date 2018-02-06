#include "Xmodem_bsp.h"
#include "bsp_systick.h"


void uart_send(u8 ch)
{      
	while((USART1->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART1->DR = (u8) ch;      	//дDR,����1����������
}

u8 uart_recieve(u8 *ch)
{
	u32 Tcnt = 0;
	while((USART1->SR&(1<<5))==0 && Tcnt < 300000) //�ȴ����յ�����
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
