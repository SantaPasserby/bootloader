/**
*******************************************************************************************************
*			 			 					Xmodem协议
*                      		(c) Copyright 2013-2020, xxx公司, 广州
*                              			All Rights Reserved
*  
* 
* @文件名	Xmodem.c
* @作者  		 
* @版本 	v1.2
* @日期    	2017-09-18 ~ 2017-09-19
* @说明   	无
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
* 20170919
* --------
* 1、改善接收协议。
* 2、添加取消Xmdoem传输函数接口，即CTRL_C。
*
* 20170925
* --------
* 1、改善固件升级中，写入字节数与擦除字节数一致，从原来的128字节改为flash的页大小。速度提升至原先的10~12倍。
*	 200多K的Bin文件，写入时间大约为30s。
* 2、添加固件可烧写至任意位置。
* 3、Xmodem接收协议添加检验，但未添加CRC校验。
* @注意事项
* 20170919
* --------
* 1、信息包序号最初为0x01，之后的序号将从0x00开始到0xFF。
*******************************************************************************************************
*/
/* Includes ------------------------------------------------------------------------------------------*/
#include "Xmodem.h"	
#include "XmodemConfig.h"
#include "Xmodem_io.h"	/*	与平台底层相关			*/
#include "bsp_systick.h"
#include "stdio.h"
#include "bsp_flash.h"
#include "iap.h"
/* Private variables ---------------------------------------------------------------------------------*/

const unsigned char CTR_C[] = {		 				/* 中断取消Xmodem传输							  */
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0x08, 
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00
};	


unsigned char Xmodem_cache[FLASH_PAGE_SIZE];

XMODEMSTATE XmodemState = {{0}, 0, 0, 1};		 	/* Xmdoem管理控制器								  */


/*
*********************************************************************************************************
*                                        取消Xmodem协议传输
*
* 描述  : 发送特定的数据，取消Xmdoem传输，等同于Ctrl+C。
*
* 参数  : 无
*
* 返回值: Xmodem_No_Err		无错误
*
* 备注  : 无
*********************************************************************************************************
*/
XmodemErr MmodemCancel(void)
{	
	const unsigned char * cancel_ptr;
	cancel_ptr = CTR_C;
	while(*cancel_ptr != 0x00)
		Xmodem_SendOneByte(*cancel_ptr++);
	Xmodem_SendOneByte(*cancel_ptr);
	return Xmodem_No_Err;	
}


/*
*********************************************************************************************************
*                                        Xmodem协议-初始化
*
* 描述  : 初始化底层驱动，清空接口缓存，重发计数器、序号计数器复位。
*
* 参数  : 无
*
* 返回值: Xmodem_No_Err		无错误
*
* 备注  : 无
*********************************************************************************************************
*/
XmodemErr XmodemInit(void)
{
	Xmodem_BspInit();
	Xmodem_ClearRecieveCache();	
	XmodemState.RetryCnt = 0;
	XmodemState.BLK = 1;
	return Xmodem_No_Err;
}
/*
*********************************************************************************************************
*                                        Xmodem协议-发送数据包
*
* 描述  : 该发送数据包接口包含校验类型选择。
*
* 参数  : PacketDate		待发送的包数据
*		  CheckType			校验类型
*
* 返回值: 无
*
* 备注  : Xmodem协议默认一个数据包中的数据段为128字节。Xmodem-1K则为1024字节。该函数接口仅支持128字节。
*********************************************************************************************************
*/
XmodemErr XmodemSendPacket(unsigned char *PacketData, unsigned char CheckType)
{
	return Xmodem_No_Err;	
}

