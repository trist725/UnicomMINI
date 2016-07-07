/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： DB.c
*
* 内容摘要： implementation for the DB module
*
* 历史：
*创建日期：2011-8-20
*创建人：	陈磊
\******************************************************************/

#include "DB.h"

sqlite3 *pDB = NULL;
//sqlite3数据库锁
pthread_mutex_t db_mutex =  PTHREAD_MUTEX_INITIALIZER;

int DB_open(const char *filename)
{
	//pFile = filename;
	
	int err;
	err = sqlite3_open(filename, &pDB);
	return err;			//返回0成功或错误码
}

int DB_close()
{
	int err;
	err = sqlite3_close(pDB);
	return err;
}

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
int DB_exec(const char *statement, ClBck cbFunc, void* context, char *errmsg)
{
	int err;
	pthread_mutex_lock(&db_mutex);
	err = sqlite3_exec(pDB, statement, cbFunc, context, &errmsg);
	pthread_mutex_unlock(&db_mutex);
	if (errmsg)
	{
		sqlite3_free(errmsg);
	}
	
	return err;

}


