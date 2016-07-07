/**********************************************************************

*文件名：
		Option.cpp
*文件描述：
		COption类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/


#include "Option.h"

COption::COption(const char *caption,int height,int width,int scry,int scrx):CBaseControl(height,width,scry,scrx)
{
	acCaption = new char [strlen(caption) +1];
	strcpy(acCaption,caption);
	choice = false ;
}

COption::~COption()
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
void COption::draw()
{
	this->pWin = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
	//wbkgd(this->shadow ,COLOR_PAIR(1));
	//box(this->shadow ,0,0);
	//touchwin(this->pWin);
	mvwprintw(this->pWin,this->m_iHeight/2,0,"【 】");
	mvwprintw(this->pWin,this->m_iHeight/2,6,"%s",this->acCaption);
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());
}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int COption::getControlType()
{
	return OPTION;
}


/*****************************
*函数名：driver
*功能：驱动
*参数：无
*返回值:键值
******************************/
int COption::driver(int KeyNum)
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
		case MYKEY_SPACE:
			

            this->press();

			break;
			
        case MYKEY_UP:
        case MYKEY_DOWN:
        case MYKEY_LEFT:
        case MYKEY_RIGHT:
        case MYKEY_TAB:
        case MYKEY_ESC:
        case MYKEY_PAGEUP:
        case MYKEY_PAGEDOWN:
			
			this->unfocus();
            return nGet;
			
            break;
			
			
        default:
            break;
        }
		
    }while(1);
	
	
}


/*****************************
*函数名：press
*功能：按钮效果--按下
*参数：无
*返回值:类型值
******************************/
void COption::press()
{
	if (choice == false)
	{
		choice = true;

		
		mvwprintw(this->pWin,this->m_iHeight/2,2,"*");
	}
	else
	{
		choice = false;

		mvwprintw(this->pWin,this->m_iHeight/2,3,"\b \b");
	}

	wrefresh(this->pWin);
}



/*****************************
*函数名：getOption
*功能：返回选择结果
*参数：无
*返回值:选择结果
******************************/
bool COption::getOption()
{
	return choice;
}


/*****************************
*函数名：focus
*功能：获得焦点
*参数：无
*返回值:无
******************************/
void COption::focus()
{

	wbkgd(this->pWin ,COLOR_PAIR(1));
	wrefresh(this->pWin);
}

/*****************************
*函数名：unfocus
*功能：取消焦点
*参数：无
*返回值:无
******************************/
void COption::unfocus()
{
	wbkgd(this->pWin ,COLOR_PAIR(2));
	wrefresh(this->pWin);
}

