/**********************************************************************

*文件名：
		Lable.cpp
*文件描述：
		CLable类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/

#include "Lable.h"

CLable::CLable(const char *caption,int height,int width,int scry,int scrx):CBaseControl(height,width,scry,scrx)
{
	acCaption = new char [strlen(caption) +1];
	strcpy(acCaption,caption);
}

CLable::~CLable()
{
	delete acCaption;
	if (this->pWin != NULL)
	{
		delwin(this->pWin);
	}
}

/*****************************
*函数名：draw
*功能：画出窗口
*参数：无
*返回值:无
******************************/
void CLable::draw()
{
	this->pWin = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
	mvwprintw(this->pWin,this->m_iHeight/2,(this->m_iWidth-strlen(acCaption))/2,"%s",this->acCaption);
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());
}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CLable::getControlType()
{
	return LABLE;
}



