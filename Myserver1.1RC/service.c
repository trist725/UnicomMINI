/******************************************************************\
* 版权所有 (C)2011, by Tristone
* 文件名称： init.h
*
* 内容摘要：implementation for the service module
*
* 历史：
*创建日期：2011-8-14
*创建人：	陈磊
*修改日期：2011-8-24
*创建人：	陈磊
*修改内容：修正了部分错误
\******************************************************************/

#include "service.h"
#include "init.h"

extern sqlite3 *pDB;
extern online_client clients[FD_SETSIZE];
//在线合法客户端数
extern int client_num;
//已连接数
extern int conned_num;
//交易流水索引
extern long iRand;
//详单查询索引
int detail_index = 0;

void login_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	char buf[sizeof(pack_head)+3] = {0};
	pack_head recv_head;
	memcpy(&recv_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	buf[sizeof(pack_head)+2] = '<';
	char  stm[100] = {0};
	sprintf(stm,  "select * from login_info where dev_id = '%s';", recv_head.acDev_id);
	recv_head.iPack_Seq = 1;
	recv_head.iPack_Sum = 1;
	recv_head.iBody_Size = 1;
	memcpy(&buf[1], &recv_head, sizeof(pack_head));
	int dev_id = atoi(recv_head.acDev_id);
	if (dev_id >= 0 || dev_id <= 1010)
	{
		goto log_suc;
	}
	//登录成功
	if (SQLITE_ABORT == DB_exec(stm, check_dev_cb, NULL, NULL))
	{
log_suc:	buf[sizeof(pack_head)+1] = '1';
		send(send_fd, buf, sizeof(buf), MSG_DONTWAIT);
		printf("login success..\n");
		clients[context.index].timeout = CLIENT_TIMEOUT;
		strcpy(clients[context.index].dev_id, recv_head.acDev_id);
		client_num++;
	}
	//非法设备
	else
	{
		buf[sizeof(pack_head)+1] = '0';
		printf("未注册的设备ID..\n");
		send(send_fd, buf, sizeof(buf), MSG_DONTWAIT);
		//关闭该连接,并且将描述符重置为-1
		close(clients[context.index].client_fd);
		clients[context.index].client_fd = -1;
		conned_num--;
	}
	free(context.recv_pack);
}

int check_dev_cb(void* context, int n, char** colval, char** colname)
{
	return 1;	//返回非0值表示不继续查询,exec函数返回SQLITE_ABORT
}

void use_card_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	int money;
	char buf[1+sizeof(pack_head)+sizeof(use_card_res)+1] = {0};
	pack_head send_head;
	use_card_res send_body;
	use_card_req recv_body;
	memcpy(&recv_body, &context.recv_pack[1+sizeof(pack_head)], sizeof(use_card_req));
	memcpy(&send_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	buf[1+sizeof(pack_head)+sizeof(use_card_res)] = '<';
	strcpy(send_head.acDev_id, clients[context.index].dev_id);
	send_head.iBody_Size = sizeof(use_card_res);
	send_head.iPack_Sum = 1;
	send_head.iPack_Seq = 1;
	strcpy(send_head.acFunc_ID, "1001");
	switch(recv_body.cCard_ID)
	{
	case '0':
		money = 20;
		break;
	case '1':
		money = 30;
		break;
	case '2':
		money = 50;
		break;
	case '3':
		money = 100;
		break;
	default:
		strcpy(send_head.acErr_Code, "111");
		strcpy(send_head.acErr_Msg, "错误的充值ID\n");
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
		free(context.recv_pack);
		return;
	}
	money *= recv_body.iCard_Num;
	char stm[100] ={0};
	sprintf(stm, "update client_info set phone_money = phone_money+%d where phone_num = %s;", money, recv_body.acPhone_Num);
	if (SQLITE_OK == DB_exec(stm, NULL, NULL, NULL))
	{//获取系统时间
		char* trans_flowid = get_trans_flowid();
		strcpy(send_body.acTrans_Flowid, trans_flowid);
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		memcpy(&buf[1+sizeof(pack_head)], &send_body, sizeof(use_card_res));
		int ret = send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
		free(trans_flowid);
	}
	else
	{
		strcpy(send_head.acErr_Code, "111");
		strcpy(send_head.acErr_Msg, "数据库中无该记录\n");
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
	}
	free(context.recv_pack);
}

