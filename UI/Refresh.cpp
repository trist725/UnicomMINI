/**********************************************************************

*文件名：
		Refresh.cpp
*文件描述：
		CRefresh类的封装
*创建者：
		黄彬

*创建时间：
2011-8-8

******************************************************************/

#include "Refresh.h"

CRefresh::CRefresh(int height,int width,int scry,int scrx,char *arg):CBaseControl(height,width,scry,scrx)
{
	argv = arg;
	isON = true ;
	isREFRESH = true;
	pthread_mutex_init(&mutex_id,NULL);
	pthread_cond_init(&cond_id,NULL);
}

CRefresh::~CRefresh()
{
	if (this->pWin != NULL)
	{
		delwin(this->pWin);
	}
}

/*****************************
*函数名：draw
*功能：画出窗口
*参数：无
*返回值:无
******************************/
void CRefresh::draw()
{
	this->pWin = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
    wrefresh(this->pWin);
	pthread_create(&pthread_id,NULL,refresh,this);

	this->sendSINGAL();
}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CRefresh::getControlType()
{
	return REFRESH;
}


/*****************************
*函数名：refresh
*功能：刷新窗口
*参数：无
*返回值:无
******************************/
void* CRefresh::refresh(void *point)
{
 	CRefresh *refresh_manager = (CRefresh *)point;
	while(refresh_manager->isON == true)
	{
		pthread_cond_wait(&refresh_manager->cond_id,&refresh_manager->mutex_id);
		while (refresh_manager->isREFRESH == true)
		{

			mvwprintw(refresh_manager->pWin,refresh_manager->m_iHeight/2,0,"%-5s",(char *)refresh_manager->argv);
			curs_set(0);
			wrefresh(refresh_manager->pWin);
			usleep(1000);
		}

	}

}



/*****************************
*函数名：changeREFRESH
*功能：更改刷新开关
*参数：无
*返回值:现在的状态
******************************/
bool CRefresh::changeREFRESH()
{
	if (isREFRESH == true)
	{
		isREFRESH = false;
	}
	else
	{
		isREFRESH = true;
	}
}

/*****************************
*函数名：changeON
*功能：更改线程开关
*参数：无
*返回值:现在的状态
******************************/
bool CRefresh::changeON()
{
	isON = false;
}



/*****************************
*函数名：sendSINGAL
*功能：条件变量
*参数：无
*返回值:无
******************************/
void CRefresh::sendSINGAL()
{
	pthread_mutex_lock(&mutex_id);
	pthread_cond_signal(&cond_id);
	pthread_mutex_unlock(&mutex_id);
}






