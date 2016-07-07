/**********************************************************************

*文件名：
		connect.cpp
*文件描述：
		CConnect 封装类
*创建者：
		黄彬

*创建时间：
2011-8-11

******************************************************************/

#include <stdio.h>
#include "Connect.h"
#include "global.h"



CConnect ::CConnect (tp_socket_addr address,socket_notify_event read_event,CPthread *pthread)
{
	on = 1;
	index = 0;
	socket_addr = address;
	//connect_sum = max_connect;

	nChoice = true;

	memset(&stSocket_info,0,sizeof(tp_socket_info));
	//初始化
	for(i=0;i<FD_SETSIZE;i++)
	{
		memset(&socket_array[i],0,sizeof(ClientSockets_T));
	}

	rec_event = read_event;

	thread_manager = pthread;

	server_active();
}

CConnect ::~CConnect ()
{
	for (i=0;i<FD_SETSIZE;i++)
	{
		if(socket_array[i].cliSocket != 0)
		{
			close(socket_array[i].cliSocket);
			socket_array[i].cliSocket = 0;
		}
	}
	//容器清零 并添加服务端
	FD_ZERO(&allset);	

	delete socket_addr.ip_addr;
}

/*****************************
*函数名：start
*功能：提供用户开始socket
*参数：无
*返回值:无
******************************/
int CConnect ::start()
{

	service_core();
}


/*****************************
*函数名：stop
*功能：提供用户结束socket
*参数：无
*返回值:无
******************************/
int CConnect ::stop()
{
	nChoice = false;
}


/*****************************
*函数名：server_active
*功能：服务器的配置
*参数：无
*返回值:无
******************************/
int CConnect ::server_active()
{
	int nSend_connect;

	//配置address
	addr_len = sizeof(server_addr);
	server_addr.sin_family =PF_INET;
	server_addr.sin_port = htons(socket_addr.port);
	server_addr.sin_addr.s_addr =inet_addr(socket_addr.ip_addr);

	//容器清零 并添加服务端
	FD_ZERO(&allset);	

	
}


/*****************************
*函数名：add_connect
*功能：添加连接
*参数：无
*返回值:无
******************************/
void CConnect ::add_connect()
{
	//创建连接
	for(i=0;i<FD_SETSIZE;i++)
	{
		if (socket_array[i].cliSocket == 0)
		{

			//配置socket
			socket_array[i].cliSocket = socket(PF_INET,SOCK_STREAM,0);
			setsockopt(socket_array[i].cliSocket,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
			//连接
			connect(socket_array[i].cliSocket,(struct sockaddr *)&server_addr,addr_len);
			//加入管理
			FD_SET(socket_array[i].cliSocket,&allset);
			//设置最大套接字
			if (maxfd < socket_array[i].cliSocket)
			{
				maxfd = socket_array[i].cliSocket;
			}
			
			//更新索引
			if (index < i )
			{
				index = i;
			}

			break;

		}
		
		
	}
}


/*****************************
*函数名：service_core
*功能：服务器核心 IO复用
*参数：无
*返回值:无
******************************/
int CConnect ::service_core()
{

	while(nChoice)
	{
		rset=allset;

		select_return=select(maxfd+1,&rset,NULL,NULL,0);			//延时5秒断开

		//printf("---select %d---\n",select_return);

		if (select_return == 0)
		{	
			break;
		}

		//进行客户端检索 是否有新消息接收
		for (i=0;i<=index ;i++ )
		{
			if ( (ac_socket=socket_array[i].cliSocket)>0)
			{
				if (FD_ISSET(ac_socket,&rset))
				{

					memset(dialogue,0,sizeof(dialogue));
					n_read=recv(ac_socket,dialogue,sizeof(dialogue),MSG_PEEK);

					if (n_read==0)
					{
						close(ac_socket);
						FD_CLR(ac_socket,&allset);
						socket_array[i].cliSocket = 0;
						printf("disconnect!!!\n");
					}
					else if (n_read > 0 )
					{

						if (socket_array[i].HaveUseThread == 0)
						{
							socket_array[i].HaveUseThread == 1;
							stSocket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
							memset(stSocket_info,0,sizeof(tp_socket_info));
							stSocket_info->client_socket_fd= ac_socket;
							stSocket_info->buffer = NULL;

							if (thread_manager != NULL)
							{
								thread_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
								thread_work->callback = ChangeState;
								thread_work->argv = (void *)stSocket_info;
// 								thread_work->callback = rec_event;
// 								thread_work->argv = (void *)stSocket_info;

								thread_manager->add_thread(thread_work);
							}
							else
							{
								rec_event(&stSocket_info);
							}


						}


					}

					//判断事件是否处理完全
					if(--select_return<=0)
					{
						break;
					}
				}
			}
		}

		usleep(100);		//稍微延时 可保国泰民安
	}
}


/*****************************
*函数名：getSocket
*功能：获取套接字
*参数：无
*返回值:无
******************************/
int CConnect ::getSocket(int array_index)
{
	return socket_array[array_index].cliSocket;
}


void* ChangeState(void *argv)
{
	int j= 0;
	tp_socket_info  *socket_info = (tp_socket_info  *)argv;

	rec_event((void *)socket_info);

	for (j=0;j<FD_SETSIZE;j++)
	{
		if (socket_array[j].cliSocket == socket_info->client_socket_fd)
		{
			socket_array[j].HaveUseThread  = 0;
			break;
		}
	}

	if (socket_info->buffer != NULL)
	{
		free(socket_info->buffer);
	}
	free(socket_info);

}
