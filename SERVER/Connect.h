#ifndef _CONNECT_H_
#define _CONNECT_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "Pthread.h"

#define BUFFER_SIZE 4096


//服务器信息
typedef struct 
{
	int port;
	char *ip_addr;
}tp_socket_addr;

//服务器保存为客户端产生套件字以及其它信息的容器
typedef struct
{
	int client_socket_fd;	
	int len;
	void *buffer;
}tp_socket_info;


//socket组件内部时间通知的函数指针用于给用户使用的回调指针
typedef void *(*socket_notify_event)(void *socket_info);

//核心服务器激活的函数指针
typedef   int (*server_active)(int port);

//socket模型的函数指针用于指向io复用核心函数
typedef   void (*service_core)(void);


typedef  struct
{
    int  cliSocket;
    int  HaveUseThread;
}ClientSockets_T;


void* ChangeState(void *argv);


static ClientSockets_T socket_array[FD_SETSIZE];
static socket_notify_event rec_event;					//客户端发送数据事件产生

class CConnect
{
public:
	CConnect (tp_socket_addr address,socket_notify_event read_event,CPthread *pthread);
	virtual ~CConnect ();
	int start();
	int stop();
	int getSocket(int array_index);
	void add_connect();
protected:
	int server_active();
	int service_core();
private:	
	//线程池
	CPthread *thread_manager;
	tp_thread_work *thread_work;

	//客户端
	int on;
	int ac_socket;		//accept返回值
	int addr_len;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	tp_socket_addr socket_addr;
	tp_socket_info	*stSocket_info;
	
	//select
	fd_set rset;
	fd_set allset;
	
	//常用变量
	int i,j;	//常用变量
	int maxfd;		//最大的fd
	int index;		//索引
	int select_return;	//select返回值
	int n_read,n_write;	//读写返回值
	int iID;		//ID 号
	bool nChoice;		//socket开关
	int connect_sum;  //套接字个数
	
	//字符串
	char dialogue[BUFFER_SIZE];
};

#endif

