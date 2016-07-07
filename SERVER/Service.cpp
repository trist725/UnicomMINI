/**********************************************************************

*文件名：
		Service.cpp
*文件描述：
		业务函数
*创建者：
		黄彬

*创建时间：
    2011-8-5


******************************************************************/

#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "Pthread.h"
#include "Service.h"
#include "mini_public.h"
#include "global.h"
#include "Connect.h"


// connect管理
static CConnect *myconnect;
//
// 接收线程池
static CPthread *thread_rec;
//
// 发送线程池
static CPthread *thread_send;

//应答链表
static CMyList AnswerList;

//链表锁
static pthread_mutex_t  mutex_list;		//互斥量
static sem_t empty_list;





/*****************************
*函数名：SERVICE_config_read
*功能：读取配置文件
*参数：无
*返回值:无
******************************/
void SERVICE_config_read()
{
	//服务器配置
	char server_ip[16];
	int server_port;
	int config_fd;
	char acBuf[50];
	
	config_fd = open("config.ini",O_RDONLY);
	if(config_fd < 0)
	{
		config_fd =open("config.ini",O_CREAT|O_WRONLY|O_TRUNC,0711);
		strcpy(server_ip,"127.0.0.1");
		server_port = 5678;
	}
	else
	{
		read(config_fd,acBuf,sizeof(acBuf));
		sscanf(acBuf,"%[^:]:%d",server_ip,&server_port);
	}

	printf("服务器IP为: %s \t 端口号为:%d\n如有需要请修改配置文件config.ini\n",server_ip,server_port);

	//服务器配置
	tp_socket_addr server_addr;
	server_addr.ip_addr = new char[20];
	strcpy(server_addr.ip_addr,server_ip);
	server_addr.port = server_port;

	//建立连接
	myconnect =new CConnect(server_addr,SERVICE_read_event,thread_rec);

}


/*****************************
*函数名：SERVICE_shm_create
*功能：共享内存创建
*参数：无
*返回值:无
******************************/
void SERVICE_shm_create()
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

	SERVICE_shm_memset();

}


/*****************************
*函数名：SERVICE_shm_memset
*功能：为共享内存初始化
*参数：无
*返回值:无
******************************/
void SERVICE_shm_memset()
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
*函数名：SERVICE_forward_init
*功能：前置任务
*参数：无
*返回值:无
******************************/
void SERVICE_forward_init()
{

	//共享内存创建
	SERVICE_shm_create();


	//链表锁
	pthread_mutex_init(&mutex_list,NULL);

	//创建链表信号量
	sem_init(&empty_list,0,MAX_THREAD);

	//数据源信号量
	sem_init(&empty_list,0,MAX_THREAD);

	//线程池创建
	thread_rec = new CPthread(MAX_THREAD,MAX_TASK);
	thread_send = new CPthread(MAX_THREAD,MAX_TASK);

	//发送模块的条件变量
	pthread_mutex_init(&mutex_send,NULL);
	pthread_cond_init(&cond_send,NULL);

	//接收模块的条件变量
	pthread_mutex_init(&mutex_rec,NULL);
	pthread_cond_init(&cond_rec,NULL);

	//数据源模块的条件变量
	pthread_mutex_init(&mutex_shm,NULL);

	//中断的条件变量
	pthread_mutex_init(&mutex_break,NULL);
	pthread_cond_init(&cond_break,NULL);

	//接受模块初始化
	pthread_t thread_rec_id;
	pthread_create(&thread_rec_id,NULL,SERVICE_rec,NULL);

	//发送模块初始化
	pthread_t thread_send_id;
	pthread_create(&thread_send_id,NULL,SERVICE_send,NULL);

// 	//心跳模块初始化
// 	pthread_t thread_heart_id;
// 	pthread_create(&thread_heart_id,NULL,SERVICE_heart,NULL);

	
	//设置好服务器IP跟端口
	SERVICE_config_read();

}


/*****************************
*函数名：SERVICE_main
*功能：主线程
*参数：无
*返回值:无
******************************/
void SERVICE_main()
{
	while(1)
	{
		if (stu_info->isOK)
		{
			//发送信号给发送模块
			pthread_mutex_lock(&mutex_send);
			pthread_cond_signal(&cond_send);
			pthread_mutex_unlock(&mutex_send);

			//运行中
			while(1)
			{
				//测试是否结束
				if (stu_info->isOK==false)
				{
					break;
				}

				//是否中断
				if (stu_info->isBreak)
				{

					//中断是否又开始
					while(1)
					{
						//处理发送信号
						if (stu_info->isBreak == false || stu_info->isOK==false)
						{
							//发送信号给发送模块
							pthread_mutex_lock(&mutex_break);
							pthread_cond_signal(&cond_break);
							pthread_mutex_unlock(&mutex_break);
							break;
						}

						usleep(100);
					}
				}

				usleep(100);
			}

			//进行初始化

		}
		usleep(100);
	}
}


