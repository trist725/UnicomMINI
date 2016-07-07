#ifndef _REFRESH_H_
#define _REFRESH_H_


#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include "BaseControl.h"



//typedef void* (*tp_refresh)(void*);

class CRefresh  :public CBaseControl
{
public:
	CRefresh(int height,int width,int scry,int scrx,char *arg);
	virtual ~CRefresh();
	void draw();        //画出窗口
	int getControlType();       //控件类型
	bool changeREFRESH();
	bool changeON();
	void sendSINGAL();
protected:
	//tp_refresh refresh_work;
	static void* refresh(void *point);
	char *argv;
private:
	pthread_mutex_t  mutex_id;
	pthread_cond_t cond_id;
	pthread_t pthread_id;
	bool isREFRESH;
	bool isON;
};

#endif // !defined(AFX_REFRESH_H__5FA6A812_D8FC_48C9_9410_93F4469D4BD8__INCLUDED_)
