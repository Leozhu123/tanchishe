#pragma once
#ifndef CLASSIC_MODE_H
#define CLASSIC_MODE_H

#include "Snake.h"
#include <random>
#include <set>
#include <conio.h>
#include <Windows.h>
#include <cctype>
#include <map>
#include <string>
#include <iostream>

const int SLEEPTIME = 10;
extern int gameSize;
const int SPEED_LV[8] = { 300,200,100,50,25,15,10,5 };
extern const int UP, DOWN, LEFT, RIGHT; //这些常量定义在"Snake.h"中
const int PAUSE = 32;        //空格的ASCII码为32 
const int SPEEDUP = 'j';
const int SPEEDDOWN = 'k';

void gotoxy(int, int);       //全局函数，移动光标，在"源.cpp"中定义

class Classic_mode {         //经典模式
private:
	Snake snake;             //一条普通的蛇
	void drawInterface();    //画经典模式的界面
protected:
	int score = 0;
	int speed = 2;           //当前速度在第几档
	bool died = 0, win = 0;
	int foodX, foodY;        //存储食物坐标
	void generateFood();     //产生食物
	int receiveCommand();    //接收指令
	void draw(const std::string& obj); //专门用来画东西的函数
public:
	//定义Classic_mode的对象的时候，直接把snake构造了，顺便再造个食物，其它变量靠类内初始值
	Classic_mode() : snake(3, gameSize) { generateFood(); }
	void help() {
		system("cls");
		draw("helpBox");       //画个提示框
		draw("classicHelp");   /* Classic_mode 画出帮助信息界面 */
	}
	void init() {
		//把该画的先画好
		system("cls");
		drawInterface();    /* Classic_mode 画游戏界面 */
		draw("food");
		draw("snake");
		draw("speed");
	}
	void run() {
		while (!(died || win)) {              //既没死也没赢
			int command = receiveCommand();   //接收指令
			if (command != -1) snake.setDirection(command);  //设置前进方向
			Body old_back = snake.back();     //记录当前的蛇尾。在吃食物的时候需要把它恢复
			draw("cleanTail");
			snake.move_fd();                  //向前移动一步
			draw("newHead");                  //把新的蛇头和第一节蛇身画上
			if (snake.ifEat(foodX, foodY)) {  //吃到了食物
				snake.restore_back(old_back); //恢复之前删除的蛇尾
				draw("snakeTail");
				++score;
				draw("score");
			//	draw("speed");
				generateFood();               //产生一个新的食物
				draw("food");
			}
			if (snake.ifTouchBody() || snake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //撞墙或撞到了自己
			if (score == gameSize*gameSize-3) win = 1;        //有谁真的能这么有耐心吗
			if (died) draw("died");
			if (win) draw("win");
		}
	}
};

void Classic_mode::generateFood() {        //用来创造食物
	std::random_device rd;
	std::uniform_int_distribution<> u(2, gameSize + 1); //用gameSize而不是magic number
	std::default_random_engine e(rd());
	while (1) {
		foodX = u(e) * 2 - 1;
		foodY = u(e);
		bool coincide = 0;
		for (const auto bd : snake) //据说只要有合适的begin()和end()就能用range for。反正它返回的就是个deque<Body>::const_iterator
			if (bd.x == foodX && bd.y == foodY) {  //判断是否与蛇重合了
				coincide = 1; break;
			}
		if (!coincide) break;       //没重合就大功告成了
	}
}

int Classic_mode::receiveCommand() {
 //共有4档速度 
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
			if (command == SPEEDUP) 
			{
				++speed; 
				if (speed > 7) speed = 7;
				draw("speed");
				continue;}      //加速
			if (command == SPEEDDOWN) 
			{ 
				--speed; 
			    if (speed < 0) speed = 0; 
				draw("speed");
				continue; } //减速
			if (command == PAUSE) {                                           //暂停
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
	draw("gameInterface");
	gotoxy(gameSize * 2 + 11, 5); std::cout << "经典模式"; //其实也就这一处有些特别...吧...
	std::cout << std::flush;
}

void Classic_mode::draw(const std::string& obj) { //万能画笔。这种定义形参的方式灵感来源于system函数
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
	else if (obj == "classicHelp") {
		gotoxy(14, 6); cout << "经典模式";
		gotoxy(7, 12); cout << "让蛇尽可能吃到更多食物";
		gotoxy(9, 13); cout << "但不能撞到墙或自己！";
		gotoxy(11, 8); cout << "W:向上  S:向下";
		gotoxy(11, 9); cout << "A:向左  D:向右";
		gotoxy(11, 10); cout << "J:加速  K:减速";
		gotoxy(18, 16); cout << "[空格] 开始 z+数字+Enter 指定大小(10-99)";
		cout << flush;
	}
	else if (obj == "gameInterface") {
		for (int i = 1; i <= gameSize+ 2; i++) {
			gotoxy(2 * i - 1, 1); cout << "■";
			gotoxy(2 * i - 1, gameSize + 2); cout << "■";
		}
		for (int i = 2; i <= gameSize+ 1; i++) {
			gotoxy(1, i); cout << "■";
			gotoxy(2*gameSize+3, i); cout << "■";
		}
		gotoxy(gameSize * 2+11, 7); cout << "SCORE:"<<score;
		gotoxy(gameSize * 2 + 11, 9); cout << "SPEED:" << speed+1;
		gotoxy(gameSize * 2 + 11, 11); cout << "W:向上移动";
		gotoxy(gameSize * 2 + 11, 12); cout << "S:向下移动";
		gotoxy(gameSize * 2 + 11, 13);	cout << "A:向左移动";
		gotoxy(gameSize * 2 + 11, 14); cout << "D:向右移动";
		gotoxy(gameSize * 2 + 11, 15); cout << "J:加速";
		gotoxy(gameSize * 2 + 11, 16);	cout << "K:减速";
		gotoxy(gameSize * 2 + 11, 17);	cout << "[空格] 暂停";
		gotoxy(gameSize * 2 + 9, 18);	cout << "Produced By GKWuxi";
		gotoxy(gameSize * 2 + 9, 19);	cout << "无锡市大桥实验学校";
		cout << flush;
	}
	else if (obj == "food") { gotoxy(foodX, foodY); cout << "★" << flush; }
	else if (obj == "snake") {
		auto it = snake.begin();
		gotoxy(it->x, it->y); cout << "●" << flush;
		while (++it != snake.end()){
			gotoxy(it->x, it->y); cout << "◎" << flush;
	}
	}
	else if (obj == "cleanTail") { gotoxy(snake.back().x, snake.back().y); cout << "  " << flush; }
	else if (obj == "newHead") {    //除了画新的蛇头之外，还要把原来的蛇头“降级”为普通蛇身
		gotoxy(snake.front().x, snake.front().y); cout << "●" << flush;
		gotoxy((snake.begin() + 1)->x, (snake.begin() + 1)->y); cout << "◎" << flush;
	}
	else if (obj == "snakeTail") { gotoxy(snake.back().x, snake.back().y); cout << "◎" << flush; }
	else if (obj == "score") { gotoxy(gameSize * 2 + 17, 7); cout << score << flush; }
	else if (obj == "speed") { gotoxy(gameSize * 2 + 11, 9); cout << "SPEED:    "; gotoxy(gameSize * 2 + 17, 9); cout << speed+1 << flush; }
	else if (obj == "died") { gotoxy(15, 8); cout << "SNAKE DIED!" << flush; }
	else if (obj == "win") { gotoxy(17, 8); cout << "YOU WIN!" << flush; }
}

#endif /* CLASSIC_MODE_H */