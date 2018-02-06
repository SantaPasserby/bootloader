#ifndef __IAP_H
#define __IAP_H
#include "config.h"
#include "stm32f10x.h"
		
	
typedef enum {
	IAP_OK = 0, 	/* (0) �ɹ� */
	IAP_ADDR_ERR,	/* (1) �̼����ص�ַ�Ƿ� */
	IAP_VECTOR_ERR, /* (2) �ж��������ַ�Ƿ�*/
	IAP_FW_ERR,		/* (3) �̼���Ч */
} IAP_RES;
		
											  
 	
 		
void IAP_Write_Bin(u32 writeaddr, u8 *buf, u32 size);	   // �̼�д��Flash
IAP_RES IAP_Program(u32 addr, u8 *buf, u32 size);		   // �̼�д��Flash
IAP_RES IAP_Load(u32 fwaddr);							   // �̼�����
void IAP_Update(void);									   // �̼�����
#endif
