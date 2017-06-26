#pragma once
#ifndef CLASSIC_MODE_H
#define CLASSIC_MODE_H

//#include <fstream>
//#include <cassert>
//extern std::ofstream debug("debug_out.txt");

#include <Windows.h>
#include <conio.h>
#include "Snake.h"
#include <random>
#include <set>
#include <cctype>
#include <map>
#include <string>
#include <iostream>
#include "Food.h"

#define SLEEPTIME 10

extern const int UP, DOWN, LEFT, RIGHT; //这些常量定义在"Snake.h"中
extern const int SPEED_LV[8] = { 300,200,100,50,25,15,10,5 }; //共有8档速度 
extern const int PAUSE = 32;        //空格的ASCII码为32 
const int SPEEDUP = 'j';
const int SPEEDDOWN = 'k';

void gotoxy(int, int);       //全局函数，移动光标，在"源.cpp"中定义

class Classic_mode {         //经典模式
protected:
	const int gameSize;
private:
	Snake snake;             //一条普通的蛇
	void drawInterface();    //画经典模式的界面
	Food food;               //存储食物坐标
	Food generateFood();     //产生食物
	void drawHelp();
protected:
	int score = 0;
	int speed = 2;           //当前速度在第几档
	bool died = 0, win = 0;
	virtual int receiveCommand();    //接收指令
	virtual void print(const std::string&); //专门用来画东西的函数
public:
	//定义Classic_mode的对象的时候，直接把snake构造了，顺便再造个食物，其它变量靠类内初始值
	Classic_mode(int gs = 20) : gameSize(gs), snake(3, gameSize), food(generateFood()) { } //{ debug << "Classic_mode constructor" << std::endl; }
	Classic_mode(int gs, int spd) : Classic_mode(gs) { speed = spd; } //委托构造函数
	virtual void help() {
//		debug << "Classic_mode::help()" << std::endl;
		system("cls");
		print("helpBox");       //画个提示框
		drawHelp();         /* Classic_mode 画出帮助信息界面 */
	}
	virtual void init() {
		//把该画的先画好
//		debug << "Classic_mode::init()" << std::endl;
		system("cls");
		drawInterface();    /* Classic_mode 画游戏界面 */
		food.draw();
		snake.draw();
		print("speed");
	}
	virtual void run() {
		while (!(died || win)) {                //既没死也没赢
			int command = receiveCommand();     //接收指令
			if (command != -1) snake.setDirection(command);  //设置前进方向
			Body old_back = snake.back();       //记录当前的蛇尾。在吃食物的时候需要把它恢复
			snake.cleanTail();
			snake.move_fd();                    //向前移动一步
			snake.drawHead();                   //把新的蛇头和第一节蛇身画上
			if (snake.ifEat(food.x, food.y)) {  //吃到了食物
				snake.restore_back(old_back);   //恢复之前删除的蛇尾
				snake.drawTail();
				++score;
				print("score");
				food = generateFood();          //产生一个新的食物
				food.draw();
			}
			if (snake.ifTouchBody() || snake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //撞墙或撞到了自己
			if (score == gameSize * gameSize - 3) win = 1;        //有谁真的能这么有耐心吗
		}
		if (died) print("died");
		if (win) print("win");
	}
};

Food Classic_mode::generateFood() {        //用来创造食物
//	debug << "Classic_mode::generateFood()" << std::endl;
	Food f(construct_random_food(gameSize, snake));
	return f;
}

int Classic_mode::receiveCommand() {
	bool got = 0;
	int command;
	//下面这两行只是为了避免写贼长贼长的看到眼花缭乱的if语句
	std::set<int> significative = { UP,DOWN,LEFT,RIGHT,SPEEDUP,SPEEDDOWN,PAUSE };     //有效的指令集合
	std::map<int, int> opposite = { { UP,DOWN },{ DOWN,UP },{ LEFT,RIGHT },{ RIGHT,LEFT } };  //用来存储方向上的相反关系
	for (int i = 1; i <= SPEED_LV[speed]; ++i) {
		Sleep(SLEEPTIME);
		if (_kbhit()) {
			command = _getch();
			command = std::tolower(command); //一律采用小写字母判断
			if (significative.find(command) == significative.end()) continue;                           //不是有效指令按键 
			auto map_it = opposite.find(command);
			if (map_it != opposite.end() && map_it->second == (snake.begin() + 1)->direction) continue; //不能让蛇头朝反方向前进  
			if (command == SPEEDUP) {          //加速
				if (speed < 7) { ++speed; print("speed"); }
				continue;
			}
			if (command == SPEEDDOWN) {        //减速
				if (speed > 0) { --speed; print("speed"); }
				continue; 
			}
			if (command == PAUSE) {            //暂停
				while (1) if (_getch() == 32) break;
				continue;
			}
			got = 1;         //得到了真正的方向指令
			break;           //退出循环
		}
	}
	if (got) return command;
	else return -1;          //没有得到方向指令，记为-1
}

void Classic_mode::drawInterface() { //画游戏界面
	print("gameInterface");
	gotoxy(gameSize * 2 + 11, 5); std::cout << "经典模式"; //其实也就这一处有些特别...吧...
	std::cout << std::flush;
}

void Classic_mode::print(const std::string& obj) { //输出一些东西。这种定义形参的方式灵感来源于system函数
	using std::cout;
	using std::endl;
	using std::flush;
	if (obj == "helpBox") {
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
		cout << flush;
	}
	else if (obj == "gameInterface") {
		for (int i = 1; i <= gameSize + 2; ++i) {
			gotoxy(2 * i - 1, 1); cout << "■";
			gotoxy(2 * i - 1, gameSize + 2); cout << "■";
		}
		for (int i = 2; i <= gameSize + 1; ++i) {
			gotoxy(1, i); cout << "■";
			gotoxy(2 * gameSize + 3, i); cout << "■";
		}
		gotoxy(gameSize * 2 + 11, 7); cout << "SCORE:" << score;
		gotoxy(gameSize * 2 + 11, 9); cout << "SPEED:" << speed + 1;
		gotoxy(gameSize * 2 + 11, 11); cout << "W:向上移动";
		gotoxy(gameSize * 2 + 11, 12); cout << "S:向下移动";
		gotoxy(gameSize * 2 + 11, 13); cout << "A:向左移动";
		gotoxy(gameSize * 2 + 11, 14); cout << "D:向右移动";
		gotoxy(gameSize * 2 + 11, 15); cout << "J:加速";
		gotoxy(gameSize * 2 + 11, 16); cout << "K:减速";
		gotoxy(gameSize * 2 + 11, 17); cout << "[空格] 暂停";
		gotoxy(gameSize * 2 + 9, 18); cout << "Produced By GKWuxi";
		gotoxy(gameSize * 2 + 9, 19); cout << "无锡市大桥实验学校";
		cout << flush;
	}
	else if (obj == "score") { gotoxy(gameSize * 2 + 17, 7); cout << score << flush; }
	else if (obj == "speed") { gotoxy(gameSize * 2 + 17, 9); cout << speed + 1 << flush; }  //为了避免给用户带来困惑，速度等级由1开始计
	else if (obj == "died") { gotoxy(15, 8); cout << "SNAKE DIED!" << flush; }
	else if (obj == "win") { gotoxy(17, 8); cout << "YOU WIN!" << flush; }
}

void Classic_mode::drawHelp() {
	using std::cout;
	using std::flush;
	gotoxy(14, 6); cout << "经典模式";
	gotoxy(11, 8); cout << "W:向上  S:向下";
	gotoxy(11, 9); cout << "A:向左  D:向右";
	gotoxy(11, 10); cout << "J:加速  K:减速";
	gotoxy(7, 12); cout << "让蛇尽可能吃到更多食物";
	gotoxy(9, 13); cout << "但不能撞到墙或自己！";
	gotoxy(18, 16); cout << "[空格] 开始";
	cout << flush;
}

#endif /* CLASSIC_MODE_H */