/****************************************************************************************
* 	这是一个贪吃蛇游戏程序。
* 	这个程序最初的版本是在4月份完成的，但是那时候对于C++的标准库以及面向对象的程序
* 设计都只是一知半解而已，因此写出来的代码越来越乱，C++98与C++11标准的代码夹杂在一起，
* 简直就像车祸现场。
*	后来真正学习了一些类的知识，知道了构造函数、封装等等的作用，以及将类写在头文件里的好
* 处，同时也更加深入地学习了STL相关的知识。于是，我决定花一点点的时间重写这个程序。
*	在原来的设计中，除了经典模式外，4.0版本新增了新的玩法“炸弹模式”，但是由于代码的质
* 量太差，维护成本实在太高，那个程序中错误百出。由于这一块内容涉及到继承的概念，我决定
* 等到学完了再来实现这个功能。
*	在整个程序的设计过程中，我还是要感谢远在英国的校友郑晖耀。他对我的帮助不容忽视，也
* 是他的一张UML图帮助我建立了最初的“封装”的思想，我才得以还算顺利地跨过面向过程到面向
* 对象的这个门槛。
*                                     Produced by GKWuxi, Wuxi Big Bridge Academy.
*****************************************************************************************/

#include "Classic_mode.h"
#include "SuperSpeed_mode.h"

using namespace std;

void gotoxy(int x, int y) {  //移动光标到第x列第y行
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void HideCursor() {          //隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int drawMenu() {
	system("cls");
	gotoxy(5, 5); cout << "┏";
	gotoxy(29, 5); cout << "┓";
	gotoxy(5, 15); cout << "┗";
	gotoxy(29, 15); cout << "┛";
	for (int i = 6; i <= 14; i++) {
		gotoxy(5, i); cout << "┃";
		gotoxy(29, i); cout << "┃";
	}
	for (int i = 7; i <= 28; i += 2) {
		gotoxy(i, 5); cout << "━";
		gotoxy(i, 15); cout << "━";
	}
	gotoxy(9, 7);
	cout << "欢迎来到贪吃蛇游戏";
	gotoxy(15, 9);
	cout << "请选择:";
	gotoxy(11, 11);
	cout << "经典模式 请按1";
	gotoxy(11, 12);
	cout << "极速模式 请按2";
	cout << flush;
	while (1) { //等待输入进入游戏 
		if (_getch() == 49) return 1;
		if (_getch() == 50) return 2;
	}
}

void run_mode(Classic_mode& mode) { //向上造型
	mode.help();
	while (1) if (_getch() == 32) break;
	mode.init();
	mode.run();
}

int main() {
	HideCursor();
	bool restart;
	do {
		restart = 0;
		int mode = drawMenu();
		switch (mode) {
		case 1: {	/*经典模式*/
			Classic_mode cm(20);
			run_mode(cm);
			break;
		    }
		case 2: {	/*极速模式*/
			SuperSpeed_mode ssm(20);
			run_mode(ssm);
			break;
			}      
		}
		gotoxy(13, 13);
		cout << "[空格] 返回主菜单";
		gotoxy(14, 14);
		cout << "[回车] 退出游戏";
		while (1) {
			if (_getch() == 32) { restart = 1; break; }
			if (_getch() == 13) break;
		}
	} while (restart);
	return 0;
}