/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： service.h
*
* 内容摘要： interface for the service module
*
* 历史：
*创建日期：2011-8-18
*创建人：	陈磊 (Tristone)
\******************************************************************/

#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "public.h"

/***************************************\
*函数名：login_service
*
*功能：处理登陆业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void login_service(ser_cont context);

/***************************************\
*函数名：use_card_service
*
*功能：处理一卡通充值业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void use_card_service(ser_cont context);

/***************************************\
*函数名：que_money_service
*
*功能：处理余额查询业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void que_money_service(ser_cont context);

/***************************************\
*函数名：buy_card_service
*
*功能：处理购买电子卡业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void buy_card_service(ser_cont context);

/***************************************\
*函数名：que_detail_service
*
*功能：处理详单查询业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void que_detail_service(ser_cont context);

/***************************************\
*函数名：que_bill_service
*
*功能：处理账单查询业务
*参数：
*
*返回值：
*
*说明：
****************************************/
void que_bill_service(ser_cont context);

/***************************************\
*函数名：get_trans_flowid
*
*功能：获取时间函数，用于生成流水号的一部分
*参数：
*
*返回值：返回了一个字符指针，指向函数内部
*	开辟的一块保存当前系统时间的内存，
*	时间格式：
*	YYYYMMDDHHMMSS
*
*说明：用完后要释放返回值所指向的堆空间
****************************************/
char* get_trans_flowid();


//int check_cb(void* context, int n, char** colval, char** colname);

/***************************************\
*函数名：check_dev_cb
*
*功能：检查登陆设备ID所用的回调函数
*参数：context--caller传入的上下文参数
*	n--查询到的数据数
*	colval--查询到的值
*	colname--查询到的值在表中的列名
*
*返回值：返回0则继续查询，全部查询成功
*	则caller返回SQLITE_OK,否则停止查询，
*	caller返回SQLITE_ABORT
*说明：
****************************************/
int check_dev_cb(void* context, int n, char** colval, char** colname);

/***************************************\
*函数名：que_money_cb
*
*功能： 查询余额时所用的回调函数
*参数：context--caller传入的上下文参数
*	n--查询到的数据数
*	colval--查询到的值
*	colname--查询到的值在表中的列名
*
*返回值：返回0则继续查询，全部查询成功
*	则caller返回SQLITE_OK,否则停止查询，
*	caller返回SQLITE_ABORT
*说明：
****************************************/
int que_money_cb(void* context, int n, char** colval, char** colname);

/***************************************\
*函数名：que_bill_cb
*
*功能：账单查询时所用的回调函数
*参数：context--caller传入的上下文参数
*	n--查询到的数据数
*	colval--查询到的值
*	colname--查询到的值在表中的列名
*
*返回值：返回0则继续查询，全部查询成功
*	则caller返回SQLITE_OK,否则停止查询，
*	caller返回SQLITE_ABORT
*说明：
****************************************/
int que_bill_cb(void* context, int n, char** colval, char** colname);

/***************************************\
*函数名：que_detail_cb
*
*功能：详单查询时所用的回调函数
*参数：context--caller传入的上下文参数
*	n--查询到的数据数
*	colval--查询到的值
*	colname--查询到的值在表中的列名
*
*返回值：返回0则继续查询，全部查询成功
*	则caller返回SQLITE_OK,否则停止查询，
*	caller返回SQLITE_ABORT
*说明：
****************************************/
int que_detail_cb(void* context, int n, char** colval, char** colname);

#endif

