#include "system_stm32f10x.h"
#include "bsp_flash.h"
#include "iap.h"
#include "bsp_usart1.h"

// 固件写入缓存
// Flash以16位操作，BL接收到固件是以8位形式接收，需要通过写缓存对数据进行转换
u16 t_buf[FIRMWARE_BUF_SIZE];	// 缓存
u16 oldcount=0;			 
u16 applenth=0;	

//  固件写入Flash中
// addr: 固件写入Flash的起始地址
// buf:	 固件缓存
// size: 固件大小
void IAP_Write_Bin(u32 addr, u8 *buf, u32 size)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr = addr;  	// 固件存储地址
	u8 *fwbuf = buf;		// 固件缓存
	for(t=0; t < size; t += 2)
	{   // 将2个字节合成半字节
		temp = (u16)fwbuf[1]<<8;
		temp +=(u16)fwbuf[0];
		fwbuf+=2;				// 指针加2
		t_buf[i++] = temp;	    // 缓存半字节数据
		if(i == FIRMWARE_BUF_SIZE)
		{
			i = 0;
			FLASH_Write(fwaddr, t_buf, FIRMWARE_BUF_SIZE);	// 写入Flash
			fwaddr+=FIRMWARE_BUF_SIZE*2;	   				// 更新Flash写地址
		}
	}	
	if(i)  		
		FLASH_Write(fwaddr, t_buf, i);						// 不足FIRMWARE_BUF_SIZE，将剩下的写入
}

 


// 固件烧写
// addr:固件烧写地址
// buf:固件数据
// size:固件大小
IAP_RES IAP_Program(u32 addr, u8 *buf, u32 size)
{
	if(((*(vu32*)(0X20003000+4))&0xFF000000)==0x08000000)   // 判断固件是否有效
		IAP_Write_Bin(addr, buf, size);					  	// 有效，则更新固件
	else 
		return IAP_FW_ERR;									// 固件无效
	return IAP_OK; 											// 固件烧写成功
}


// 固件加载
// fwaddr: 固件加载地址
IAP_RES IAP_Load(u32 fwaddr)
{
#if FIRMWARE_LOAD_MODE
	if(((*(vu32*)(fwaddr+4))&0xFF000000)==0x08000000)
	{	
		if(((*(vu32*)fwaddr)&0x2FFE0000)==0x20000000)	   // 判断中断向量表地址是否有效
		{ 	 
			// 设置固件加载后的栈地址
			// 栈地址存放在固件头4个字节中
			MSR_MSP(*(vu32*)fwaddr);
			// 加载固件
			printf("\r\n正在加载固件...");
			((void(*)(void))*((vu32 *)(fwaddr+4)))();				  							 
		}
		else
			return IAP_VECTOR_ERR;						  // 向量表地址出错
			
	}
	else
		return IAP_ADDR_ERR;							  // 固件加载地址出错
#else
	if(((*(vu32*)(fwaddr+4))&0xFF000000)==0x20000000)	
	{
		if(((*(vu32*)fwaddr)&0x2FFE0000)==0x20000000)	  // 判断中断向量表地址是否有效
		{ 	 
			// 设置固件加载后的栈地址
			// 栈地址存放在固件头4个字节中
			MSR_MSP(*(vu32*)fwaddr);
			// 加载固件
			printf("\r\n正在加载固件...");
			((void(*)(void))*((vu32 *)(fwaddr+4)))();	 			  							 
		}else
			return IAP_VECTOR_ERR;						  // 向量表地址出错
	}
	else
		return IAP_ADDR_ERR;						      // 固件加载地址出错
#endif
	return IAP_OK;
}													  	  // 固件成功加载













