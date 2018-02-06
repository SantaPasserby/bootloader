/**
*******************************************************************************************************
*			 			 		 	�����ն�shell-xmodemЭ�����ݴ���							 
*                      			(c) Copyright 2013-2020, xxx��˾, ����
*                              			All Rights Reserved
*  
* 
* @�ļ���	cmd_xmodem.c
* @����  		 
* @�汾 	V1.0
* @����    	2017-09-06
* @˵��   	xmodemЭ�����ݴ���			
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
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
*                                        xmodemЭ���ļ����
*
* ����  : ͨ��xmodemЭ�飬ʵ�ִ����ļ������ݵĴ���
*
* ����  : ���忴cmd_tbl_t�ṹ�塣
*
* ����ֵ: ��
*
* ��ע  : ��
*********************************************************************************************************
*/
int do_xmodem (cmd_tbl_t * a, int b, int c , char* d[])
{
	unsigned char retry = 1;
	unsigned char retry_cnt = 0;
	unsigned char result;
	int addr;
	if((unsigned int)d[0] > b)						/* ����������ƥ��									*/
	{
		printf("\tArguments input error!\n\r");
	}
	else
	{
		if((unsigned int)d[0] == 0)				/* ��������Ϊ1����ʾ��ȡ������Ϣ�ķ���					*/
			printf("\r\n\tPlease input\"help xmodem\" get for help.\n\r");
		else if((unsigned int)d[0] == 1 || 		/* ��������Ϊ1~2��ʵ�����ݴ���������ת				*/
				((unsigned int)d[0] == 2))
												
												/* �жϵ�1�������Ƿ�Ϊ���趨��һ��						*/
			if(!strcasecmp(d[2], "NAK") || !strcasecmp(d[2], "CRC")) 
			{	
				if(((unsigned int )d[0] == 2))	/* ���趨ֵһ�£����жϲ��������Ƿ�Ϊ2��				*/
				{
					addr = get_loadaddr(d[3]);	/* ��2�������ȡ��2������								*/
					if(addr == -1)				/* �����ȡ��2��������ֵΪ-1����˵������ֵ����			*/
					{
						printf("\n\r��ַ��������\r\n");
						return -1;
					}
				}
				else				  			/* ���趨ֵ��һ�£����жϲ�������Ϊ1��					*/
					addr = 0x08009000;			/* ʹ��Ĭ�ϵĵ�ַ0x08009000								*/
				printf("\n\r������...\r\n");
				while(retry)					
				{	
					retry = 0;			  
					XmodemInit(); 
					if(!strcasecmp(d[2], "NAK"))	  /* �ж�XmodemУ�����ͣ�У���ִ����һ��			*/
						result = XmodemRecievePacket(XmodemState.PacketCache, 
													 Xmodem_NAK,
													 0, 
													 (unsigned long)addr);
					else							  /* CRCУ��ִ�иò���								*/
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
							printf("\b\rconnect timeout��try to reconnect: %d", ++retry_cnt);
							if(retry_cnt >= 10)
							{
								retry = 0;
								printf("\n\rconnect failed��\r\n");
							}
							else
								retry = 1;
							break;
						case Xmodem_RetryFail:
							MmodemCancel();
							printf("\n\rreconnect failed��\r\n");
							break;
					}			
				}
			}
			else if(!strcasecmp(d[2], "bin") && 
					(unsigned int)d[0] == 2)		/* ʵ�ֹ̼���ת								*/
			{
			 	IAP_Load(0x08009000);
			}
	}
	return 1;
}												   

