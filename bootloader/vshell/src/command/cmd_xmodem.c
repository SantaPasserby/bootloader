/**
*******************************************************************************************************
*			 			 		 	命令终端shell-xmodem协议数据传输							 
*                      			(c) Copyright 2013-2020, xxx公司, 广州
*                              			All Rights Reserved
*  
* 
* @文件名	cmd_xmodem.c
* @作者  		 
* @版本 	V1.0
* @日期    	2017-09-06
* @说明   	xmodem协议数据传输			
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
*
*******************************************************************************************************
*/
/* Includes -----------------------------------------------------------------------------------------*/
#include "cmd_xmodem.h"
#include "cmd_boot.h"
#include "Xmodem.h"
#include "cmd_help.h"

#include "stdio.h"
#include "string.h"
#include "iap.h"


/*
*********************************************************************************************************
*                                        xmodem协议文件输出
*
* 描述  : 通过xmodem协议，实现串口文件及数据的传输
*
* 参数  : 具体看cmd_tbl_t结构体。
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
int do_xmodem (cmd_tbl_t * a, int b, int c , char* d[])
{
	unsigned char retry = 1;
	unsigned char retry_cnt = 0;
	unsigned char result;
	int addr;
	if((unsigned int)d[0] > b)						/* 参数个数不匹配									*/
	{
		printf("\tArguments input error!\n\r");
	}
	else
	{
		if((unsigned int)d[0] == 0)				/* 参数个数为1，提示获取命令信息的方法					*/
			printf("\r\n\tPlease input\"help xmodem\" get for help.\n\r");
		else if((unsigned int)d[0] == 1 || 		/* 参数个数为1~2，实现数据传输或程序跳转				*/
				((unsigned int)d[0] == 2))
												
												/* 判断第1个参数是否为与设定的一致						*/
			if(!strcasecmp(d[2], "NAK") || !strcasecmp(d[2], "CRC")) 
			{	
				if(((unsigned int )d[0] == 2))	/* 与设定值一致，则判断参数个数是否为2个				*/
				{
					addr = get_loadaddr(d[3]);	/* 是2个，则获取第2个参数								*/
					if(addr == -1)				/* 如果获取第2个参数的值为-1，则说明输入值有误			*/
					{
						printf("\n\r地址输入有误\r\n");
						return -1;
					}
				}
				else				  			/* 与设定值不一致，则判断参数个数为1个					*/
					addr = 0x08009000;			/* 使用默认的地址0x08009000								*/
				printf("\n\r连接中...\r\n");
				while(retry)					
				{	
					retry = 0;			  
					XmodemInit(); 
					if(!strcasecmp(d[2], "NAK"))	  /* 判断Xmodem校验类型，校验和执行下一步			*/
						result = XmodemRecievePacket(XmodemState.PacketCache, 
													 Xmodem_NAK,
													 0, 
													 (unsigned long)addr);
					else							  /* CRC校验执行该步骤								*/
						result = XmodemRecievePacket(XmodemState.PacketCache, 
													 Xmodem_CRC, 
													 0,
													 (unsigned long)addr);
					switch(result)
					{
					 	case Xmodem_No_Err: 
							printf("\n\rsuccess!\r\n");
							break;
						case Xmodem_RecTimeout:
							MmodemCancel();
							printf("\n\rrecieve timeout!\r\n");
							break;
						case Xmodem_SetFormat_Timeout:
							printf("\b\rconnect timeout，try to reconnect: %d", ++retry_cnt);
							if(retry_cnt >= 10)
							{
								retry = 0;
								printf("\n\rconnect failed！\r\n");
							}
							else
								retry = 1;
							break;
						case Xmodem_RetryFail:
							MmodemCancel();
							printf("\n\rreconnect failed！\r\n");
							break;
					}			
				}
			}
			else if(!strcasecmp(d[2], "bin") && 
					(unsigned int)d[0] == 2)		/* 实现固件跳转								*/
			{
			 	IAP_Load(0x08009000);
			}
	}
	return 1;
}												   

