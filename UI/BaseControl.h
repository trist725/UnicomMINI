#ifndef _BASECONTROL_H_
#define _BASECONTROL_H_

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define FORM	 0
#define LABLE	 1
#define BUTTON	 2
#define EDIT	 3
#define PASSWD	 4
#define OPTION	 5
#define REFRESH  6
#define DIALOGUE 7

#define MYKEY_TAB			9
#define MYKEY_ENTER			10
#define MYKEY_BACK			263
#define MYKEY_UP	    	259
#define MYKEY_DOWN			258
#define MYKEY_LEFT			260
#define MYKEY_RIGHT			261
#define MYKEY_F2			266
#define MYKEY_F3			267
#define MYKEY_F4			268
#define MYKEY_F5			269
#define MYKEY_ESC               27
#define MYKEY_SPACE			32
#define MYKEY_PAGEUP    339
#define MYKEY_PAGEDOWN    338



class CBaseControl  
{
    public:
        CBaseControl(int height,int width,int scry,int scrx);
        virtual ~CBaseControl();
        virtual void draw()=0;              //画出窗口
        virtual void focus();               //获得焦点
        virtual void unfocus();             //取消焦点
        virtual int getControlType()=0;     //控件类型
        virtual int driver(int KeyNum);      //驱动
        void setName(char *Name);           //设置控件名
        char * getFormName() const;         //获取控件名
        WINDOW *getWindow() const;          //获得窗口指针
        void SetParent(CBaseControl &parent);       //设置父控件
        void SetIndex(int index);		//设置index
        int getIndex()const;            //获得index
        int getScry() const;        //获取y坐标
        int getScrx() const;        //获取x坐标
    protected:
        WINDOW *pWin;		// WINDOW *pWin当前控件对应NCurse Window的指针
        int nIndex;			//控件跳转索引
        char m_acName[30];      //控件名字
        int m_iStartx;          //起始x坐标
        int m_iStarty;          //起始Y坐标
        int m_iHeight;          //高度
        int m_iWidth;           //宽度
        CBaseControl *pParentControl;	//存放父控件指针
    private:
};

#endif

