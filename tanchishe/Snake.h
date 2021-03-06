#pragma once
#ifndef SNAKE_H
#define SNAKE_H

//extern std::ofstream debug;

#include <deque>
#include <random>
#include <iostream>

//这几个常量需要与"Classic_mode.h"共享
extern const int UP = 'w';
extern const int DOWN = 's';
extern const int LEFT = 'a';
extern const int RIGHT = 'd';

void gotoxy(int, int);      //全局函数，定义在"源.cpp"中

struct Body {        //一节蛇身
	int x, y, direction;
	Body() = default;
	Body(int ix, int iy, int id) : x(ix), y(iy), direction(id) { }
};

class Snake {
protected:
	std::deque<Body> body;
	Body& head() { return body.front(); }
public:
	Snake(int length = 3, int gameSize = 20) {                //构造一条长度为length的蛇，蛇的位置随机处理。同时这也是默认构造函数。
//		debug << "Snake constructor" << std::endl;
		std::random_device rd;                                                       //定义一个随机数种子
		std::uniform_int_distribution<int> u(1 + length, gameSize + 2 - length);     //保证蛇不会撞墙
		std::uniform_int_distribution<int> d(0, 3);                                  //随机选择一个方向
		std::default_random_engine e(rd());
		int ds[4] = { UP,DOWN,LEFT,RIGHT };
		body.emplace_back(u(e) * 2 - 1, u(e), ds[d(e)]);                             //构造一个蛇头，保证蛇的x坐标是奇数
		for (int i = 1; i != length; ++i) {                                          //继续构造length-1节蛇身
			switch (body.front().direction) {
			case UP:
				body.emplace_back(head().x, body.back().y + 1, body.front().direction); break;
			case DOWN:
				body.emplace_back(head().x, body.back().y - 1, body.front().direction); break;
			case LEFT:
				body.emplace_back(body.back().x + 2, head().y, body.front().direction); break;
			case RIGHT:
				body.emplace_back(body.back().x - 2, head().y, body.front().direction); break;
			}
		}
	}
	//返回蛇身的迭代器，这里的迭代器都是const_iterator
	auto begin()->std::deque<Body>::const_iterator const { return body.cbegin(); }
	auto end()->std::deque<Body>::const_iterator const { return body.cend(); }
	//返回蛇头和蛇尾：这里前后的几个函数都是使用标准库规范的
	const Body& front() const { return body.front(); }
	const Body& back() const { return body.back(); }
	void setDirection(int d) { head().direction = d; }         //设置蛇的前进方向
	bool ifEat(int food_x, int food_y) {                       //判断是否吃到了食物
		return front().x == food_x && front().y == food_y;
	}
	bool ifTouchWall(int lwall, int rwall, int uwall, int dwall) { //判断是否撞墙
		return front().x == lwall || front().x == rwall || front().y == uwall || front().y == dwall;
	}
	bool ifTouchBody() {                                       //判断是否撞到了自己
		auto it = begin();
		while (++it != end())
			if (it->x == front().x && it->y == front().y) return 1;
		return 0;
	}
	void move_fd() {                       //蛇前进
		body.pop_back();                   //先删除旧的蛇尾
        Body new_head = head();            //在旧的蛇头的基础上改变 
		switch (front().direction) {
		case UP: --new_head.y; break;
		case DOWN: ++new_head.y; break;
		case LEFT: ----new_head.x; break;  //据说这样写是合乎语法的
		case RIGHT: ++++new_head.x; break; //在侯捷的《STL源码剖析》里出现了3个operator++连写，真是石乐志了
		}
		body.push_front(new_head);         //添加新的蛇头
	}
	void restore_back(Body old_back) { body.push_back(old_back); }  //在吃食物的时候需要恢复被删除的蛇尾
	void draw();
	void drawHead();
	void drawTail();
	void cleanTail();
};

void Snake::draw() {                  //画全蛇
	auto it = begin();
	gotoxy(it->x, it->y);
	std::cout << "●" << std::flush;
	while (++it != end()) {
		gotoxy(it->x, it->y);
		std::cout << "◎" << std::flush;
	}
}

void Snake::drawHead() {              //除了画新的蛇头之外，还要把原来的蛇头“降级”为普通蛇身
	gotoxy(front().x, front().y); std::cout << "●" << std::flush;
	gotoxy((begin() + 1)->x, (begin() + 1)->y); std::cout << "◎" << std::flush;
}

void Snake::drawTail() {
	gotoxy(back().x, back().y); std::cout << "◎" << std::flush;
}

void Snake::cleanTail() {
	gotoxy(back().x, back().y); std::cout << "  " << std::flush;
}

#endif /* SNAKE_H */