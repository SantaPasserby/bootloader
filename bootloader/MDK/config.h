#ifndef __CONFIG_H
#define __CONFIG_H

// IAP���
#define FIRMWARE_LOAD_ADDR			0x08009000   // �̼����ص�ַ
#define FIRMWARE_BUF_SIZE			1024		 // �̼�д�����С

#define FIRMWARE_CACHE_ADDR			0x20003000	 // �̼���������ַ
#define FIRMWARE_CACHE_SIZE			1024*4		 // �̼���������С

#define FIRMWARE_LOAD_MODE			1			 // [0]:�̼���RAM�м���
												 // [1]:�̼���Flash�м���


// ��������أ�������BL���˴�Ϊ�˷���˵���̼���
#define VECTOR_TABLE_ADDR			0x08000000	 // Flash��ַ 0x08000000
#define VECTOR_TABLE_OFFSET			0x9000		 // ������ƫ�Ƶ�ַ





//
#define BOOT_TIME_S 1

#endif
