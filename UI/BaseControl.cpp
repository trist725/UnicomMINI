/**********************************************************************

*文件名：
		BaseControl.cpp
*文件描述：
		CBaseControl类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/


#include "BaseControl.h"



//构造函数
CBaseControl::CBaseControl(int height,int width,int scry,int scrx)
{
	m_iHeight = height;
	m_iWidth = width;
	m_iStarty = scry;
	m_iStartx = scrx;
	memset(m_acName,0,30);
    this->pWin =NULL;
	this->nIndex = 0;
}


//析构函数 
CBaseControl::~CBaseControl()
{

}

/*****************************
*函数名：setparent
*功能：设置父窗口指针
*参数：无
*返回值:无
******************************/
void CBaseControl::SetParent(CBaseControl &parent)
{
	this->pParentControl = &parent;
}

/*****************************
*函数名：getWindow
*功能：获得窗口指针
*参数：无
*返回值:无
******************************/
WINDOW * CBaseControl::getWindow() const
{
	return this->pWin;
}

/*****************************
*函数名：setindex
*功能：设置索引号
*参数：无
*返回值:无
******************************/
void CBaseControl::SetIndex(int index)
{
	this->nIndex = index;
}

/*****************************
*函数名：getindex
*功能：获得索引号
*参数：无
*返回值:无
******************************/
int CBaseControl::getIndex() const
{
	return this->nIndex;
}


/*****************************
*函数名：driver
*功能：驱动
*参数：无
*返回值:无
******************************/
int CBaseControl::driver(int KeyNum)
{
    return KeyNum;
}

/*****************************
*函数名：focus
*功能：获得焦点
*参数：无
*返回值:无
******************************/
void CBaseControl::focus()
{

}

/*****************************
*函数名：unfocus
*功能：取消焦点
*参数：无
*返回值:无
******************************/
void CBaseControl::unfocus()
{

}


/*****************************
*函数名：getscry
*功能：获取y坐标
*参数：无
*返回值:无
******************************/
int CBaseControl::getScry() const
{
    return m_iStarty;
}


/*****************************
*函数名：getscrx
*功能：获取x坐标
*参数：无
*返回值:无
******************************/
int CBaseControl::getScrx() const
{
    return m_iStartx;
}


