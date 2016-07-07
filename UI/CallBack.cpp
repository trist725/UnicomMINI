/**********************************************************************

*文件名：
		CallBack.cpp
*文件描述：
		业务函数
*创建者：
		黄彬

*创建时间：
    2011-8-5


******************************************************************/

#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "CallBack.h"
#include "Button.h"
#include "Lable.h"
#include "Refresh.h"
#include "Dialogue.h"
#include "Form.h"
#include "global.h"

/*****************************
*函数名：CALLBACK_forward_init
*功能：共享内存创建
*参数：无
*返回值:无
******************************/
void CALLBACK_forward_init()
{
	//创建共享内存
	CALLBACK_shm_create();

	//发送模块初始化
	pthread_t thread_time;
	pthread_create(&thread_time,NULL,CALLBACK_time_refresh,NULL);
}


/*****************************
*函数名：CALLBACK_shm_create
*功能：共享内存创建
*参数：无
*返回值:无
******************************/
void CALLBACK_shm_create()
{
	int shm_id;
    void *shm_addr = NULL;
    shm_id = shmget((key_t)1111,SHM_SIZE,0666|IPC_CREAT);
    if(shm_id < 0)
    {
        perror("shm_id");
        exit(0);
    }
    shm_addr = shmat(shm_id,NULL,0);
    stu_info = (SHM_MESSAGE *)shm_addr;

	CALLBACK_shm_memset();
}

/*****************************
*函数名：CALLBACK_shm_memset
*功能：为共享内存初始化
*参数：无
*返回值:无
******************************/
void CALLBACK_shm_memset()
{
	memset(stu_info,0,sizeof(SHM_MESSAGE));


	strcpy(stu_info->send_connect,"0\0");
	strcpy(stu_info->have_connect,"0\0");
	strcpy(stu_info->business_OK,"0\0");
	strcpy(stu_info->business_FAIL,"0\0");
	strcpy(stu_info->send_bag,"0\0");
	strcpy(stu_info->rec_bag,"0\0");
}


/*****************************
*函数名：CALLBACK_time_refresh
*功能：时间刷新
*参数：无
*返回值:无
******************************/
void* CALLBACK_time_refresh(void *argv)
{

	int t;

	while(1)
	{		
		if (stu_info->isOK && stu_info->isBreak == false)
		{
			sscanf(time_used,"%d",&t);
			sprintf(time_used,"%d",t+1);
			
			sscanf(time_left,"%d",&t);
			sprintf(time_left,"%d",t-1);
			
			//printf("%s %s",time_used,time_left);
			
			if (t == 1)
			{
				stu_info->isOK = false;
			}
		}
		
		sleep(1);
	}
	
	
}



/*****************************
*函数名：CALLBACK_form_init
*功能：窗口创建
*参数：无
*返回值:无
******************************/
void* CALLBACK_form_init(void *form_control)
{
	CForm *form;
	CBaseControl *control;

	
	//调试界面
    form = new CForm(24,80,0,0,CALLBACK_debug);
	
    control = new CLable("测试时长:",1,10,2,4);
    form->addControl(*control);
	control = new CLable(stu_info->debug_time,1,4,2,16);
    form->addControl(*control);
	
    control = new CLable("模拟设备数:",1,12,2,26);
    form->addControl(*control);
	control = new CLable(stu_info->debug_sum,1,4,2,40);
    form->addControl(*control);
	
    control = new CLable("发送间隔(毫秒):",1,16,2,50);
    form->addControl(*control);
	control = new CLable(stu_info->debug_interval,1,10,2,68);
    form->addControl(*control);
	
	control = new CLable("发起连接数:",1,14,5,2);
    form->addControl(*control);
	control = new CLable("成功登录  :",1,14,5,42);
    form->addControl(*control);
	control = new CLable("已测试时间:",1,14,8,2);
    form->addControl(*control);
	control = new CLable("剩余时间  :",1,14,8,42);
    form->addControl(*control);
	control = new CLable("发送交易包:",1,14,11,2);
    form->addControl(*control);
	control = new CLable("收到应答包:",1,14,11,42);
    form->addControl(*control);
	control = new CLable("交易成功  :",1,14,14,2);
    form->addControl(*control);
	control = new CLable("交易失败  :",1,14,14,42);
    form->addControl(*control);
	
	
    control = new CButton("停止测试",3,12,18,15,1);
    form->addControl(*control);
    control = new CButton("打印结果",3,12,18,34,2);
    form->addControl(*control);
	control = new CButton("退    出",3,12,18,53,3);
    form->addControl(*control);

	control = new CRefresh(1,10,5,18,stu_info->send_connect);
	form->addControl(*control);
	control = new CRefresh(1,10,5,58,stu_info->have_connect);
	form->addControl(*control);
	control = new CRefresh(1,10,8,18,time_used);
	form->addControl(*control);
	control = new CRefresh(1,10,8,58,time_left);
	form->addControl(*control);
	control = new CRefresh(1,10,11,18,stu_info->send_bag);
	form->addControl(*control);
	control = new CRefresh(1,10,11,58,stu_info->rec_bag);
	form->addControl(*control);
	control = new CRefresh(1,10,14,18,stu_info->business_OK);
	form->addControl(*control);
	control = new CRefresh(1,10,14,58,stu_info->business_FAIL);
	form->addControl(*control);


	return (void *)form;
}

/*****************************
*函数名：CALLBACK_start_debug
*功能：开始测试回调
*参数：无
*返回值:无
******************************/