/*****************************
*函数名:SERVICE_heart
*功能：发送模块
*参数：无
*返回值:无
******************************/
void* SERVICE_heart(void* argv)
{
	char *acBuf;
	int nSum;
	int i;
	int str_len;

	//包头
	struct package_head *bag_head;

	tp_socket_info *new_socket_info;

	tp_thread_work *new_work;

	while(1)
	{
		if (stu_info->isOK)
		{
			//等待条件变量

			sscanf((char *)stu_info->debug_sum,"%d",&nSum);

			while(stu_info->isOK)
			{
			
				for (i = 0;i < nSum ;i++)
				{
					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(i+1,"0000",1,1,0);

					//发包 -- 打包
					str_len = sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					acBuf[str_len-1]='<';
					
					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;
					
					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;
					
					//加入队列
					thread_send->add_thread(new_work);
				}
				
				sleep(5);
			}
		}

		sleep(1);
	}
}



/*****************************
*函数名:SERVICE_send
*功能：发送模块
*参数：无
*返回值:无
******************************/
void* SERVICE_send(void* argv)
{

	//封包参数
		//长度
		int str_len;

		//结构体
		tp_socket_info *new_socket_info;
		tp_thread_work *new_work;

		//总包
		char *acBuf;

		//包头
		struct package_head *bag_head;
		//包体
		struct request_login *func_0001;
		struct request_recharge *func_1001;
		struct request_callsearch *func_2001;
		struct request_historybill *func_2002;
		struct request_billdetail *func_2003;
		struct request_buycard *func_3001;



	//常用变量
	int i=0;
	int dev_id,now_num;

	//重要参数
	unsigned int nTime,nSum,nInterval;
	int nSend_connect;
	int nSend_bag;
	double dInterval;

	//赋空
	myconnect = NULL;

	while(1)
	{
		//条件变量
		pthread_cond_wait(&cond_send,&mutex_send);


		//获取参数
		sscanf((char *)stu_info->debug_time,"%d",&nTime);
		sscanf((char *)stu_info->debug_sum,"%d",&nSum);
		sscanf((char *)stu_info->debug_interval,"%d",&nInterval);
		nInterval = nInterval * 1000;

		//应答设置
		server_answer = stu_info->isAnswer;


		//发送信号开始准备接收
		pthread_mutex_lock(&mutex_rec);
		pthread_cond_signal(&cond_rec);
		pthread_mutex_unlock(&mutex_rec);


		for (i = 0;i < nSum ;i++)
		{
			now_num = i;
			dev_id = i+1;
	
			if (stu_info->isOK == false)
			{
				break;
			}

			//判断是否中断
			if (stu_info->isBreak)
			{
				pthread_cond_wait(&cond_break,&mutex_break);
				
				//判断是否已退出
				if (stu_info->isOK == false)
				{
					break;
				}
			}
			
			//添加连接
			myconnect->add_connect();
			
			//发起连接数+1
			sscanf(stu_info->send_connect,"%d",&nSend_connect);
			sprintf(stu_info->send_connect,"%d",nSend_connect+1);
			
			////包头部分 ---调用包头函数
			bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"0001",1,1,sizeof(struct request_login));
			
			//包体
			func_0001 =  (struct request_login *)malloc(sizeof(struct request_login));
			memset(func_0001,0,sizeof(struct request_login));
			
			//发送上线包 -- 打包
			//acBuf = SERVICE_bag_packet((void *)bag_head,sizeof(struct package_head),(void *)func_0001,sizeof(struct request_login));
			
			//发包 -- 打包
			str_len = sizeof(struct request_login) + sizeof(struct package_head) + 2;
			acBuf = (char *)malloc(str_len);
			memset(acBuf,0,str_len);
			acBuf[0]='~';
			memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
			memcpy(acBuf+1+sizeof(struct package_head),(char *)func_0001,sizeof(struct request_login));
			acBuf[str_len-1]='<';
			
			//写入线程池队列前的处理
			new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
			new_socket_info->client_socket_fd = myconnect->getSocket(i);
			new_socket_info->len = str_len;
			new_socket_info->buffer = acBuf;
			
			//发包函数
			new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
			new_work->callback = SERVICE_send_bag;
			new_work->argv = (void *)new_socket_info;
			
			//加入队列
			thread_send->add_thread(new_work);



			//等待时间
			usleep(1000);

		}
		
		

		//根据时间间隔进行操作
		while(stu_info->isOK)
		{
			//根据选择进行相应操作
			for (i = 0;i < nSum ;i++)
			{

				now_num = i;
				dev_id = i+1;

				if (stu_info->isOK == false)
				{
					break;
				}

				//判断是否中断
				if (stu_info->isBreak)
				{
					pthread_cond_wait(&cond_break,&mutex_break);
					
					//判断是否已退出
					if (stu_info->isOK == false)
					{
						break;
					}
				}

				if (stu_info->use_card)
				{
					

					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"1001",1,1,sizeof(struct request_recharge));
					//包体
					func_1001 =  (struct request_recharge *)malloc(sizeof(struct request_recharge));
					memset(func_1001,0,sizeof(struct request_recharge));
					strcpy(func_1001->phone_num,"13123456789");
					func_1001->card_id = '1';
					func_1001->card_num = 1;

					//发包 -- 打包
					str_len = sizeof(struct request_recharge) + sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					memcpy(acBuf+1+sizeof(struct package_head),(char *)func_1001,sizeof(struct request_recharge));
					acBuf[str_len-1]='<';

					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;

					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;

					//发起连接数+1
					sscanf(stu_info->send_bag,"%d",&nSend_bag);
					sprintf(stu_info->send_bag,"%d",nSend_bag+1);

					//加入队列
					thread_send->add_thread(new_work);

					//等待时间
					usleep(nInterval);
				}

				if (stu_info->buy_card)
				{
			

					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"3001",1,1,sizeof(struct request_buycard));
					//包体
					func_3001 =  (struct request_buycard *)malloc(sizeof(struct request_buycard));
					memset(func_3001,0,sizeof(struct request_buycard));
					//包体描述
					func_3001->char_count = 1;

					//发包 -- 打包
					str_len = sizeof(struct request_buycard) + sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					memcpy(acBuf+1+sizeof(struct package_head),(char *)func_3001,sizeof(struct request_buycard));
					acBuf[str_len-1]='<';

					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;

					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;

					//发起连接数+1
					sscanf(stu_info->send_bag,"%d",&nSend_bag);
					sprintf(stu_info->send_bag,"%d",nSend_bag+1);

					//加入队列
					thread_send->add_thread(new_work);


					//等待时间
					usleep(nInterval);

				}
				if (stu_info->phone_last)
				{

					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"2001",1,1,sizeof(struct request_callsearch));
					//包体
					func_2001 =  (struct request_callsearch *)malloc(sizeof(struct request_callsearch));
					memset(func_2001,0,sizeof(struct request_callsearch));
					//包体描述
					func_2001->fee_type = '0';
					strcpy(func_2001->phone_num,"13123456789");

					//发包 -- 打包
					str_len = sizeof(struct request_callsearch) + sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					memcpy(acBuf+1+sizeof(struct package_head),(char *)func_2001,sizeof(struct request_callsearch));
					acBuf[str_len-1]='<';

					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;

					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;

					//发起连接数+1
					sscanf(stu_info->send_bag,"%d",&nSend_bag);
					sprintf(stu_info->send_bag,"%d",nSend_bag+1);

					//加入队列
					thread_send->add_thread(new_work);

					//等待时间
					usleep(nInterval);


				}
				if (stu_info->phone_blur)
				{

					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"2002",1,1,sizeof(struct request_historybill));
					//包体
					func_2002 =  (struct request_historybill *)malloc(sizeof(struct request_historybill));
					memset(func_2002,0,sizeof(struct request_historybill));
					//包体描述
					strcpy(func_2002->phone_num,"13123456789");
					func_2002->fee_type = '1';
					strcpy(func_2002->month,"201108");

					//发包 -- 打包
					str_len = sizeof(struct request_historybill) + sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					memcpy(acBuf+1+sizeof(struct package_head),(char *)func_2002,sizeof(struct request_historybill));
					acBuf[str_len-1]='<';

					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;

					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;

					//发起连接数+1
					sscanf(stu_info->send_bag,"%d",&nSend_bag);
					sprintf(stu_info->send_bag,"%d",nSend_bag+1);

					//加入队列
					thread_send->add_thread(new_work);

					//等待时间
					usleep(nInterval);


				}
				if (stu_info->phone_particular)
				{
					//等待时间
					//usleep(nInterval);

					////包头部分 ---调用包头函数
					bag_head = (struct package_head *)SERVICE_bag_head(dev_id,"2003",1,1,sizeof(struct request_billdetail));
					//包体
					func_2003 =  (struct request_billdetail *)malloc(sizeof(struct request_billdetail));
					memset(func_2003,0,sizeof(struct request_billdetail));
					//包体描述
					strcpy(func_2003->phone_num,"13123456789");
					func_2003->fee_type = '2';
					strcpy(func_2003->month_begin,"201106");
					strcpy(func_2003->month_end,"201108");

					//发包 -- 打包
					str_len = sizeof(struct request_billdetail) + sizeof(struct package_head) + 2;
					acBuf = (char *)malloc(str_len);
					memset(acBuf,0,str_len);
					acBuf[0]='~';
					memcpy(acBuf+1,(char *)bag_head,sizeof(struct package_head));
					memcpy(acBuf+1+sizeof(struct package_head),(char *)func_2003,sizeof(struct request_billdetail));
					acBuf[str_len-1]='<';

					//写入线程池队列前的处理
					new_socket_info = (tp_socket_info *)malloc(sizeof(tp_socket_info));
					new_socket_info->client_socket_fd = myconnect->getSocket(i);
					new_socket_info->len = str_len;
					new_socket_info->buffer = acBuf;

					//发包函数
					new_work = (tp_thread_work *)malloc(sizeof(tp_thread_work));
					new_work->callback = SERVICE_send_bag;
					new_work->argv = (void *)new_socket_info;

					//发起连接数+1
					sscanf(stu_info->send_bag,"%d",&nSend_bag);
					sprintf(stu_info->send_bag,"%d",nSend_bag+1);

					//加入队列
					thread_send->add_thread(new_work);

					//等待时间
					//usleep(nInterval);

				}

				//等待时间
				usleep(nInterval);

			}

		}
		//usleep(100);
		
	}

}


