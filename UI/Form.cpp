/**********************************************************************

*文件名：
		Form.cpp
*文件描述：
		CForm类的封装
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/



#include "Form.h"
#include "Button.h"
#include "Edit.h"
#include "Option.h"
#include "Refresh.h"



CForm::CForm(int height,int width,int scry,int scrx,PFUN pfun):CBaseControl(height,width,scry,scrx)
{
	this->pWin = NULL;
	this->nIndex = 0;
	snIndex = 1;
    pFunction = pfun;
}

CForm::~CForm()
{

	if (this->pWin != NULL)
	{
		delwin(this->pWin);
	}

}

/*****************************
*函数名：addcontrol
*功能：添加控件
*参数：无
*返回值:无
******************************/

void CForm::addControl(CBaseControl &control)
{


	//为增加的控件设置父控件 为本窗口
	control.SetParent(*this);

	snIndex ++;

	this->pListControl.addNode(&control);

	//
}


/*****************************
*函数名：draw
*功能：画出窗口
*参数：无
*返回值:无
******************************/
void CForm::draw()
{

	//画出本窗口
	//得到当前窗口对应的NCURSES WINDOW *指针
	this->pWin = newwin(m_iHeight,m_iWidth,m_iStarty,m_iStartx);
	wbkgd(this->pWin,COLOR_PAIR(2));
	box(this->pWin,0,0);
	wrefresh(this->pWin);

	//画出窗口所有的子控件
	for (pListControl.begin();pListControl.hasNext();)
	{
		((CBaseControl *)(pListControl.Next()))->draw();
	}
}

/*****************************
*函数名：show
*功能：画出窗口并驱动
*参数：无
*返回值:无
******************************/
int CForm::show()
{
	this->draw();

	//进入窗口驱动函数
	return this->driver(1);

}

/*****************************
*函数名：unshow
*功能：删除窗口
*参数：无
*返回值:无
******************************/
void CForm::unshow()
{

	//删除窗口所有的子控件
	for (pListControl.begin();pListControl.hasNext();)
	{
		delwin(((CBaseControl *)(pListControl.Next()))->getWindow());

	}

	//删除本窗口
	delwin(this->getWindow());

	refresh();

}

/*****************************
*函数名：getControlType
*功能：返回类型
*参数：无
*返回值:类型值
******************************/
int CForm::getControlType()
{
	return FORM;
}

/*****************************
*函数名：driver
*功能：返回类型
*参数：无
*返回值:判断值
******************************/
int CForm::driver(int KeyNum)
{
	int nGet=MYKEY_TAB,index=1,i=0,nReturn;
    CBaseControl *pControl;


	///判断form里面是否有控件
	pControl =	this->find(index);

	while(pControl!=NULL)
	{

		//设置特殊键有效
        //keypad(pControl->getWindow(),TRUE);

        //pControl->focus(TURN_ON);

        nGet=pControl->driver(nGet);

		switch (nGet)
		{

		case MYKEY_LEFT:
			pControl->unfocus();

			index --;
			if(index <= 0)
			{
				index = snIndex-1;
			}
			pControl = this->find(index);


			break;

		case MYKEY_ENTER:

            if(pFunction != NULL)
            {
                nReturn = pFunction((void *)this,((CButton *)pControl)->getButtonID());

				if (nReturn == 999)
				{
					pControl = this->find(index);
				}
				else
				{		
					return nReturn;
				}
            }
            else return ((CButton *)pControl)->getButtonID();


			break;

		case MYKEY_TAB:
		case MYKEY_RIGHT:


			pControl->unfocus();

			index ++;
			if(index >= snIndex)
			{
				index = 1;
			}
			pControl = this->find(index);

			break;

// 		case MYKEY_ESC:
// 
// 
// 			return 0;
// 			break;

		default:
			break;
		}
	}

	return 0;

}


/*****************************
*函数名：find
*功能：找到索引位置的窗口
*参数：无
*返回值:窗口指针
******************************/
CBaseControl* CForm::find(int index)
{
	return ((CBaseControl *)(pListControl[index]));
}


/*****************************
*函数名：getOption
*功能：获取选择框的选择结果
*参数：无
*返回值:选择结果
******************************/
bool CForm::getOption(int index)
{
	int now=0;
    int num=0;
    int count= 0;
	
    count = this->pListControl.count();
	
    while(count > now)
    {
        now += 1;
        if ((((CBaseControl *)(pListControl[now]))->getControlType()) == OPTION)
        {
            num += 1;
            if(num == index)
            {
                return (((COption *)(pListControl[now]))->getOption());
            }
        }
    }
	
	
    return false;
}

/*****************************
*函数名：getEditCaption
*功能：获取窗体中编辑框的内容
*参数：无
*返回值:文本内容
******************************/
char * CForm::getEditCaption(int index)
{
    int now=0;
    int num=0;
    int count= 0;

    count = this->pListControl.count();

    while(count > now)
    {
        now += 1;
        if ((((CBaseControl *)(pListControl[now]))->getControlType()) == EDIT || (((CBaseControl *)(pListControl[now]))->getControlType()) == PASSWD)
        {
            num += 1;
            if(num == index)
            {
                return (((CEdit *)(pListControl[now]))->getCaption());
            }
        }
    }


    return NULL;


}

/*****************************
*函数名：getListNow
*功能：获取当前链表指针指向的窗口指针
*参数：无
*返回值:无
******************************/
CBaseControl * CForm::getListNow()
{
    return ((CBaseControl *)(this->pListControl.ReturnNow ()));
}

/*****************************
*函数名：changeButtonCaption
*功能：改变指定按钮的名字
*参数：无
*返回值:是否成功
******************************/
bool CForm::changeButtonCaption(char *caption,int index)
{
	int now=0;
    int num=0;
    int count= 0;
	
    count = this->pListControl.count();
	
    while(count > now)
    {
        now += 1;
        if ((((CBaseControl *)(pListControl[now]))->getControlType()) == BUTTON)
        {
            num += 1;
            if(num == index)
            {
               (((CButton *)(pListControl[now]))->changeCaption(caption));
			    return true;
            }
        }
    }
	
	
    return false;
}

/*****************************
*函数名：changeRefresh
*功能：改变刷新框的状态
*参数：无
*返回值:无
******************************/
void CForm::changeRefresh()
{
	int now=0;
    int count= 0;
	
    count = this->pListControl.count();
	
    while(count > now)
    {
        now += 1;
        if ((((CBaseControl *)(pListControl[now]))->getControlType()) == REFRESH)
        {
             (((CRefresh *)(pListControl[now]))->changeREFRESH());	 			 
			 (((CRefresh *)(pListControl[now]))->sendSINGAL());
        }
    }	
}


/*****************************
*函数名：closeRefresh
*功能：关闭刷新框的线程
*参数：无
*返回值:无
******************************/
void CForm::closeRefresh()
{
	int now=0;
    int count= 0;
	
    count = this->pListControl.count();
	
    while(count > now)
    {
        now += 1;
        if ((((CBaseControl *)(pListControl[now]))->getControlType()) == REFRESH)
        {
             (((CRefresh *)(pListControl[now]))->changeON());
			 (((CRefresh *)(pListControl[now]))->sendSINGAL());
        }
    }	
}


/*****************************
*函数名：delAll
*功能：删除链表内数据
*参数：无
*返回值:无
******************************/
void CForm::delAll()
{
    //删除窗口所有的子控件
	for (pListControl.begin();pListControl.hasNext();)
	{
		delwin(((CBaseControl *)(pListControl.Next()))->getWindow());
	}

	snIndex = 1;

    refresh();

    this->pListControl.delAll();

}



