#ifndef _MINI_PUBLIC_H_
#define _MINI_PUBLIC_H_

//包头
struct package_head{
	char dev_id[18];//设备ID
	int packet_size;//包长度
	int packet_seq;//包序号
	int packet_sum;//包总数
	char trans_id[19];//交易流水号
	char func_id[5];//功能号，详见包体接口
	char error_code[4] ;//错误代码
	char error_msg[30];//错误信息
};
//登录请求
//请求包
struct request_login{
	char soft_ver[7];//软件版本号
	char hard_ver[7];//硬件版本号
};

//应答包
struct response_login{
	char dev_status;//设备状态  1激活可用 0 冻结挂失
};

//一卡通充值
//请求包
struct request_recharge{
	char phone_num[13];//手机号码
	char card_id;//充值卡 ID 0-20元  1：30元 2:50元 3表示 100元
	int card_num;//卡数量
};
//应答包
struct response_recharge{
	char trans_flowid[23];//交易流水号

};

//                       话费查询

//余额查询
//请求包
struct request_callsearch{
	char phone_num[13];//手机号码
	char fee_type;//话费类型 0话费余额 1 历史账单 2历史账单
};
//应答包
struct response_callsearch{
	char trans_flowid[23];//平台流水号
	char balance[10];//余额单位分
};

//历史账单
//请求包
struct request_historybill{
	char phone_num[13];//手机号码
	char fee_type;//话费类型 0话费余额 1历史账单 2历史详单
	char month[7];//月份格式 YYYYMM

};
//应答包
struct response_historybill{
	char trans_flowid[23];//流水化 YYYYMMDDHHMISS+func_id+4未随机数字
	char bill_content[1024];//账单内容
};
//账单详情
//请求包
struct request_billdetail{
	char phone_num[13];//手机号码
	char fee_type;//化肥类型
	char month_begin[7];//起始月份 格式YYYYMM
	char month_end[7];//终止月份

};

//应答包
struct response_billdetail{
	char trans_flowid[23];//交易流水号
	char bill_content[1024];//详单内容
};

//购买电子卡
//请求包
struct request_buycard{
	char card_sum[13];//卡面值
	int char_count;//卡数量
};
//应答包
struct response_buycard{
	char trans_flowid[23];//交易流水号
	int card_count;//卡数量
	char card_no[30];//卡序列号
};


#endif

