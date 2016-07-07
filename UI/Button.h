#ifndef CBUTTON_H
#define CBUTTON_H

#include "BaseControl.h"

class CButton :public CBaseControl
{
    public:
        CButton(const char *caption,int height,int width,int scry,int scrx,int buttonID);
        virtual ~CButton();
        virtual void draw();              //画出窗口
        virtual void focus();               //获得焦点
        virtual void unfocus();             //取消焦点
        virtual int getControlType();     //控件类型
        virtual int driver(int KeyNum);      //驱动
        void press();           //按钮效果
        int getButtonID();     //获得按钮ID
		void changeCaption(char *caption);
    protected:
        WINDOW *shadow;     //阴影窗口坐标
        char *acCaption;     //保存窗口内容
        int nButtonID;      //事件ID
    private:
};

#endif // CBUTTON_H


