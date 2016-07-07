/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： thread_pool.c
*
* 内容摘要： implementation for the ThreadPool module
*				
* 历史：
*创建日期：2011-8-19
*创建人：	陈磊
*修订历史: 2011-8-23
*修订人:	陈磊
*修订内容:改进部分实现
\******************************************************************/

#include "thread_pool.h"
#include "init.h"

//线程池地址
thread_pool* pool = NULL;
//线程状态位图结构
char thread_stat[THREAD_NUM];
//监控线程ID
pthread_t breathe_id = 0;
//队列任务数信号灯
//sem_t sem_task;

void thread_init()
{
	int i;
	int shm_id;
	void* shm_addr = NULL;

	//创建心跳处理线程
	pthread_create(&breathe_id, NULL, monitor, NULL);

	//pthread_create(&breathe_id, NULL, fix, NULL);

	//分配池空间
	pool = (thread_pool*)malloc(sizeof(thread_pool));

	//sem_init(&sem_task, 0, 0);
	
	memset(pool, 0, sizeof(thread_pool));
	//分配池内线程空间
	pool->pool_threads = (thread_info*)malloc(sizeof(thread_info) * THREAD_NUM);

	//创建线程
	for (i = 0; i < THREAD_NUM; i++)
	{
		pool->pool_threads[i].index = i;
		pthread_create(&(pool->pool_threads[i].thread_id), NULL, routine_func, (void*)(&pool->pool_threads[i].index));
	}
	//设置池内所有线程状态,0-idle,1-busy
	memset(thread_stat, 0, sizeof(thread_stat));
}

void* routine_func(void* context)
{
	
	int index = *((int*)context);
	pthread_mutex_init(&pool->pool_threads[index].mutex_id, NULL);		
	pthread_cond_init(&pool->pool_threads[index].cond_id, NULL);
	pthread_mutex_lock(&pool->pool_threads[index].mutex_id);
	ser_cont task_context;
	actual_func actual_task = NULL;
	while(1)
	{
		//线程开始后睡眠等待唤醒
		pthread_cond_wait(&pool->pool_threads[index].cond_id, &pool->pool_threads[index].mutex_id);
		//sem_wait(&sem_task);
		//printf("thread %d is running..\n", pool->pool_threads[index].index);
		if (pool->que_head)
		{
			//取出任务函数和参数
			actual_task = pool->que_head->actual_task;
			task_context = pool->que_head->context;
			//将该任务从任务队列删除
			queue_del(&pool->que_head);
			//执行任务
			actual_task(task_context);
		}
		thread_stat[index] = THREAD_IDLE;
	}



}







