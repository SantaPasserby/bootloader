/**
*******************************************************************************************************
*			 			 					shell命令配置								 
*                      		(c) Copyright 2013-2020, xxx公司, 广州
*                              			All Rights Reserved
*  
* 
* @文件名	vshell_list.c
* @作者  		 
* @版本 	V1.0
* @日期    	2017-09-06
* @说明   	该文件实现命令的解析及运行。
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
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

/* 帮助命令，可查看所有命令以及命令的帮助信息				*/
#if	IS_USE_HELP_CMD		/*	help command					*/
	{
		"help",		    /*	命令名							*/
		0,0,			/* 	参数最大个数，是否支持重复命令	*/
		do_help,	  	/*	命令具体实现的函数接口			*/
		"help-display help info",/*	用法帮助信息			*/
		"help-display help info",/* 命令帮助信息			*/
	},

	{
		"?",		    /*	命令名							*/
		0,0,			/* 	参数最大个数，是否支持重复命令	*/
		do_help,	  	/*	命令具体实现的函数接口			*/
		"help-display help info",/*	用法帮助信息			*/
		"help-display help info",/* 命令帮助信息			*/
	},
#endif




/* 版本查询命令，可查看版本号					*/
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

/* 程序跳转命令，跳转到特定区域执行固件程序					*/
#if	IS_USE_BOOT_CMD		/*	boot command					*/
	{
		"boot",		    /*	命令名							*/
		1,0,			/* 	参数最大个数，是否支持重复命令	*/
		do_boot,	  	/*	命令具体实现的函数接口			*/
						/*	用法帮助信息					*/
		"\n\r\tboot	[loadaddr]	-	load the addr of fireware.\r\n\
		boot	-	Load the address 0x08009000 by default.",
						/* 命令帮助信息						*/
		"help-display help info",
	},
#endif



























	0
};