/*****************************
*函数名:SERVICE_bag_head
*功能：包头
*参数：无
*返回值:无
******************************/
void * SERVICE_bag_head(int dev_id,char *func_id,int packet_seq,int packet_sum,int packet_size)
{
	//计数
	static int for_send_num = 0;

	//时间
	time_t t;
	struct tm *now;

	struct package_head *bag_head = (struct package_head *)malloc(sizeof(struct package_head));
	memset(bag_head , 0 ,sizeof(struct package_head));
	sprintf(bag_head->dev_id,"%d",dev_id);
	strcpy(bag_head->func_id,func_id);

	bag_head->packet_seq = packet_seq;
	bag_head->packet_sum = packet_sum;
	bag_head->packet_size = packet_size;

	//生成交易流水号
	t=time(NULL);
	now=localtime(&t);
	sprintf(bag_head->trans_id,"%4d%02d%02d%02d%02d%02d%04d",now->tm_year+1900,
		now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec,for_send_num++);

	//超过9999就置零
	if (for_send_num == 9999)
	{
		for_send_num = 0;
	}

	return bag_head;

}


/*****************************
*函数名:SERVICE_bag_packet
*功能：包头
*参数：无
*返回值:无
******************************/
char* SERVICE_bag_packet(void *head,int head_size,void *func,int func_size)
{
	char *new_packet;
	int str_len = head_size+func_size+3;

	new_packet = (char *)malloc(str_len);
	memset(new_packet,0,str_len);
	new_packet[0]='~';
	memcpy(new_packet+1,head,head_size);
	memcpy(new_packet+1+head_size,func,func_size);
	new_packet[str_len-2]='<';

	return new_packet;

}


