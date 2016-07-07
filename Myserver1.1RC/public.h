/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： public.h
*
* 内容摘要： 公用的一个头文件，包含了大部分调用的Linux API所需的
*	     头文件，以及大部分结构体的定义与宏
*
* 历史：
*创建日期：2011-8-13
*创建人：	陈磊
\******************************************************************/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sqlite3.h>
#include <errno.h>
#include <time.h>
//#include <semaphore.h>


#include "protocol.h"

#define LISTEN_QUEUE 10		//监听队列长度
#define THREAD_NUM 7		//初始线程数
#define SERVER_PORT 607		//该服务器使用的端口
#define CLIENT_TIMEOUT 30	//客户端超时上限
#define THREAD_IDLE 0		//线程空闲
#define THREAD_BUSY 1		//线程忙
#define BUF_SIZE 2048		//缓冲大小

//线程启动函数指针
typedef void*(*start_routine) (void*);


//传给业务函数的结构体
typedef struct service_context_t
{
	int index;
	char* recv_pack;
}ser_cont;

//实际工作函数
typedef void (*actual_func) (ser_cont);

//任务队列节点结构
typedef struct que_list_node_t
{
	actual_func actual_task;	//实际要处理的任务函数指针
	ser_cont context;		//传给上述函数的参数
	struct que_list_node_t *next;	//指向下一个队列节点
	struct que_list_node_t *pre;	//指向上一个队列节点
}que_node;

//线程信息
typedef struct thread_info_t
{
	pthread_t thread_id;		//线程ID
	int index;			//索引编号
	pthread_mutex_t mutex_id;	//互斥量
	pthread_cond_t cond_id;		//条件变量
}thread_info;

//线程池
typedef struct thread_pool_t
{	
	//pthread_mutex_t mutex_id;				//互斥锁
	int idle_threads;					//池内空闲线程数
	thread_info* pool_threads;				//指向池内线程的指针
	que_node* que_head;					//任务队列头
	que_node* que_tail;					//任务队列尾
}thread_pool;


#endif








