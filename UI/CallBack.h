#ifndef _CALLBACK_H_
#define _CALLBACK_H_


#define SHM_SIZE 4096

typedef struct
{
    char send_connect[10];		//发送连接
    char have_connect[10];		//成功登录
	char send_bag[10];			//发送交易
	char rec_bag[10];			//收到交易
	char business_OK[10];		//交易成功
	char business_FAIL[10];		//交易失败
	char debug_time[10];			//测试时间
	char debug_sum[10];			//测试总数
	char debug_interval[10];		//测试间隔
	bool isAnswer;				//是否应答
	bool use_card;				//是否充值
	bool buy_card;				//是否买卡
	bool phone_last;			//是否余额
	bool phone_blur;			//是否账单
	bool phone_particular;		//是否详单
    bool isBreak;				//是否中断
	bool isOK;					//是否OK
}SHM_MESSAGE;


void CALLBACK_forward_init();			//初始化处理

void CALLBACK_shm_create();				//共享内存的创建

void CALLBACK_shm_memset();				//共享内存的重置

void* CALLBACK_time_refresh(void *argv);		//时间刷新

void* CALLBACK_form_init(void *form_control);		//创建窗口

int CALLBACK_start_debug(void *form_control);		//开始测试回调

int CALLBACK_parameter(void* control,int eventID);	//开始测试按钮的事件

int CALLBACK_stop_debug(void *form_control);		//暂停测试

int CALLBACK_write_debug(void *form_control);		//写入文件

int CALLBACK_end_debug(void *form_control);			//退出

int CALLBACK_debug(void* control,int eventID);		//回调



#endif