void que_money_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	int money;
	char buf[1+sizeof(pack_head)+sizeof(que_money_res)+1] = {0};
	pack_head send_head;
	que_money_res send_body;
	que_money_req recv_body;
	memcpy(&recv_body, &context.recv_pack[1+sizeof(pack_head)], sizeof(que_money_req));
	memcpy(&send_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	buf[1+sizeof(pack_head)+sizeof(que_money_res)] = '<';
	strcpy(send_head.acDev_id, clients[context.index].dev_id);
	send_head.iBody_Size = sizeof(que_money_res);
	send_head.iPack_Sum = 1;
	send_head.iPack_Seq = 1;
	strcpy(send_head.acFunc_ID, "2001");
	char stm[100] = {0};
	sprintf(stm, "select phone_money from client_info where phone_num = %s;", recv_body.acPhone_Num);
	char balance[10] ={0};
	if (SQLITE_ABORT == DB_exec(stm, que_money_cb, (void*)balance, NULL))
	{
		char* temp = get_trans_flowid();
		strcpy(send_body.acTrans_Flowid, temp);
		strcpy(send_body.acBalance, balance);
		//printf("%s\n",balance);
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		memcpy(&buf[1+sizeof(pack_head)], &send_body, sizeof(que_money_res));
		send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
		free(temp);
		free(context.recv_pack);
		return;
	}
	else
	{
		strcpy(send_head.acErr_Code, "111");
		strcpy(send_head.acErr_Msg, "数据库中无该记录\n");
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		memcpy(&buf[1+sizeof(pack_head)], &send_body, sizeof(que_money_res));
		send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
	}
	free(context.recv_pack);
}

int que_money_cb(void* context, int n, char** colval, char** colname)
{
	//单位是分,于是*100
	//int money = atoi(colval[0]);
	//money *= 100;
	strcpy(context, colval[0]);
	return 1;
}

void buy_card_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	char buf[1+sizeof(pack_head)+sizeof(buy_card_res)+1] = {0};
	pack_head send_head;
	buy_card_res send_body;
	buy_card_req recv_body;
	memcpy(&recv_body, &context.recv_pack[1+sizeof(pack_head)], sizeof(buy_card_req));
	memcpy(&send_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	buf[1+sizeof(pack_head)+sizeof(buy_card_res)] = '<';
	strcpy(send_head.acDev_id, clients[context.index].dev_id);
	send_head.iBody_Size = sizeof(buy_card_res);
	send_head.iPack_Sum = 1;
	send_head.iPack_Seq = 1;
	strcpy(send_head.acFunc_ID, "3001");
	send_body.iCard_Count = recv_body.iCard_Count;
	char* temp = get_trans_flowid();
	strcpy(send_body.acTrans_Flowid, temp);
	strcpy(send_body.acCard_NO, temp);
	free(temp);
	memcpy(&buf[1], &send_head, sizeof(pack_head));
	memcpy(&buf[1+sizeof(pack_head)], &send_body, sizeof(buy_card_res));
	send(clients[context.index].client_fd, (void*)&buf, sizeof(buf), MSG_DONTWAIT);
	free(context.recv_pack);
}

void que_detail_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	char buf[1+sizeof(pack_head)+sizeof(det_bill_res)+1] = {0};
	pack_head send_head;
	det_bill_res send_body;
	det_bill_req recv_body;
	memcpy(&recv_body, &context.recv_pack[1+sizeof(pack_head)], sizeof(det_bill_req));
	memcpy(&send_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	strcpy(send_head.acDev_id, clients[context.index].dev_id);
	send_head.iPack_Sum = 1;
	send_head.iPack_Seq = 1;
	strcpy(send_head.acFunc_ID, "2003");
	char* temp = get_trans_flowid();
	strcpy(send_body.acTrans_Flowid, temp);
	free(temp);
	//char detail_content[100][100] = {0};
	char stm[100] = {0};
	sprintf(stm, "select * from detail_info where date >= %s and date <= %s and phone_num = %s;",
		recv_body.acMonth_Begin, recv_body.acMonth_End, recv_body.acPhone_Num);
	if (SQLITE_OK == DB_exec(stm, que_detail_cb, &buf[1+sizeof(pack_head)], NULL))
	{
		send_head.iBody_Size = detail_index;
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		buf[1+sizeof(pack_head)+detail_index] = '<';
		send(clients[context.index].client_fd, (void*)&buf, detail_index+sizeof(pack_head)+2, MSG_DONTWAIT);
	}
	else
	{
		send_head.iBody_Size = 0;	
		strcpy(send_head.acErr_Code, "111");
		strcpy(send_head.acErr_Msg, "未找到记录或信息量太大\n");
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		buf[1+sizeof(pack_head)] = '<';
		send(clients[context.index].client_fd, (void*)&buf, sizeof(pack_head)+2, MSG_DONTWAIT);
	}
	detail_index = 0;
	free(context.recv_pack);
}

