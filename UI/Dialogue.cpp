/**********************************************************************

*文件名：
		Dialogu.cpp
*文件描述：
		CDialogu类的封装
*创建者：
		黄彬

*创建时间：
2011-8-9

******************************************************************/


#include "Dialogue.h"

CDialogue::CDialogue(const char *caption,int height,int width,int scry,int scrx):CLable(caption,height,width,scry,scrx)
{
	this->pWin =NULL;
	this->nIndex = 0;
}

CDialogue::~CDialogue()
{
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
void CDialogue::draw()
{
	this->pWin = newwin(this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
	wbkgd(this->pWin,COLOR_PAIR(3));
	mvwprintw(this->pWin,this->m_iHeight/2,(this->m_iWidth-strlen(acCaption))/2,"%s",this->acCaption);
	wrefresh(this->pWin);
}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CDialogue::getControlType()
{
	return DIALOGUE;
}

/*****************************
*函数名：show
*功能：画出窗口并驱动
*参数：无
*返回值:类型值
******************************/
void CDialogue::show()
{
	this->draw();

	this->driver(1);
}




/*****************************
*函数名：driver
*功能：驱动
*参数：无
*返回值:无
******************************/
int CDialogue::driver(int KeyNum)
{
    curs_set(0);
	sleep(1);
}

