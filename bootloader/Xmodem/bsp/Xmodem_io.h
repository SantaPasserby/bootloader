/**
*******************************************************************************************************
*			 			 					xmdoemЭ��ײ�������װ								 
*                      		(c) Copyright 2013-2020, xxx��˾, ����
*                              			All Rights Reserved
*  
* 
* @�ļ���	Xmodem_io.h
* @����  		 
* @�汾 	V1.0
* @����    	2017-09-06
* @˵��   	xmodemЭ��ײ��������
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
*
*******************************************************************************************************
*/
 #ifndef __XMODEM_IO_H
#define __XMODEM_IO_H





void Xmodem_BspInit(void);
void Xmodem_SendOneByte(unsigned char OneByte);
unsigned char Xmodem_RecieveByte(unsigned char * RecByte);
void Xmodem_ClearRecieveCache(void);


#endif