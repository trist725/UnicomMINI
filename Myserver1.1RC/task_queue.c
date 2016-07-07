/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： DB.h
*
* 内容摘要：implementation for the task_queue module
*
* 历史：
*创建日期：2011-8-14
*创建人：	陈磊
*创建日期：2011-8-24
*修改内容：添加了任务队列保护锁
*修改人：	陈磊
\******************************************************************/

#include "task_queue.h"

//任务队列锁
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
//任务数信号灯
//extern sem_t sem_task;

que_node* queue_add(que_node** head, que_node* tail, actual_func task_func, ser_cont context)
{
	pthread_mutex_lock(&queue_mutex);
	if (tail)
	{
		que_node* temp = tail;
		tail = malloc(sizeof(que_node));
		tail->next = temp;
		tail->pre = NULL;
		temp->pre = tail;
	}
	//tail为NULL则为空队列,添加第一个节点
	else
	{
		tail = malloc(sizeof(que_node));
		*head = tail;
		tail->next = NULL;
		tail->pre = NULL;
	}
	tail->actual_task = task_func;
	tail->context = context;
	//sem_post(&sem_task);
	pthread_mutex_unlock(&queue_mutex);
	return tail;
}

int queue_del(que_node** head)
{
	pthread_mutex_lock(&queue_mutex);
	if (*head)
	{
		que_node *temp = *head;
		*head = temp->pre;
		free(*head);
	}
	pthread_mutex_unlock(&queue_mutex);
	return 1;
}











