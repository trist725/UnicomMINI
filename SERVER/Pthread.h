#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#include <pthread.h>
#include <semaphore.h>
#include "CMyList.h"

#define MAX_THREAD 50
#define MAX_TASK 50

typedef  void*(*tp_thread_callback)(void *);

typedef struct
{
	tp_thread_callback callback;
	void *argv;
}tp_thread_work;


typedef struct
{
	pthread_t id;
	int index;
}thread_info;


class CPthread  
{
public:
	CPthread(int num_threads ,int max_task);
	virtual ~CPthread();
	void add_thread(tp_thread_work *thread_work);
	void close();
protected:
	static void* thread(void* args);
private:
	int i;			//常用变量
	tp_thread_work pthread_work;
	sem_t empty_id;
	sem_t full_id;
	thread_info *threads;
	int nThreads;
	int nTask;
	pthread_mutex_t  mutex_id;		//互斥量
	CMyList mylist;
	bool isON;		//开关

};

#endif // !defined(AFX_PTHREAD_H__01065501_54C3_4A1B_BF77_C0A56AE6395C__INCLUDED_)
