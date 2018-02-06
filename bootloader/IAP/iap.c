#include "system_stm32f10x.h"
#include "bsp_flash.h"
#include "iap.h"
#include "bsp_usart1.h"

// �̼�д�뻺��
// Flash��16λ������BL���յ��̼�����8λ��ʽ���գ���Ҫͨ��д��������ݽ���ת��
u16 t_buf[FIRMWARE_BUF_SIZE];	// ����
u16 oldcount=0;			 
u16 applenth=0;	

//  �̼�д��Flash��
// addr: �̼�д��Flash����ʼ��ַ
// buf:	 �̼�����
// size: �̼���С
void IAP_Write_Bin(u32 addr, u8 *buf, u32 size)
{
	u16 t;
	u16 i=0;
	u16 temp;
	u32 fwaddr = addr;  	// �̼��洢��ַ
	u8 *fwbuf = buf;		// �̼�����
	for(t=0; t < size; t += 2)
	{   // ��2���ֽںϳɰ��ֽ�
		temp = (u16)fwbuf[1]<<8;
		temp +=(u16)fwbuf[0];
		fwbuf+=2;				// ָ���2
		t_buf[i++] = temp;	    // ������ֽ�����
		if(i == FIRMWARE_BUF_SIZE)
		{
			i = 0;
			FLASH_Write(fwaddr, t_buf, FIRMWARE_BUF_SIZE);	// д��Flash
			fwaddr+=FIRMWARE_BUF_SIZE*2;	   				// ����Flashд��ַ
		}
	}	
	if(i)  		
		FLASH_Write(fwaddr, t_buf, i);						// ����FIRMWARE_BUF_SIZE����ʣ�µ�д��
}

 


// �̼���д
// addr:�̼���д��ַ
// buf:�̼�����
// size:�̼���С
IAP_RES IAP_Program(u32 addr, u8 *buf, u32 size)
{
	if(((*(vu32*)(0X20003000+4))&0xFF000000)==0x08000000)   // �жϹ̼��Ƿ���Ч
		IAP_Write_Bin(addr, buf, size);					  	// ��Ч������¹̼�
	else 
		return IAP_FW_ERR;									// �̼���Ч
	return IAP_OK; 											// �̼���д�ɹ�
}


// �̼�����
// fwaddr: �̼����ص�ַ
IAP_RES IAP_Load(u32 fwaddr)
{
#if FIRMWARE_LOAD_MODE
	if(((*(vu32*)(fwaddr+4))&0xFF000000)==0x08000000)
	{	
		if(((*(vu32*)fwaddr)&0x2FFE0000)==0x20000000)	   // �ж��ж��������ַ�Ƿ���Ч
		{ 	 
			// ���ù̼����غ��ջ��ַ
			// ջ��ַ����ڹ̼�ͷ4���ֽ���
			MSR_MSP(*(vu32*)fwaddr);
			// ���ع̼�
			printf("\r\n���ڼ��ع̼�...");
			((void(*)(void))*((vu32 *)(fwaddr+4)))();				  							 
		}
		else
			return IAP_VECTOR_ERR;						  // �������ַ����
			
	}
	else
		return IAP_ADDR_ERR;							  // �̼����ص�ַ����
#else
	if(((*(vu32*)(fwaddr+4))&0xFF000000)==0x20000000)	
	{
		if(((*(vu32*)fwaddr)&0x2FFE0000)==0x20000000)	  // �ж��ж��������ַ�Ƿ���Ч
		{ 	 
			// ���ù̼����غ��ջ��ַ
			// ջ��ַ����ڹ̼�ͷ4���ֽ���
			MSR_MSP(*(vu32*)fwaddr);
			// ���ع̼�
			printf("\r\n���ڼ��ع̼�...");
			((void(*)(void))*((vu32 *)(fwaddr+4)))();	 			  							 
		}else
			return IAP_VECTOR_ERR;						  // �������ַ����
	}
	else
		return IAP_ADDR_ERR;						      // �̼����ص�ַ����
#endif
	return IAP_OK;
}													  	  // �̼��ɹ�����













