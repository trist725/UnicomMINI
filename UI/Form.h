#ifndef CFORM_H
#define CFORM_H

#include "BaseControl.h"
#include "CMyList.h"

//#define FIRST   500

typedef int (*PFUN)(void*,int);

class CForm :public CBaseControl
{
    public:
        CForm(int height,int width,int scry,int scrx,PFUN pfun);
        virtual ~CForm();
        virtual void draw();              //画出窗口
        //virtual void focus();               //获得焦点
        //virtual void unfocus();             //取消焦点
        virtual int getControlType();     //控件类型
        virtual void addControl(CBaseControl &control);     //添加控件
        virtual int show();     //创建窗口并驱动
        virtual void unshow();      //删除窗口
        char * getEditCaption(int index);       //获取指定位置的编辑框的文本
		bool getOption(int index);			//获取指定位置的选择框的选择
        CBaseControl * getListNow(); //获取当前链表指针指向的窗口指针
		bool changeButtonCaption(char *caption,int index);
        void delAll();   //删除所有子窗口
		void changeRefresh();
		void closeRefresh();
    protected:
        virtual int driver(int KeyNum);      //驱动
        CBaseControl* find(int index);      //获取索引位置的窗口
        CMyList pListControl;               //链表
        int snIndex;            //子窗口的个数
        PFUN pFunction;         //函数指针
    private:
};

#endif // CFORM_H


