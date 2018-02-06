/**
*******************************************************************************************************
*			 			 					shell命令解析								 
*                      		(c) Copyright 2013-2020, xxx公司, 广州
*                              			All Rights Reserved
*  
* 
* @文件名	vshell.h
* @作者  		 
* @版本 	V1.0
* @日期    	2017-09-06
* @说明   	shell变量类型相关声明
*******************************************************************************************************
* @注意事项
*
*******************************************************************************************************
* @修改记录
*
*******************************************************************************************************
*/
#ifndef __VSHELL_H
#define __VSHELL_H

/* Includes -----------------------------------------------------------------------------------------*/
#include "interger.h"
#include "shell_config.h"

/* Exported types -----------------------------------------------------------------------------------*/
typedef struct cmd_tbl_s cmd_tbl_t;
typedef struct shell_tcb	SHELL_TCB;


struct cmd_tbl_s{
	char *name; 								/* 命令名 											 */
	int maxargs;								/* 最大参数个数 									 */
	int repeatable;								/* 是否自动重复										 */
	int (*cmd) (cmd_tbl_t *, int, int, char *[]);
	char *usage;								/* 简短的帮助信息 									 */

#if CONFIG_SYS_LONGHELP 
	char *help;									/* 较为详细的帮助信息 								 */
#endif

#if CONFIG_AUTO_COMPLETE						/* 命令补全											 */
	int (*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
#endif

};


struct shell_tcb{
	char buf[64];											/*	字符接收缓存区							  */
	int argc;												/*	字符串计数器							  */
	char *argv[10]; 										/*	命令及参数缓存区						  */
};


 




/* functions define -------------------------------------------------------------------------------- */
void parse_file_outer(void);
 
void main_loop(void);
#endif

















//#define EXT_VSHELL_CMD(name)\
//extern  __vshell_cmd_##name;

//#define Struct_Section  __attribute__ ((unused,section (".text")))
//
//#define VSHELL_CMD(name,maxargs,rep,cmd,usage,help)\
//const cmd_tbl_t __vshell_cmd_##name Struct_Section = {#name, maxargs, rep, cmd, usage, help}







