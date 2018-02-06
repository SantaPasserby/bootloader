#ifndef __IAP_H
#define __IAP_H
#include "config.h"
#include "stm32f10x.h"
		
	
typedef enum {
	IAP_OK = 0, 	/* (0) 成功 */
	IAP_ADDR_ERR,	/* (1) 固件加载地址非法 */
	IAP_VECTOR_ERR, /* (2) 中断向量表地址非法*/
	IAP_FW_ERR,		/* (3) 固件无效 */
} IAP_RES;
		
											  
 	
 		
void IAP_Write_Bin(u32 writeaddr, u8 *buf, u32 size);	   // 固件写入Flash
IAP_RES IAP_Program(u32 addr, u8 *buf, u32 size);		   // 固件写入Flash
IAP_RES IAP_Load(u32 fwaddr);							   // 固件加载
void IAP_Update(void);									   // 固件接收
#endif
