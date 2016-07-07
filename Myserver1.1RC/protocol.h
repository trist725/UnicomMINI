/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： protocol.h
*
* 内容摘要：定义了通信协议，即通信时所用的包头及所有包体
*
* 历史：
*创建日期：2011-8-14
*创建人：	陈磊
\******************************************************************/

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_


//协议包头
typedef struct pack_head_t
{
	char acDev_id[18];		//设备ID
	int iBody_Size;			//包体大小
	int iPack_Seq;			//包序号，应答用
	int iPack_Sum;			//包总数，应答用
	char acTrans_id[19];	//交易包流水号格式：YYYYMMDDHHMISS+4位随机数
	char acFunc_ID[5];		//功能号
	char acErr_Code[4];		//错误代码,全为'0'表示协议不兼容
	char acErr_Msg[30];		//错误信息
}pack_head;


//登录请求包----0001
typedef struct Packet_Login_Request_T
{
	char acSoft_Ver[7];		//软件版本号
	char acHard_Ver[7];		//硬件版本号
}log_req;

//登录应答包----0001
typedef struct Packet_Login_Respond_T
{
	char cDev_Stat;		//设备状态,'1'表示合法设备
}log_res;

//一卡通充值请求包----1001
typedef struct Packet_Prepaid_Request_T
{	
	char acPhone_Num[13];	//手机号码
	char cCard_ID;			//充值的卡ID 0－20元;1－30元;2－50元;3－100元
	int	iCard_Num;			//卡数量
}use_card_req;

//一卡通充值应答包体----1001
typedef struct Packet_Prepaid_Respond_T
{
	char acTrans_Flowid[23];//交易流水：平台生成：格式：YYYYMMDDHHMISS+func_id+5位随机数
}use_card_res;

//余额查询请求包----2001
typedef struct Packet_Balance_Query_Request_T
{
	char acPhone_Num[13];	//手机号码
	char cFee_Type;			//话费类型0－话费余额，1－历史账单，2－历史详单
}que_money_req;

//余额查询应答包----2001
typedef struct Packet_Balance_Query_Respond_T
{
	char acTrans_Flowid[23];//交易流水：平台生成：格式：YYYYMMDDHHMISS+func_id+5
	char acBalance[10];		//余额，单位分
}que_money_res;

//历史账单查询请求包----2002
typedef struct Packet_HisBill_Qry_Request_T
{
	char acPhone_Num[13];	//手机号码
	char cFee_Type;			//话费类型0－话费余额，1－历史账单，2－历史详单
	char acMonth[7];		//月份 格式YYYYMM
}bill_req;

//历史账单查询应答包----2002
typedef struct Packet_HisBill_Qry_Respond_T
{
	char acTrans_Flowid[23];	//交易流水：平台生成：格式：YYYYMMDDHHMISS+func_id+5
	char acBill_Content[1024];	//账单内容
}bill_res;

//历史详单查询请求包----2003
typedef struct Packet_DetBill_Qry_Request_T
{
	char acPhone_Num[13];	//手机号码
	char cFee_Type;			//话费类型0－话费余额，1－历史账单，2－历史详单
	char acMonth_Begin[7];	//起始月份
	char acMonth_End[7];	//终止月份
}det_bill_req; 

//历史详单查询应答包----2003
typedef struct Packet_DetBill_Qry_Respond_T
{
	char acTrans_Flowid[23];	//交易流水：平台生成：格式：YYYYMMDDHHMISS+func_id+5	
	char acBill_Content[1024];	//详单内容
}det_bill_res;

//购买电子卡请求包----3001
typedef struct Packet_Buy_Card_Request_T
{
	char acCard_Sum[13];	//卡面值
	int iCard_Count;		//卡数量
}buy_card_req;

//购买电子卡应答包----3001
typedef struct Packet_Buy_Card_Respond_T
{
	char acTrans_Flowid[23];//交易流水：平台生成：格式：YYYYMMDDHHMISS+func_id+5
	int iCard_Count;		//卡数量
	char acCard_NO[30];		//卡序列号
}buy_card_res;


#endif

