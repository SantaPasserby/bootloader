/**
*******************************************************************************************************
*			 			 				XmodemЭ��
*                      (c) Copyright 2013-2020, xxx��˾, ����
*                              All Rights Reserved
*  
* 
* @�ļ���	Xmodem.h
* @����  		 
* @�汾 	v1.0
* @����    	2017-09-18 ~ 2017-09-19
* @˵��   	Ŀǰ��ʵ��PC->MCU�Ľ��ա�
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
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
	unsigned char PacketCache[133];	/*	���ݰ����133�ֽ�	*/

	unsigned char RecieveFlag;		/*	bit0-SOH, 
										bit1-BLK, 
										bit2-~BLK, 
										bit3-NAK, 
										bit4-Format, 
										bit5-ACK
									 */
	
	unsigned char RetryCnt;		  	/* ����ͳ���Ƿ񳬹��趨���ط�����	*/
	unsigned int BLK;				/* ��¼���ݰ����					*/

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