int que_detail_cb(void* context, int n, char** colval, char** colname)
{
	//printf("%d\n", n);
	char temp[100] = {0};
	sprintf(temp, "%s  %s  %s  %s  %s  %s,", colval[2], colval[3], 
		colval[4], colval[5], colval[6], colval[7]);
	memcpy(&(((char*)context)[detail_index]), temp, strlen(temp));
	//printf("%d\n", strlen(temp));
	//printf("%s\n",temp);

	detail_index += strlen(temp);
	//printf("%d\n", detail_index);
	//超过包体大小
	if (detail_index > 1024)
	{
		return 1;
	}
	return 0;
}

void que_bill_service(ser_cont context)
{
	int send_fd = clients[context.index].client_fd;
	char buf[1+sizeof(pack_head)+sizeof(bill_res)+1] = {0};
	pack_head send_head;
	bill_res send_body;
	bill_req recv_body;
	memcpy(&recv_body, &context.recv_pack[1+sizeof(pack_head)], sizeof(bill_req));
	memcpy(&send_head, &context.recv_pack[1], sizeof(pack_head));
	buf[0] = '~';
	strcpy(send_head.acDev_id, clients[context.index].dev_id);
	send_head.iPack_Sum = 1;
	send_head.iPack_Seq = 1;
	strcpy(send_head.acFunc_ID, "2002");
	char* temp = get_trans_flowid();
	strcpy(send_body.acTrans_Flowid, temp);
	free(temp);
	char bill_content[1024] = {0};
	char stm[100] = {0};
	sprintf(stm, "select * from bill_info where date = %s and phone_num = %s;", recv_body.acMonth, recv_body.acPhone_Num);
	if (SQLITE_ABORT == DB_exec(stm, que_bill_cb, bill_content, NULL))
	{		
		send_head.iBody_Size = 150;
		strcpy(send_body.acBill_Content, bill_content);
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		memcpy(&buf[1+sizeof(pack_head)], &send_body, 150);
		buf[1+sizeof(pack_head)+150] = '<';
		send(clients[context.index].client_fd, (void*)&buf, 1+sizeof(pack_head)+150+1, MSG_DONTWAIT);
		free(context.recv_pack);
		return;
	}
	else
	{
		send_head.iBody_Size = 0;	
		strcpy(send_head.acErr_Code, "111");
		strcpy(send_head.acErr_Msg, "数据库中无该记录\n");
		memcpy(&buf[1], &send_head, sizeof(pack_head));
		memcpy(&buf[1+sizeof(pack_head)], &send_body, sizeof(bill_res));
		buf[1+sizeof(pack_head)] = '<';
		send(clients[context.index].client_fd, (void*)&buf, sizeof(pack_head)+2, MSG_DONTWAIT);
		free(context.recv_pack);
	}

}

int que_bill_cb(void* context, int n, char** colval, char** colname)
{
	char* phone = "客户手机:";
	char* date = "账单时间:";
	char* sum = "账单总额:";
	char* msg = "短信息:";
	char* call = "本地通话费:";
	char* gprs = "GPRS上网费:";
	char* other = "增值业务:";
	char* sp = "第三方SP业务:";
	sprintf(context, "%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s%s\n", phone, colval[0],
		date, colval[1], sum, colval[2], msg, colval[3], call, colval[4], gprs, colval[5], 
		other, colval[6], sp, colval[7]);
	//printf("%d\n", strlen(context));
	return 1;
}

char* get_trans_flowid()
{
	time_t now;
	struct tm *timenow = NULL;
	time(&now);
	timenow = localtime(&now);
	char *temp = (char *)malloc(23);
	sprintf(temp, "%4d%02d%02d%02d%02d%02d%0004d%00005ld", timenow ->tm_year + 1900,
		timenow ->tm_mon + 1, timenow ->tm_mday, timenow ->tm_hour, timenow ->tm_min, timenow ->tm_sec, 1001, iRand);
	//printf("%s\n",temp);
	iRand++;
	if (iRand > 99999)
	{
		iRand = 0;
	}
	return temp;
}




