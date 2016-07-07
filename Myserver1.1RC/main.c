/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： main.c
*
* 内容摘要： main function
*
* 历史：
*创建日期：2011-8-18
*创建人：	陈磊 (Tristone)
\******************************************************************/
#include "init.h"
#include "DB.h"


int main(int argc, char* argv[])
{
	char* dbname = "mini_server.db";
	printf("use local port：%d\nuse database : /%s\n", SERVER_PORT, dbname);
	thread_init();
	DB_open(dbname);	//可以用argv,待扩展
	socket_server_startup();




	return 0;
}










