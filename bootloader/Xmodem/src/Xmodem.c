/**
*******************************************************************************************************
*			 			 					XmodemЭ��
*                      		(c) Copyright 2013-2020, xxx��˾, ����
*                              			All Rights Reserved
*  
* 
* @�ļ���	Xmodem.c
* @����  		 
* @�汾 	v1.2
* @����    	2017-09-18 ~ 2017-09-19
* @˵��   	��
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
* 20170919
* --------
* 1�����ƽ���Э�顣
* 2�����ȡ��Xmdoem���亯���ӿڣ���CTRL_C��
*
* 20170925
* --------
* 1�����ƹ̼������У�д���ֽ���������ֽ���һ�£���ԭ����128�ֽڸ�Ϊflash��ҳ��С���ٶ�������ԭ�ȵ�10~12����
*	 200��K��Bin�ļ���д��ʱ���ԼΪ30s��
* 2����ӹ̼�����д������λ�á�
* 3��Xmodem����Э����Ӽ��飬��δ���CRCУ�顣
* @ע������
* 20170919
* --------
* 1����Ϣ��������Ϊ0x01��֮�����Ž���0x00��ʼ��0xFF��
*******************************************************************************************************
*/
/* Includes ------------------------------------------------------------------------------------------*/
#include "Xmodem.h"	
#include "XmodemConfig.h"
#include "Xmodem_io.h"	/*	��ƽ̨�ײ����			*/
#include "bsp_systick.h"
#include "stdio.h"
#include "bsp_flash.h"
#include "iap.h"
/* Private variables ---------------------------------------------------------------------------------*/

const unsigned char CTR_C[] = {		 				/* �ж�ȡ��Xmodem����							  */
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x08, 0x08, 
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00
};	


unsigned char Xmodem_cache[FLASH_PAGE_SIZE];

XMODEMSTATE XmodemState = {{0}, 0, 0, 1};		 	/* Xmdoem���������								  */


/*
*********************************************************************************************************
*                                        ȡ��XmodemЭ�鴫��
*
* ����  : �����ض������ݣ�ȡ��Xmdoem���䣬��ͬ��Ctrl+C��
*
* ����  : ��
*
* ����ֵ: Xmodem_No_Err		�޴���
*
* ��ע  : ��
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
*                                        XmodemЭ��-��ʼ��
*
* ����  : ��ʼ���ײ���������սӿڻ��棬�ط�����������ż�������λ��
*
* ����  : ��
*
* ����ֵ: Xmodem_No_Err		�޴���
*
* ��ע  : ��
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
*                                        XmodemЭ��-�������ݰ�
*
* ����  : �÷������ݰ��ӿڰ���У������ѡ��
*
* ����  : PacketDate		�����͵İ�����
*		  CheckType			У������
*
* ����ֵ: ��
*
* ��ע  : XmodemЭ��Ĭ��һ�����ݰ��е����ݶ�Ϊ128�ֽڡ�Xmodem-1K��Ϊ1024�ֽڡ��ú����ӿڽ�֧��128�ֽڡ�
*********************************************************************************************************
*/
XmodemErr XmodemSendPacket(unsigned char *PacketData, unsigned char CheckType)
{
	return Xmodem_No_Err;	
}

