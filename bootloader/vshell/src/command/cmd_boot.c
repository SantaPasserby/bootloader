/**
*******************************************************************************************************
*			 			 				命令终端shell-加载特定地址命令								 
*                      			(c) Copyright 2013-2020, xxx公司, 广州
*                              			All Rights Reserved
*  
* 
* @文件名	cmd_boot.c
* @作者  		 
* @版本 	V1.0
* @日期    	2017-09-06
* @说明   	帮助信息提示
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
*
*******************************************************************************************************
*/
/* Includes -----------------------------------------------------------------------------------------*/
#include "cmd_boot.h"
#include "iap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


 
/*
*********************************************************************************************************
*                                        将字符串转为10进制
*
* 描述  : 将16进制的加载地址从字符串中分离，转为10进制。
*
* 参数  : str			指向字符串
*
* 返回值: 无
*
* 备注  : 无
*********************************************************************************************************
*/
int get_loadaddr(const char * str)
{
	unsigned i;
	unsigned char len;
	unsigned int tmp = 0;
	len = strlen(str);
	if (strspn((const char *)str, "0123456789") == len)	/*	判断字符串是否为纯数字						*/
	{
		for (i = 0; i < len; i++) 						/*	从字符串中分离16进制数，将其转为10进制		*/
		{
			tmp += pow(16, len - 1 - i)*(str[i] - 48);
		}
	}
	else
	{												  	/*	从字符串中分离16进制数，将其转为10进制		*/
		if (*str == '0' && len >= 2 && (*(str + 1) == 'x' || *(str + 1) == 'X'))
		{
			for (i = 2; i < len; i++)
			{
				tmp += pow(16, len - 1 - i)*(str[i] - 48);
			}
		}
		else
			return -1; 									/*	地址格式输入有误							*/
	}
	return tmp;
}
 
/*
*********************************************************************************************************
*                                        加载特定地址命令
*
* 描述  : 加载特定地址。
*
* 参数  : 具体看cmd_tbl_t结构体。
*
* 返回值: 加载失败错误值
*
* 备注  : 无
*********************************************************************************************************
*/
int do_boot(cmd_tbl_t * a, int b, int c , char* d[])
{
	unsigned char res = IAP_OK;
	int addr;
	if((unsigned int)d[0] > b)						/* 参数个数不匹配									*/
	{
		printf("\tArguments input error!\n\r");
	}
	else
	{
		switch((unsigned int)d[0])
		{
		 	case 0:
				res = IAP_Load(0x08009000);
				printf("地址输入有误或固件向量表有误!\r\n");
				break;
			case 1:
					addr = get_loadaddr((const char *)d[2]);
					if(addr != -1)
				  		res = IAP_Load((unsigned long)addr);
					printf("地址输入有误或固件向量表有误!\r\n");	
				break;
		}
	}
	return res;
}

