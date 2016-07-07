/**********************************************************************

*文件名：
		Main.cpp
*文件描述：
		main函数
*创建者：
		黄彬

*创建时间：
2011-8-4

******************************************************************/

#include "MainControl.h"
#include "CallBack.h"

void startGUI();
void endGUI();
void GUI_color();

int main()
{

	//创建共享内存
	CALLBACK_forward_init();

	//NCurses环境初始化
	//.........................
	startGUI();
	GUI_color();

	refresh();

	CMainControl Main_control;

	Main_control.show();

	endGUI();
	//.........................
	//NCurses环境关闭


	return 0;
}


/*****************************
*函数名：startGUI
*功能：ncurses初始化
*参数：无
*返回值:无
******************************/
void startGUI()
{
	initscr();
	start_color();
	noecho();
	cbreak();
    refresh();
	curs_set(0);
}

/*****************************
*函数名：endGUI
*功能：退出ncurses
*参数：无
*返回值:无
******************************/
void endGUI()
{
	endwin();
}


/*****************************
*函数名：GUI_color
*功能：颜色搭配
*参数：无
*返回值:无
******************************/
void GUI_color()
{
    init_pair(1,COLOR_BLACK,COLOR_WHITE);
	init_pair(2,COLOR_BLACK,COLOR_CYAN);
	init_pair(3,COLOR_BLACK,COLOR_MAGENTA);
	init_pair(4,COLOR_CYAN,COLOR_BLACK);
	init_pair(5,COLOR_WHITE,COLOR_BLUE);
	init_pair(6,COLOR_WHITE,COLOR_BLACK);
	init_pair(7,COLOR_BLUE,COLOR_BLUE);
	init_pair(8,COLOR_WHITE,COLOR_BLUE);
	init_pair(9,COLOR_BLACK,COLOR_RED);
	init_pair(10,COLOR_BLACK,COLOR_BLACK);
	init_pair(11,COLOR_CYAN,COLOR_CYAN);
}