/*****************************
*函数名:SERVICE_send_bag
*功能：发送模块
*参数：无
*返回值:无
******************************/
void* SERVICE_send_bag(void* argv)
{
	tp_socket_info *socket_info = (tp_socket_info *)argv;
	int n_write;
	struct package_head bag_head;
	int func_num;
	int t;
	

	if (server_answer)
	{		
		memcpy((char *)&bag_head,(char *)socket_info->buffer+1,sizeof(struct package_head));
		AnswerList.addNode((void *)&bag_head);
	}

	n_write=write(socket_info->client_socket_fd,(char *)socket_info->buffer,socket_info->len);

	free(socket_info->buffer);
	free(socket_info);


}


/*****************************
*函数名:SERVICE_rec
*功能：发送模块
*参数：无
*返回值:无
******************************/
void* SERVICE_rec(void* argv)
{
	while (1)
	{
		//条件变量
		pthread_cond_wait(&cond_rec,&mutex_rec);
		//开始运行
		myconnect ->start();
	}
}

static int times=0;

/*****************************
*函数名:SERVICE_read_event
*功能：发送模块
*参数：无
*返回值:无
******************************/
void* SERVICE_read_event(void *argv)
{
	char buffer[MAX_SIZE]={0};
	int client_socket;
	int n_read;
	char head_get,back_get;
	struct package_head bag_head;
	struct package_head *test_head;
	int func_num;
	int t;
	bool isRight = true;

	//包体
	struct response_login func_0001;
	struct response_recharge func_1001;
	struct response_callsearch func_2001;
	struct response_historybill func_2002;
	struct response_billdetail func_2003;
	struct response_buycard func_3001;

	//获取套接字
	client_socket = ((tp_socket_info *)argv)->client_socket_fd ;

	//读取内容
	memset(buffer,0,MAX_SIZE);
	n_read = read(client_socket,buffer,sizeof(buffer));

	//前置标志和后置标志的判断
	head_get = buffer[0];
	//读取包头
	memcpy((void *)&bag_head,buffer+1,sizeof(struct package_head));
	//printf("前置标志 %c 设备ID %s 业务ID %s 包长度%d 交易流水号%s\n",head_get,bag_head.dev_id,bag_head.func_id,bag_head.packet_size,bag_head.trans_id);


	//判断前置标志 决定是否继续读取
	if (head_get != '~')
	{
		isRight = false;
	}
	
	//读取末尾标志
	back_get=buffer[n_read-1];
	
	//判断末尾标志
	if (back_get != '<')
	{
		isRight = false;
	}

	
	//判断功能号决定读取数据大小
	sscanf(bag_head.func_id,"%4d",&func_num);
	
	//如果是登陆包 就判断是否激活
	if (func_num == 1)
	{
		memcpy((void *)&func_0001,buffer+sizeof(struct package_head)+1,sizeof(struct response_login));
	}
	
	switch(func_num)
	{
	case 1:
		if (func_0001.dev_status != '0')
		{
			sscanf(stu_info->have_connect,"%d",&t);
			sprintf(stu_info->have_connect,"%d",t+1);
			
		}
		break;
		
	case 1001:
	case 2001:
	case 2002:
	case 2003:
	case 3001:
		
		pthread_mutex_lock(&mutex_shm);
		
		sscanf(stu_info->rec_bag,"%d",&t);
		sprintf(stu_info->rec_bag,"%d",t+1);
		
		pthread_mutex_unlock(&mutex_shm);
		
		break;
		
	default:
		break;
	}


	if (server_answer && isRight == true)
	{
	
		if (func_num != 1 )
		{
			//处理应答	
			for (t=1;t<=AnswerList.count();t++)
			{
				test_head = (struct package_head *)AnswerList[t];
				
				if (strcmp(test_head->func_id,bag_head.func_id) == 0)
				{
					sem_wait(&empty_list);
					
					pthread_mutex_lock(&mutex_list);
					
					AnswerList.delNode(t);
					
					pthread_mutex_unlock(&mutex_list);
					
					sem_post(&empty_list);
					
					pthread_mutex_lock(&mutex_shm);
					
					sscanf(stu_info->business_OK,"%d",&t);
					sprintf(stu_info->business_OK,"%d",t+1);
					
					pthread_mutex_unlock(&mutex_shm);
					
					return NULL;
				}
			}	
			isRight = false;
			
			
		}
			
	}


	if (isRight == false)
	{
		
		if (server_answer && func_num != 1)
		{
			pthread_mutex_lock(&mutex_shm);
			
			sscanf(stu_info->business_FAIL,"%d",&t);
			sprintf(stu_info->business_FAIL,"%d",t+1);
			
			pthread_mutex_unlock(&mutex_shm);
			
			//printf("错误包 前置标志 %c 后置标志 %c 设备ID %s 业务ID %s 包长度%d 交易流水号%s\n",head_get,back_get,bag_head.dev_id,bag_head.func_id,bag_head.packet_size,bag_head.trans_id);
		}
	}
	
}

