#pragma once
#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <random>
#include "Snake.h"

//extern std::ofstream debug;

void gotoxy(int, int);      //全局函数，移动光标，定义在"源.cpp"中

struct Food { 
	int x, y;               //食物坐标
	Food() = default;
	Food(int ix, int iy) : x(ix), y(iy) { }
	Food(Food& f) : x(f.x), y(f.y) { }       //拷贝构造
	void draw() {                            //画食物
		gotoxy(x, y);
		std::cout << "★" << std::flush; 
	}
	void clean() {                           //擦除食物
		gotoxy(x, y);
		std::cout << "  " << std::flush;
	}
	bool operator!=(Food& f) {               //判断两个食物不相等
		return f.x != x && f.y != y;
	}
};

Food construct_random_food(int gameSize, Snake& snake) {       //用来创造食物
//	debug << "construct_random_food(int, Snake&)" << std::endl;
	static std::random_device rd;
	static std::uniform_int_distribution<> u(2, gameSize + 1); //用gameSize而不是magic number
	static std::default_random_engine e(rd());
	Food f;
	while (1) {
		f.x = u(e) * 2 - 1;
		f.y = u(e);
		bool coincide = 0;
		for (const auto bd : snake) //据说只要有合适的begin()和end()就能用range for。反正它返回的就是个deque<Body>::const_iterator
			if (bd.x == f.x && bd.y == f.y) {  //判断是否与蛇重合了
				coincide = 1; break;
			}
		if (!coincide) break;       //没重合就大功告成了
	}
	return f;
}

#endif /*FOOD_H*/