/**********************************************************************

*文件名：
		Button.cpp
*文件描述：
		CButton类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/


#include <unistd.h>
#include "Button.h"

CButton::CButton(const char *caption,int height,int width,int scry,int scrx,int buttonID):CBaseControl(height,width,scry,scrx)
{
	this->acCaption = new char [strlen(caption) +1];
	strcpy(this->acCaption,caption);
	this->pWin = NULL;
	this->shadow = NULL;
	this->nIndex = 0;
	nButtonID = buttonID;
}

CButton::~CButton()
{
	delete acCaption;
	if (this->pWin != NULL)
	{
		delwin(this->pWin);
	}
	if (this->shadow != NULL)
	{
		delwin(this->shadow);
	}
}

/*****************************
*函数名：draw
*功能：画出窗口
*参数：无
*返回值:无
******************************/
void CButton::draw()
{
    //画出阴影效果
	this->shadow = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty+1,this->m_iStartx+2);
	wbkgd(this->shadow,COLOR_PAIR(10));
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());

    //画出按钮
	this->pWin = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
	wbkgd(this->pWin,COLOR_PAIR(9));
	box(this->pWin,0,0);
	mvwprintw(this->pWin,this->m_iHeight/2,(this->m_iWidth-strlen(acCaption))/2,"%s",this->acCaption);
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());
}


/*****************************
*函数名：focus
*功能：获得焦点
*参数：无
*返回值:无
******************************/
void CButton::focus()
{

    wbkgd(this->pWin,COLOR_PAIR(3));
	curs_set(0);
    wrefresh(this->pWin);

}

/*****************************
*函数名：unfocus
*功能：失去焦点
*参数：无
*返回值:无
******************************/
void CButton::unfocus()
{
    wbkgd(this->pWin,COLOR_PAIR(9));
	curs_set(0);
    wrefresh(this->pWin);
}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CButton::getControlType()
{
	return BUTTON;
}

/*****************************
*函数名：press
*功能：按钮效果--按下
*参数：无
*返回值:类型值
******************************/
void CButton::press()
{
	mvwin(this->pWin,this->pParentControl->getScry()+this->m_iStarty+1,this->pParentControl->getScrx()+this->m_iStartx+2);
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());
	sleep(1);
	mvwin(this->pWin,this->pParentControl->getScry()+this->m_iStarty,this->pParentControl->getScrx()+this->m_iStartx);
	wbkgd(this->shadow,COLOR_PAIR(10));
	wrefresh(this->shadow);
	wbkgd(this->pWin,COLOR_PAIR(3));
	wrefresh(this->pWin);
	
}

/*****************************
*函数名：getButtonID
*功能：返回按钮ID
*参数：无
*返回值:类型值
******************************/
int CButton::getButtonID()
{
	return this->nButtonID;
}


/*****************************
*函数名：driver
*功能：驱动
*参数：无
*返回值:无
******************************/
int CButton::driver(int KeyNum)
{
    int nGet;

    //设置特殊键有效
    keypad(this->pWin,TRUE);

    this->focus();

    do
    {
        nGet=wgetch(this->pWin);

        switch(nGet)
        {

        case MYKEY_ENTER:

            this->press();

        case MYKEY_UP:
        case MYKEY_DOWN:
        case MYKEY_LEFT:
        case MYKEY_RIGHT:
        case MYKEY_TAB:
        case MYKEY_ESC:
        case MYKEY_PAGEUP:
        case MYKEY_PAGEDOWN:

            return nGet;

            break;


        default:
            break;
        }

    }while(1);


}


/*****************************
*函数名：changeCaption
*功能：改变按钮名字
*参数：无
*返回值:无
******************************/
void CButton::changeCaption(char *caption)
{
	delete this->acCaption;
	this->acCaption = new char [strlen(caption) +1];
	strcpy(this->acCaption,caption);
	mvwprintw(this->pWin,this->m_iHeight/2,(this->m_iWidth-strlen(acCaption))/2,"%s",this->acCaption);
	wrefresh(this->pWin);
	
}


