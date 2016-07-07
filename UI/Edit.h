#ifndef CEDIT_H
#define CEDIT_H

#include "BaseControl.h"

class CEdit :public CBaseControl
{
    public:
        CEdit(int height,int width,int scry,int scrx,int size,int type);
        virtual ~CEdit();
        virtual void draw();              //画出窗口
        virtual void focus();               //获得焦点
        virtual void unfocus();             //取消焦点
        virtual int getControlType();     //控件类型
        virtual int driver(int KeyNum);      //驱动
        char* getCaption();                 //取得文本内容
    protected:
        char acCaption[30];                //保存输入内容
		int CaptionSize;			//编辑框可输入字数
		int nType;					//编辑框可输入字符的类型 0:字母+数字 1:数字 2:字母
    private:
};

#endif //CEDIT_H


