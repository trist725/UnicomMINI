/**********************************************************************

*文件名：
		Edit.cpp
*文件描述：
		CEdit类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/

#include "Edit.h"


//构造函数
CEdit::CEdit(int height,int width,int scry,int scrx,int size,int type):CBaseControl(height,width,scry,scrx)
{
	this->pWin = NULL;
	this->nIndex = 0;
	this->CaptionSize = size;
	memset(acCaption,0,30);
	this->nType = type;
}


//析构函数
CEdit::~CEdit()
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
void CEdit::draw()
{
	this->pWin = derwin(this->pParentControl->getWindow(),this->m_iHeight,this->m_iWidth,this->m_iStarty,this->m_iStartx);
	box(this->pWin,0,0);
	wbkgd(this->pWin,COLOR_PAIR(1));
	touchwin(this->pParentControl->getWindow());
	wrefresh(this->pParentControl->getWindow());

	memset(acCaption,0,30);
}


/*****************************
*函数名：focus
*功能：获得焦点
*参数：无
*返回值:无
******************************/
void CEdit::focus()
{
    curs_set(1);
    wrefresh(this->pWin);
}

/*****************************
*函数名：unfocus
*功能：失去焦点
*参数：无
*返回值:无
******************************/

void CEdit::unfocus()
{

    curs_set(0);
    wrefresh(this->pWin);

}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CEdit::getControlType()
{
	return EDIT;
}

/*****************************
*函数名：getCaption
*功能：返回文本内容
*参数：无
*返回值:类型值
******************************/
char* CEdit::getCaption()
{
	return acCaption;
}

/*****************************
*函数名：driver
*功能：驱动
*参数：无
*返回值:类型值
******************************/
int CEdit::driver(int KeyNum)
{
	//static int offset=0;
	int result=1,nGet=0,i=0;

	//设置特殊键有效
	keypad(this->pWin,TRUE);

	this->focus();

	while(1)
	{

		//保留以输入信息
		if (this->getControlType() == EDIT)
		{
			mvwprintw(this->pWin,1,1,"%s",this->acCaption);
		}
		else if (this->getControlType() == PASSWD)
		{
			for(i=0;i<strlen(this->acCaption);i++)
			{
				mvwprintw(this->pWin,1,1+i,"%c",'*');
			}

			if (strlen(this->acCaption) == 0)
			{
				wmove(this->pWin,1,1);
			}
		}
		wrefresh(this->pWin);

		nGet=wgetch(this->pWin);

		switch(nGet)
		{

		case MYKEY_BACK:

			if(strlen(acCaption) > 0)
			{
				wprintw(this->pWin,"\b \b");
				wrefresh(this->pWin);

				acCaption[strlen(acCaption)-1] = '\0';

			}
			else beep();

			break;


		case MYKEY_LEFT:
			this->unfocus();
			return MYKEY_LEFT;

			break;

		case MYKEY_TAB:
		case MYKEY_ENTER:
		case MYKEY_RIGHT:

			this->unfocus();
			return MYKEY_TAB;

			break;

		case MYKEY_UP:
		case MYKEY_DOWN:
        case MYKEY_ESC:
        case MYKEY_PAGEUP:
        case MYKEY_PAGEDOWN:

            return nGet;

            break;

		default:

			if (nType == 0)
			{
				if ( (nGet >= 'a' && nGet <= 'z') || (nGet >= 'A' && nGet <= 'Z') || (nGet >= '0' && nGet <= '9') )
				{
					//限制输入个数 此处修改
					if (strlen(acCaption) < CaptionSize)
					{
						acCaption[strlen(acCaption)] = nGet;
					}
					else beep();
				}
			}
			else if (nType == 1)
			{
				if (nGet >= '0' && nGet <= '9')
				{
					//限制输入个数 此处修改
					if (strlen(acCaption) < CaptionSize)
					{
						acCaption[strlen(acCaption)] = nGet;
					}
					else beep();
				}
			}
			else if (nType == 2)
			{
				if ( (nGet >= 'a' && nGet <= 'z') || (nGet >= 'A' && nGet <= 'Z'))
				{
					//限制输入个数 此处修改
					if (strlen(acCaption) < CaptionSize)
					{
						acCaption[strlen(acCaption)] = nGet;
					}
					else beep();
				}
			}
			break;
		}
	}
}

