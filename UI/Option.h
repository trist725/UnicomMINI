#ifndef _OPTION_H_
#define _OPTION_H_

#include "BaseControl.h"

class COption : public CBaseControl
{
    public:
        COption(const char *caption,int height,int width,int scry,int scrx);
        virtual ~COption();
        void draw();				//画出窗口
        int getControlType();       //控件类型
		bool getOption();			//返回选择
		int driver(int KeyNum);      //驱动
		void press();           //按钮效果
		void focus();               //获得焦点
        void unfocus();             //取消焦点
    protected:
        char *acCaption;    //保存内容
		bool choice;		//选择
    private:
};

#endif //_OPTION_H_




