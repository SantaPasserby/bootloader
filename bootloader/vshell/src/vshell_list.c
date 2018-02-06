/**
*******************************************************************************************************
*			 			 					shell��������								 
*                      		(c) Copyright 2013-2020, xxx��˾, ����
*                              			All Rights Reserved
*  
* 
* @�ļ���	vshell_list.c
* @����  		 
* @�汾 	V1.0
* @����    	2017-09-06
* @˵��   	���ļ�ʵ������Ľ��������С�
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
*
*******************************************************************************************************
*/ 


/* Includes -----------------------------------------------------------------------------------------*/
#include "vshell_list.h"
#include "shell_Config.h"
#include "cmd_help.h"
#include "cmd_version.h"
#include "cmd_xmodem.h" 
#include "cmd_boot.h"




const cmd_tbl_t cmd_tbl_c[] = {

/* ��������ɲ鿴���������Լ�����İ�����Ϣ				*/
#if	IS_USE_HELP_CMD		/*	help command					*/
	{
		"help",		    /*	������							*/
		0,0,			/* 	�������������Ƿ�֧���ظ�����	*/
		do_help,	  	/*	�������ʵ�ֵĺ����ӿ�			*/
		"help-display help info",/*	�÷�������Ϣ			*/
		"help-display help info",/* ���������Ϣ			*/
	},

	{
		"?",		    /*	������							*/
		0,0,			/* 	�������������Ƿ�֧���ظ�����	*/
		do_help,	  	/*	�������ʵ�ֵĺ����ӿ�			*/
		"help-display help info",/*	�÷�������Ϣ			*/
		"help-display help info",/* ���������Ϣ			*/
	},
#endif




/* �汾��ѯ����ɲ鿴�汾��					*/
#if IS_USE_VERSION_CMD	/*	versio command*/
	{
		"version",0,0,do_version,
		"version	-	read the version of the shell.",
		"you may want to gain the version infomation of the shell."
	},
#endif

#if IS_USE_XMODEM_CMD 	/*	Xmodem command*/
	{
		"Xmodem",3,0,do_xmodem,
		"\n\r\tXmodem	[format{NAK/CRC}]	-	Start revieve transform.\r\n\
		Xmodem read		-		read data.",
		"Use Xmodem protocol to revieve files."
	},
#endif

#if IS_USE_LOADX_CMD
	{
		"loadx",3,0,do_xmodem,
		"\n\r\tloadx	[format{NAK/CRC}]	-	Start revieve transform.\r\n\
		Xmodem read		-		read data.",
		"Use Xmodem protocol to revieve files."
	},
#endif

/* ������ת�����ת���ض�����ִ�й̼�����					*/
#if	IS_USE_BOOT_CMD		/*	boot command					*/
	{
		"boot",		    /*	������							*/
		1,0,			/* 	�������������Ƿ�֧���ظ�����	*/
		do_boot,	  	/*	�������ʵ�ֵĺ����ӿ�			*/
						/*	�÷�������Ϣ					*/
		"\n\r\tboot	[loadaddr]	-	load the addr of fireware.\r\n\
		boot	-	Load the address 0x08009000 by default.",
						/* ���������Ϣ						*/
		"help-display help info",
	},
#endif



























	0
};










