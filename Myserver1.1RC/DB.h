/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： DB.h
*
* 内容摘要：interface for the DB module
*
* 历史：
*创建日期：2011-8-14
*创建人：	陈磊
\******************************************************************/

#ifndef _DB_H_
#define _DB_H_

#include "public.h"



//exec所需的函数指针
typedef int (*ClBck)(void*, int, char**, char**);
//初始化并打开一个数据库
int DB_open(const char *filename);	
//关闭数据库
int DB_close();							

/***************************************\
*函数名：DB_exec
*
*功能：执行SQL语句
*
*参数：
*	1.statement--要执行的SQL语句
*	2.cbFunc--指向要回调的函数的指针
*	3.context--传递给回调函数的第一个参数
*	4.errmsg--错误信息描述
*
*返回值：0--成功 !0--失败
*
*说明：不用回调时第二、第三参数设为NULL
*	不需要错误信息时errmsg也能设为NULL
****************************************/
int DB_exec(const char *statement, ClBck cbFunc, void* context, char *errmsg);














#endif
