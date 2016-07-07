#ifndef CMAINCONTROL_H
#define CMAINCONTROL_H


#include "Form.h"



class CMainControl
{
public:
	CMainControl();
	virtual ~CMainControl();
	void show();		//展示
protected:
	void init();		//创建
	void addfunction();	//添加功能框
	void driver();		//驱动
	CForm * find(int index);	//查找窗口
private:
	CMyList pListControl;       //链表
	CBaseControl *control;      //窗口指针
	int height;     //窗口高
	int width;      //窗口宽
	int scry;       //窗口y坐标
	int scrx;       //窗口x坐标
	
};

#endif // CMAINCONTROL_H
