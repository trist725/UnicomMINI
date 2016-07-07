#ifndef CLABLE_H
#define CLABLE_H

#include "BaseControl.h"

class CLable : public CBaseControl
{
    public:
        CLable(const char *caption,int height,int width,int scry,int scrx);
        virtual ~CLable();
        void draw();        //画出窗口
        int getControlType();       //控件类型
    protected:
        char *acCaption;    //保存内容
    private:
};

#endif // CLABLE_H