int CALLBACK_start_debug(void *form_control)
{
	CForm *form = (CForm *)form_control;
	CForm *form_debug;
	int nReturn ;
	int t;
	int interval;

	//获取参数
	strcpy(stu_info->debug_time,form->getEditCaption(1));
    strcpy(stu_info->debug_sum,form->getEditCaption(2));
	strcpy(stu_info->debug_interval,form->getEditCaption(3));

	sscanf(stu_info->debug_interval,"%d",&interval);

	if (interval == 0)
	{
		CDialogue dialogue(" 时 间 间 隔 不 能 为 零 ",8,32,8,24);;
		dialogue.show();
		
		return 1;
	}

	if (strlen(stu_info->debug_time) != 0 && strlen(stu_info->debug_sum) != 0 && strlen(stu_info->debug_interval) != 0 )
	{
		//获取选择框选择
		stu_info->isAnswer = form->getOption(1);
		stu_info->use_card= form->getOption(2);
		stu_info->buy_card= form->getOption(3);
		stu_info->phone_last= form->getOption(4);
		stu_info->phone_blur= form->getOption(5);
		stu_info->phone_particular= form->getOption(6);

		if (stu_info->use_card == false && stu_info->buy_card == false && stu_info->phone_last == false && stu_info->phone_blur == false && stu_info->phone_particular == false)
		{
			CDialogue dialogue(" 请 至 少 选 择 一 个 功 能 ",8,32,8,24);;
			dialogue.show();
			
			return 1;
		}


		form_debug  = (CForm *)CALLBACK_form_init(form_control);

		strcpy(time_used,"0");
		strcpy(time_left,stu_info->debug_time);

		//进入测试
		isDebug = 1;
		stu_info->isOK = true;
		
		nReturn = form_debug->show();

		form_debug->unshow();

		return nReturn;

	}
	else
	{
		CDialogue dialogue(" 参 数 不 全 ",8,20,8,30);;
		dialogue.show();
		
		return 1;
	}

	
}



/*****************************
*函数名:CALLBACK_parameter
*功能：开始测试的回调函数
*参数：无
*返回值:无
******************************/
int CALLBACK_parameter(void* control,int eventID)
{

    switch(eventID)
    {
    case 1:
        return CALLBACK_start_debug(control);
        break;
    case -1:
        return -1;
        break;


    default:
        break;
    }
}



/*****************************
*函数名:CALLBACK_stop_debug
*功能：暂停的回调函数
*参数：无
*返回值:无
******************************/
int CALLBACK_stop_debug(void *form_control)
{
	CForm *form = (CForm *)form_control;
	//改变按键文字
	//更改刷新框的状态
	if (stu_info->isOK == true)
	{
		
		if (isDebug == 1)
		{
			isDebug = 2;
			stu_info->isBreak = true;
			form->changeButtonCaption("开始测试",1);
			form->changeRefresh();
		}
		else if (isDebug == 2)
		{
			isDebug = 1;
			stu_info->isBreak = false;
			form->changeButtonCaption("停止测试",1);
			form->changeRefresh();
		}

	}

	return 999;	
}

/*****************************
*函数名:CALLBACK_write_debug
*功能：测试报告的回调函数
*参数：无
*返回值:无
******************************/
int CALLBACK_write_debug(void *form_control)
{
	int fw;
	char buffer[50];
	//写下现在几个指针的内容到文件
	fw = open("test.txt",O_CREAT|O_WRONLY|O_TRUNC,0711);
	if(fw < 0)
	{
		perror("open failed");
    }

	//写入
	strcpy(buffer,"发送连接数:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->send_connect,strlen(stu_info->send_connect));
	strcpy(buffer,"\t成功登录:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->have_connect,strlen(stu_info->have_connect));
	strcpy(buffer,"\t已测试时间:");
	write(fw,buffer,strlen(buffer));
	write(fw,time_used,strlen(time_used));
	strcpy(buffer,"\t剩余时间:");
	write(fw,buffer,strlen(buffer));
	write(fw,time_left,strlen(time_left));
	strcpy(buffer,"\t发送交易包:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->send_bag,strlen(stu_info->send_bag));
	strcpy(buffer,"\t收到应答包:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->rec_bag,strlen(stu_info->rec_bag));
	strcpy(buffer,"\t交易成功:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->business_OK,strlen(stu_info->business_OK));
	strcpy(buffer,"\t交易失败:");
	write(fw,buffer,strlen(buffer));
	write(fw,stu_info->business_FAIL,strlen(stu_info->business_FAIL));

	return 999;
}

/*****************************
*函数名:CALLBACK_end_debug
*功能：退出按钮的回调函数
*参数：无
*返回值:无
******************************/
int CALLBACK_end_debug(void *form_control)
{
	CForm *form = (CForm *)form_control;

	//更改刷新框是否刷新
	if (isDebug == 1)
	{
		form->changeRefresh();
	}
	isDebug = 0;
	
	//更改刷新框线程开关
	form->closeRefresh();
	
	CALLBACK_shm_memset();

	return 0;
}


/*****************************
*函数名:CALLBACK_debug
*功能：测试参数界面的回调函数
*参数：无
*返回值:无
******************************/
int CALLBACK_debug(void* control,int eventID)
{

    switch(eventID)
    {
    case 1:
        return CALLBACK_stop_debug(control);
        break;
    case 2:
        return CALLBACK_write_debug(control);
        break;
	case 3:
        return CALLBACK_end_debug(control);
		break;
    default:
        break;
    }
}
