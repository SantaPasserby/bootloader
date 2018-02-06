#ifndef __CONFIG_H
#define __CONFIG_H

// IAP相关
#define FIRMWARE_LOAD_ADDR			0x08009000   // 固件加载地址
#define FIRMWARE_BUF_SIZE			1024		 // 固件写缓存大小

#define FIRMWARE_CACHE_ADDR			0x20003000	 // 固件缓存区地址
#define FIRMWARE_CACHE_SIZE			1024*4		 // 固件缓存区大小

#define FIRMWARE_LOAD_MODE			1			 // [0]:固件从RAM中加载
												 // [1]:固件从Flash中加载


// 向量表相关（不属于BL，此处为了方便说明固件）
#define VECTOR_TABLE_ADDR			0x08000000	 // Flash地址 0x08000000
#define VECTOR_TABLE_OFFSET			0x9000		 // 向量表偏移地址





//
#define BOOT_TIME_S 1

#endif
