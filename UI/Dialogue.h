#ifndef CDIALOGUE_H
#define CDIALOGUE_H

#include <unistd.h>
#include "Lable.h"

class CDialogue :public CLable
{
public:
	CDialogue(const char *caption,int height,int width,int scry,int scrx);
	virtual ~CDialogue();
	void draw();        //画出窗口
	int getControlType();       //获得类型
	void show();        //画出窗口并驱动
protected:
	int driver(int KeyNum);   //驱动
};

#endif // CDIALOGUE_H
