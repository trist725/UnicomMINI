/**********************************************************************

*文件名：
		MainControl.cpp
*文件描述：
		CMainControl类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/

#include "MainControl.h"
#include "Edit.h"
#include "Button.h"
#include "Lable.h"
#include "Option.h"
#include "CallBack.h"

CMainControl::CMainControl()
{

}

CMainControl::~CMainControl()
{

}

/*****************************
*函数名：init
*功能：窗口初始化
*参数：无
*返回值:无
******************************/
void CMainControl::init()
{
	this->addfunction();
}


/*****************************
*函数名：show
*功能：画出窗口并驱动
*参数：无
*返回值:无
******************************/
void CMainControl::show()
{
    this->init();

    this->driver();

}


/*****************************
*函数名：driver
*功能:驱动
*参数：无
*返回值:无
******************************/
void CMainControl::driver()
{
    CForm *curform;
    int formReturn=0,now=1;


    curform = this->find(now);

    while(curform != NULL)
    {
        wbkgd(stdscr,COLOR_PAIR(7));

		refresh();

		curs_set(0);

        formReturn = curform->show();

        curform->unshow();

        switch(formReturn)
        {
        case 0:
			now = 1;

			break;

        case 1:

            now = formReturn;

            break;

        case -1:
            return;
            break;

        default:
            break;
        }

        curform = this->find(now);
    }

}


/*****************************
*函数名：find
*功能：获取指定位置的窗口
*参数：无
*返回值:无
******************************/
CForm * CMainControl::find(int index)
{
	return ((CForm *)(pListControl[index]));
}


/*****************************
*函数名：addfunction
*功能：添加功能界面
*参数：无
*返回值:无
******************************/
void CMainControl::addfunction()
{
	CForm *form;

	
	
   //调试选择
    form = new CForm(24,80,0,0,CALLBACK_parameter);
	
    control = new CLable("测试时长( 秒 ):",3,16,1,2);
    form->addControl(*control);
    control = new CLable("模拟设备数:",3,16,1,42);
    form->addControl(*control);
    control = new CLable("发送间隔(毫秒):",3,16,5,2);
    form->addControl(*control);
	control = new CLable("业务类型",1,10,9,35);
    form->addControl(*control);
	control = new CLable("支持←→键选择,tab切换,选择框空格,enter支持,服务器配置可通过config.ini修改",1,76,21,3);
    form->addControl(*control);
	control = new CLable("为了测试稳定,请30毫秒起测否则服务器丢包严重",1,50,22,1);
    form->addControl(*control);

	
    control = new CEdit(3,16,1,20,4,1);
    form->addControl(*control);
    control = new CEdit(3,16,1,60,3,1);
    form->addControl(*control);
	control = new CEdit(3,16,5,20,8,1);
    form->addControl(*control);

	control = new COption("是否处理应答",1,24,6,50);
	form->addControl(*control);
	control = new COption("一卡通充值",1,24,11,10);
	form->addControl(*control);
	control = new COption("购买电子卡",1,24,11,50);
	form->addControl(*control);
	control = new COption("手机余额查询",1,24,13,10);
	form->addControl(*control);
	control = new COption("手机历史账单",1,24,13,50);
	form->addControl(*control);
	control = new COption("手机历史详单",1,24,15,10);
	form->addControl(*control);
	
	
    control = new CButton("开始测试",3,12,17,23,1);
    form->addControl(*control);
    control = new CButton("关    闭",3,12,17,45,-1);
    form->addControl(*control);
	
    this->pListControl.addNode(form);

	
   
}


