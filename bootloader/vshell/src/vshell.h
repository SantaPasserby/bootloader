/**
*******************************************************************************************************
*			 			 					shell�������								 
*                      		(c) Copyright 2013-2020, xxx��˾, ����
*                              			All Rights Reserved
*  
* 
* @�ļ���	vshell.h
* @����  		 
* @�汾 	V1.0
* @����    	2017-09-06
* @˵��   	shell���������������
*******************************************************************************************************
* @ע������
*
*******************************************************************************************************
* @�޸ļ�¼
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
	char *name; 								/* ������ 											 */
	int maxargs;								/* ���������� 									 */
	int repeatable;								/* �Ƿ��Զ��ظ�										 */
	int (*cmd) (cmd_tbl_t *, int, int, char *[]);
	char *usage;								/* ��̵İ�����Ϣ 									 */

#if CONFIG_SYS_LONGHELP 
	char *help;									/* ��Ϊ��ϸ�İ�����Ϣ 								 */
#endif

#if CONFIG_AUTO_COMPLETE						/* ���ȫ											 */
	int (*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
#endif

};


struct shell_tcb{
	char buf[64];											/*	�ַ����ջ�����							  */
	int argc;												/*	�ַ���������							  */
	char *argv[10]; 										/*	�������������						  */
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







