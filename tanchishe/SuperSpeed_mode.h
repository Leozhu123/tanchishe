#pragma once
#ifndef SUPERSPEED_MODE_H
#define SUPERSPEED_MODE_H

#include "Classic_mode.h"
#include "SuperSpeed_snake.h"
#include <vector>

//extern std::ofstream debug;

extern const int UP, DOWN, LEFT, RIGHT;     //定义在"Snake.h"中
extern const int SPEED_LV[8];               //定义在"Classic_mode.h"中
extern const int PAUSE;                     //定义在"Classic_mode.h"中

void gotoxy(int, int);

class SuperSpeed_mode : public Classic_mode {
private:
	SuperSpeed_snake ssnake;
	void drawInterface();                   //覆盖了基类Classic_mode::drawInterface()函数
	const int nFood = 5;                    //用来存储所需食物的个数
	std::vector<Food> foods;
	void drawHelp();
	int receiveCommand();                   //覆盖了基类Classic_mode::receiveCommand()函数
protected:
	std::vector<Food> generateFood();       //重载了基类Classic_mode::generateFood()函数
public:
	//子类的构造函数，先调用基类的构造函数构造基类
	SuperSpeed_mode(int gs = 20) : Classic_mode(gs, 7), ssnake(5, gameSize), foods(generateFood()) { } //{ debug << "SuperSpeed_mode constructor" << std::endl; }
	void help() {
//		debug << "SuperSpeed_mode::help()" << std::endl;
		system("cls");
		print("helpBox");
		drawHelp();         /* SuperSpeed_mode 画出帮助信息界面 */
	}
	void init() {
//		debug << "SuperSpeed_mode::init()" << std::endl;
		system("cls");
		drawInterface();    /* SuperSpeed_mode 画游戏界面 */
		for (auto& fd : foods) fd.draw();   //依次画出食物
		ssnake.draw();                      //画蛇
		print("speed");
	}
	void run() {
		while (!(died || win)) {                                          //既没死也没赢
			int command = receiveCommand();                /* SuperSpeed_mode::receiveCommand() */
			if (command != -1) ssnake.setDirection(command);              //设置方向
			Body old_back = ssnake.back();                                //老套路
			ssnake.cleanTail();  
			ssnake.move_fd();
			ssnake.drawHead();
			auto eaten = ssnake.ifEat(foods.begin(), foods.end());        //被吃到的食物的迭代器
			if (eaten != foods.end()) {                                   //有食物被吃到
				ssnake.restore_back(old_back); 
				ssnake.drawTail();
				++score;
				print("score");
				if (score % nFood) {                                      //只要重新产生一个食物
					*eaten = construct_random_food(gameSize, ssnake);     //构造一个食物
					eaten->draw();                                        //把它画出来
				}
				else {
					for (auto& fd : foods) if (fd != *eaten) fd.clean();  //只能擦掉没被吃的食物，否则将会把蛇头也擦掉
					foods = generateFood();                               //重新产生所有食物
					for (auto& fd : foods) fd.draw();                     //全部画出来
				}
			}
			if (ssnake.ifTouchBody() || ssnake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //撞墙或撞到了自己
			if (score == 50) win = 1;
		}
		if (died) print("died");
		if (win) print("win");
	}
};

void SuperSpeed_mode::drawInterface() {
	print("gameInterface");
	gotoxy(gameSize * 2 + 11, 5);
	std::cout << "极速模式";
	gotoxy(gameSize * 2 + 17, 15); std::cout << "（不可用）";
	gotoxy(gameSize * 2 + 17, 16); std::cout << "（不可用）";
	std::cout << std::flush;
}

std::vector<Food> SuperSpeed_mode::generateFood() {           //产生一vector的Food
//	debug << "SuperSpeed_mode::generateFood()" << std::endl;
	std::vector<Food> f;
	for (int i = 0; i != nFood; ++i) 
		f.push_back(construct_random_food(gameSize, ssnake));
	return f;
}

void SuperSpeed_mode::drawHelp() {
	using std::cout;
	using std::flush;
	gotoxy(14, 6); cout << "极速模式";
	gotoxy(11, 8); cout << "W:向上  S:向下";
	gotoxy(11, 9); cout << "A:向左  D:向右";
	gotoxy(8, 11); cout << "蛇会以顶级的速度前进!";
	gotoxy(10, 12); cout << "而且食物也会变多";
	gotoxy(10, 13); cout << "努力得到50分吧!";
	gotoxy(18, 16); cout << "[空格] 开始";
	cout << flush;
}

int SuperSpeed_mode::receiveCommand() {                             //不再处理加速和减速的指令
	int command;
	bool got = 0;
	std::set<int> significative = { UP,DOWN,LEFT,RIGHT,PAUSE };     //有效的指令集合
	std::map<int, int> opposite = { { UP,DOWN },{ DOWN,UP },{ LEFT,RIGHT },{ RIGHT,LEFT } };  //用来存储方向上的相反关系
	for (int i = 1; i <= SPEED_LV[speed]; ++i) {
		Sleep(SLEEPTIME);
		if (_kbhit()) {
			command = _getch();
			command = std::tolower(command);   //一律采用小写字母判断
			if (significative.find(command) == significative.end()) continue;                            //不是有效指令按键 
			auto map_it = opposite.find(command);
			if (map_it != opposite.end() && map_it->second == (ssnake.begin() + 1)->direction) continue; //不能让蛇头朝反方向前进  
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

#endif /*SUPERSPEED_MODE_H*/