/*
*********************************************************************************************************
*                                        XmodemЭ��-�������ݰ�
*
* ����  : �ý������ݰ��ӿڰ���У������ѡ��
*
* ����  : PacketDate		���ջ����ַ
*		  CheckType			У������
*								Xmodem_NAK		����У������ݰ���ʽ				
*								Xmodem_CRC	 	����CRC���ݰ���ʽ						
*
* ����ֵ: ��
*
* ��ע  : XmodemЭ��Ĭ��һ�����ݰ��е����ݶ�Ϊ128�ֽڡ�Xmodem-1K��Ϊ1024�ֽڡ��ú����ӿڽ�֧��128�ֽڡ�
*
* ˼·	��1�����ȷ���'C'����NAK��ȷ�����յ����ݰ���ʽ��
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
	Xmodem_SendOneByte(CheckType);								/* ����'C'��NAKȷ�����ݰ���ʽ			*/
	XmodemState.RecieveFlag = 1<<4;							   	/* ��Ǹ�ʽ�ַ��ѷ���					*/
	while(1)
	{
		if(XmodemState.RecieveFlag & 1<<3)						/* ����NAK���ط�NAK						*/
		{
			Xmodem_SendOneByte(Xmodem_NAK);										
		}
																/* ����ACK���ط�ACK						*/
		if(XmodemState.RecieveFlag & 1<<5)										
		{
			Xmodem_SendOneByte(Xmodem_ACK);
		}


		if(XmodemState.RecieveFlag & 1<<4)						/* �Ƿ��״δ��䣬�����ж��Ƿ������ݻ�Ӧ */
		{
			if(Xmodem_RecieveByte(PacketTemp) == Xmodem_RecTimeout) 
																/* ���������Ƿ�ʱ						*/																										
				return Xmodem_SetFormat_Timeout;				/* �ǣ��������ø�ʽ��ʱ������Ϣ			*/
			else
				XmodemState.RecieveFlag = 0;				    /* �񣬱�ʾ���ճɹ�����������б�־		*/
		}
		else
		{	/* �ж��Ƿ���ճ�ʱ																			*/
			if(Xmodem_RecieveByte(PacketTemp))					/* �ǽ��ճ�ʱ*/
			{
				XmodemState.RetryCnt++;						   	/* ͳ���ط�����							*/
				XmodemState.RecieveFlag = 1<<3;					/* ��ǣ�����NAK�������½��� 			*/
				if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* �ظ���������							*/
					return Xmodem_RetryFail;					/* �����ط�ʧ��			 				*/				
			}
			else												/* �ǽ��ճ�ʱ���ѵõ�����				*/
			{
				XmodemState.RetryCnt = 0; 						/* ����ط�����							*/
				XmodemState.RecieveFlag = 0;					/* ��ձ�־								*/
			}	
		}
	

		/* �Ƿ��ѽ��չ�SOH��ͷ																			*/
		if(!(XmodemState.RecieveFlag & 0x01))					/* ��û���չ�������һ�´���				*/
		{
			if(*PacketTemp == Xmodem_SOH)						/* û�н��չ������жϴ˴ν��յ��Ƿ�ΪSOH*/
			{
				XmodemState.RetryCnt = 0; 						/* ����ط�����							*/
				XmodemState.RecieveFlag |= 0x01;				/* �ǣ����ǽ��յ�SOH					*/
			}
			else if(*PacketTemp == Xmodem_EOT)					/* ������SOH�����ж��Ƿ�ΪEOT������		*/
			{
				if(block_cnt != 0)
					IAP_Write_Bin(w_prt, Xmodem_cache, block_cnt*128);

//				XmodemState.RecieveFlag = 1<<5;				 	/* ���յ�EOT��ֱ��Ӧ��					*/
				Xmodem_SendOneByte(Xmodem_ACK);					/* ��EOT��ֱ�ӷ���ACK����ʾ�������		*/
				Xmodem_SendOneByte(Xmodem_ACK);
				Xmodem_SendOneByte(Xmodem_ACK);					
				return Xmodem_No_Err;							/* �����޴��󣬱�ʾ�������				*/ 
			}
			else												/* �Ȳ���SOH��Ҳ����EOT�����жϴ˴����գ�
																   �������·���						*/
			{	
				XmodemState.RecieveFlag = 1<<3;				    /* ����ط���־							*/
				XmodemState.RetryCnt++;							/* ��¼�ط�����							*/
				if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* �ظ���������							*/
					return Xmodem_RetryFail;					/* �����ط�ʧ��			 				*/
			}
		}

		/* ���ѳɹ����յ���Ϣ��ͷSOH����ʼ������������												*/
		while(XmodemState.RecieveFlag & 0x01)					/* ��ʼ��������							*/
		{
			PacketTemp++;	   									/* ���½��ջ����ַ						*/			
//			printf("[%d]\r\n", PacketTemp - PacketCache);		
			/* �����ճ�ʱ, �����½���   																*/
			if(Xmodem_RecieveByte(PacketTemp))									
			{
				XmodemState.RecieveFlag = 1<<3;					/* ������½��գ����SOH��־			*/
				PacketTemp = PacketCache;						/* ���ý��ջ��棬���¿�ʼ����			*/
				break;											/* �˳�ѭ��								*/
			}
			/* ���ѳɹ����յ��������Ϣ����У������յ��������յ�������Ƿ�һ��						*/
			if(!(XmodemState.RecieveFlag & 0x02))				
			{
//				printf("[BLK %d]\r\n",*PacketTemp);
				if(*PacketTemp == (unsigned char)XmodemState.BLK)				/* �ж�����Ƿ�һ��						*/
					XmodemState.RecieveFlag |= 0x02;		   	/* һ�£������������					*/
				else											/* ��һ�£��������½��ո���ŵ���Ϣ��	*/
				{
					XmodemState.RecieveFlag = 1<<3;			    /* ���½�����Ϣ��						*/
					PacketTemp = PacketCache;					/* ���ý��ջ����ַ						*/
					break;										/* �˳�����ѭ��							*/
				}
			}
			/* ͳ�ƽ��յ������ݸ�����������������һ����ŵ���Ϣ�������跢��NAK��������һ��ŵ���Ϣ��	*/
			switch(CheckType)
			{
				case Xmodem_NAK:
					if((PacketTemp - PacketCache) == 131)		/* У�����Ϣ����ʽ��132�ֽ�			*/
					{											/* �ѳɹ�����132�ֽ�					*/
						for(cnt = 3; cnt < 131; cnt++)
						{
							sum += PacketCache[cnt];		
						}
						if((unsigned char ) sum == PacketCache[131])
						{
										   	
							XmodemState.RecieveFlag = 1<<5;			/* ��Ƿ���ACK							*/
							XmodemState.BLK++;						/* �´�����յ���Ϣ����Ÿ���			*/
						}
						else
						{
							XmodemState.RetryCnt++;
							XmodemState.RecieveFlag = 1<<3;					/* ��ǣ�����NAK�������½��� 			*/
							if(XmodemState.RetryCnt >= RETRY_MAX_TIMES)		/* �ظ���������							*/
								return Xmodem_RetryFail;					/* �����ط�ʧ��			 				*/
							XmodemState.RecieveFlag = 1<<3;	
								
						}
						sum = 0;
						PacketTemp = PacketCache;	   						/* ���ý��ջ���							*/
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
					if((PacketTemp - PacketCache) == 132)		/* CRCУ����Ϣ����ʽ��133�ֽ�			*/
					{
																/* �ѳɹ�����133�ֽ�					*/
						PacketTemp = PacketCache;			   	/* ���ý��ջ���							*/
						XmodemState.RecieveFlag = 1<<5;			/* ��Ƿ���ACK							*/
						XmodemState.BLK++;						/* �´�����յ���Ϣ����Ÿ���			*/						
					}
					break;
			}
		}
	}

 }






















