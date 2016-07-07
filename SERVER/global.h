#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <semaphore.h>
#include "Service.h"

// #ifdef _GLOBAL_
// #define EXTERN_FU
// #else
// #define EXTERN_FU extern
// #endif
// 

//随机数
static int for_send_num = 0;

//文件发送模块条件变量
static pthread_mutex_t mutex_send;
static pthread_cond_t cond_send;

//中断条件变量
static pthread_mutex_t mutex_break;
static pthread_cond_t cond_break;;

//文件接受模块条件变量
static pthread_mutex_t mutex_rec;
static pthread_cond_t cond_rec;

//共享内存
static SHM_MESSAGE *stu_info=NULL;

//判断是否启用应答
static bool server_answer = false;


//数据锁
static pthread_mutex_t  mutex_shm;		//互斥量
static sem_t empty_shm;



#endif