/*
*********************************************************************************************************
*                                        Xmodem协议-接收数据包
*
* 描述  : 该接收数据包接口包含校验类型选择。
*
* 参数  : PacketDate		接收缓存地址
*		  CheckType			校验类型
*								Xmodem_NAK		采用校验和数据包格式				
*								Xmodem_CRC	 	采用CRC数据包格式						
*
* 返回值: 无
*
* 备注  : Xmodem协议默认一个数据包中的数据段为128字节。Xmodem-1K则为1024字节。该函数接口仅支持128字节。
*
* 思路	：1、首先发送'C'或者NAK来确定接收的数据包格式。
*********************************************************************************************************
*/
XmodemErr XmodemRecievePacket(unsigned char *PacketCache, unsigned char CheckType, unsigned char mark, unsigned int addr)
{
	unsigned char *PacketTemp;
	unsigned int w_prt = addr;
	unsigned int block_cnt = 0;
	unsigned int cnt = 0;
	unsigned int sum = 0;
	PacketTemp = PacketCache;
	Xmodem_SendOneByte(CheckType);								/* 发送'C'或NAK确定数据包格式			*/
	XmodemState.RecieveFlag = 1<<4;							   	/* 标记格式字符已发送					*/
	while(1)
	{
		if(XmodemState.RecieveFlag & 1<<3)						/* 发送NAK或重发NAK						*/
		{
			Xmodem_SendOneByte(Xmodem_NAK);										
		}
																/* 发送ACK或重发ACK						*/
		if(XmodemState.RecieveFlag & 1<<5)										
		{
			Xmodem_SendOneByte(Xmodem_ACK);
		}


		if(XmodemState.RecieveFlag & 1<<4)						/* 是否首次传输，是则判断是否有数据回应 */
		{
			if(Xmodem_RecieveByte(PacketTemp) == Xmodem_RecTimeout) 
																/* 接收数据是否超时						*/																										
				return Xmodem_SetFormat_Timeout;				/* 是，返回设置格式超时错误信息			*/
			else
				XmodemState.RecieveFlag = 0;				    /* 否，表示接收成功，则清空所有标志		*/
		}
		else
		{	/* 判断是否接收超时																			*/
			if(Xmodem_RecieveByte(PacketTemp))					/* 是接收超时*/
			{
				XmodemState.RetryCnt++;						   	/* 统计重发次数							*/
				XmodemState.RecieveFlag = 1<<3;					/* 标记，发送NAK进行重新接收 			*/
				if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* 重复次数过多							*/
					return Xmodem_RetryFail;					/* 返回重发失败			 				*/				
			}
			else												/* 非接收超时，已得到数据				*/
			{
				XmodemState.RetryCnt = 0; 						/* 清除重发次数							*/
				XmodemState.RecieveFlag = 0;					/* 清空标志								*/
			}	
		}
	

		/* 是否已接收过SOH包头																			*/
		if(!(XmodemState.RecieveFlag & 0x01))					/* 若没接收过，进行一下处理				*/
		{
			if(*PacketTemp == Xmodem_SOH)						/* 没有接收过，则判断此次接收的是否为SOH*/
			{
				XmodemState.RetryCnt = 0; 						/* 清除重发次数							*/
				XmodemState.RecieveFlag |= 0x01;				/* 是，则标记接收到SOH					*/
			}
			else if(*PacketTemp == Xmodem_EOT)					/* 若不是SOH，再判断是否为EOT结束符		*/
			{
				if(block_cnt != 0)
					IAP_Write_Bin(w_prt, Xmodem_cache, block_cnt*128);

//				XmodemState.RecieveFlag = 1<<5;				 	/* 接收到EOT，直接应答					*/
				Xmodem_SendOneByte(Xmodem_ACK);					/* 是EOT，直接发送ACK，表示接收完成		*/
				Xmodem_SendOneByte(Xmodem_ACK);
				Xmodem_SendOneByte(Xmodem_ACK);					
				return Xmodem_No_Err;							/* 返回无错误，表示接收完成				*/ 
			}
			else												/* 既不是SOH，也不是EOT，则判断此处接收，
																   错误，重新发送						*/
			{	
				XmodemState.RecieveFlag = 1<<3;				    /* 标记重发标志							*/
				XmodemState.RetryCnt++;							/* 记录重发次数							*/
				if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* 重复次数过多							*/
					return Xmodem_RetryFail;					/* 返回重发失败			 				*/
			}
		}

		/* 若已成功接收到信息包头SOH，则开始接收其后的数据												*/
		while(XmodemState.RecieveFlag & 0x01)					/* 开始接收数据							*/
		{
			PacketTemp++;	   									/* 更新接收缓存地址						*/			
//			printf("[%d]\r\n", PacketTemp - PacketCache);		
			/* 若接收超时, 需重新接收   																*/
			if(Xmodem_RecieveByte(PacketTemp))									
			{
				XmodemState.RecieveFlag = 1<<3;					/* 标记重新接收，清除SOH标志			*/
				PacketTemp = PacketCache;						/* 重置接收缓存，重新开始接收			*/
				break;											/* 退出循环								*/
			}
			/* 若已成功接收到块序号信息，则校验需接收的序号与接收到的序号是否一致						*/
			if(!(XmodemState.RecieveFlag & 0x02))				
			{
//				printf("[BLK %d]\r\n",*PacketTemp);
				if(*PacketTemp == (unsigned char)XmodemState.BLK)				/* 判断序号是否一致						*/
					XmodemState.RecieveFlag |= 0x02;		   	/* 一致，则标记序号无误					*/
				else											/* 不一致，则需重新接收该序号的信息包	*/
				{
					XmodemState.RecieveFlag = 1<<3;			    /* 重新接收信息包						*/
					PacketTemp = PacketCache;					/* 重置接收缓存地址						*/
					break;										/* 退出本次循环							*/
				}
			}
			/* 统计接收到的数据个数，若已完整接收一次序号的信息包，则需发送NAK，接收下一序号的信息包	*/
			switch(CheckType)
			{
				case Xmodem_NAK:
					if((PacketTemp - PacketCache) == 131)		/* 校验和信息包格式，132字节			*/
					{											/* 已成功接收132字节					*/
						for(cnt = 3; cnt < 131; cnt++)
						{
							sum += PacketCache[cnt];		
						}
						if((unsigned char ) sum == PacketCache[131])
						{
										   	
							XmodemState.RecieveFlag = 1<<5;			/* 标记发送ACK							*/
							XmodemState.BLK++;						/* 下次需接收的信息包序号更新			*/
						}
						else
						{
							XmodemState.RetryCnt++;
							XmodemState.RecieveFlag = 1<<3;					/* 标记，发送NAK进行重新接收 			*/
							if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* 重复次数过多							*/
								return Xmodem_RetryFail;					/* 返回重发失败			 				*/
							XmodemState.RecieveFlag = 1<<3;	
								
						}
						sum = 0;
						PacketTemp = PacketCache;	   						/* 重置接收缓存							*/
						switch(mark)
						{
							case 0:
								for(cnt = 0; cnt < 128; cnt++)
									Xmodem_cache[128*block_cnt + cnt] = PacketCache[3 +cnt];
								block_cnt++;
								if(block_cnt == (FLASH_PAGE_SIZE/128))
								{
									IAP_Write_Bin(w_prt, Xmodem_cache, FLASH_PAGE_SIZE);
									w_prt+=FLASH_PAGE_SIZE;
									block_cnt = 0;
								}
							break;
						}
					}
					break;
				case Xmodem_CRC:
					if((PacketTemp - PacketCache) == 132)		/* CRC校验信息包格式，133字节			*/
					{
																/* 已成功接收133字节					*/
						PacketTemp = PacketCache;			   	/* 重置接收缓存							*/
						XmodemState.RecieveFlag = 1<<5;			/* 标记发送ACK							*/
						XmodemState.BLK++;						/* 下次需接收的信息包序号更新			*/						
					}
					break;
			}
		}
	}

 }






















