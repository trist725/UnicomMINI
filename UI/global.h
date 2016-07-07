#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "CallBack.h"

//共享内存
SHM_MESSAGE *stu_info=NULL;

//时间源
char time_left[10];			//剩余时间
char time_used[10];			//已用时间

//是否测试判定
int isDebug = 0;			//0.停止  1.运行   2.中断


#endif

