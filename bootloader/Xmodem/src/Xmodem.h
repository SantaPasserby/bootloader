/**
*******************************************************************************************************
*			 			 				Xmodem协议
*                      (c) Copyright 2013-2020, xxx公司, 广州
*                              All Rights Reserved
*  
* 
* @文件名	Xmodem.h
* @作者  		 
* @版本 	v1.0
* @日期    	2017-09-18 ~ 2017-09-19
* @说明   	目前仅实现PC->MCU的接收。
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
*
*******************************************************************************************************
*/

#ifndef __XMODEM_H
#define __XMODEM_H

/* Exported types ------------------------------------------------------------*/
typedef enum  
{
	Xmodem_No_Err 		= 0,
	Xmodem_RecTimeout 	= 1,
	Xmodem_SetFormat_Timeout 	= 2,
	Xmodem_RetryFail 	= 3,
}XmodemErr; 


typedef struct
{
	unsigned char PacketCache[133];	/*	数据包最大133字节	*/

	unsigned char RecieveFlag;		/*	bit0-SOH, 
										bit1-BLK, 
										bit2-~BLK, 
										bit3-NAK, 
										bit4-Format, 
										bit5-ACK
									 */
	
	unsigned char RetryCnt;		  	/* 用于统计是否超过设定的重发次数	*/
	unsigned int BLK;				/* 记录数据包序号					*/

}XMODEMSTATE;


extern 	XMODEMSTATE XmodemState;

/* Exported define -----------------------------------------------------------*/
#define Xmodem_SOH	0x01
#define Xmodem_EOT	0x04
#define Xmodem_ACK	0x06
#define Xmodem_NAK	0x15
#define Xmodem_CAN	0x18
#define Xmodem_CRC	'C'
/* External variables --------------------------------------------------------*/




/* Exported functions ------------------------------------------------------- */
XmodemErr XmodemInit(void);
XmodemErr XmodemRecievePacket(unsigned char *PacketCache, unsigned char CheckType, unsigned char mark, unsigned int addr);
XmodemErr MmodemCancel(void);

#endif
