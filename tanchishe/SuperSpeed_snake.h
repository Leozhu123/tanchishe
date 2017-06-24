#pragma once
#ifndef SUPERSPEED_SNAKE_H
#define SUPERSPEED_SNAKE_H

#include "Snake.h"
#include "Food.h"
#include <vector>

class SuperSpeed_snake : public Snake {
public:
	SuperSpeed_snake(int length = 5, int gameSize = 20) : Snake(length, gameSize) { }
	std::vector<Food>::iterator ifEat(std::vector<Food>::iterator begin, 
		                       std::vector<Food>::iterator end) {    //重载了基类Snake::ifEat()函数，并且使用标准库规范
		while (begin != end) {
			if (begin->x == front().x && begin->y == front().y) break;
			++begin;
		}
		return begin;                                                //返回一个迭代器，指向被吃到的食物。若没有吃到，返回end
	}
};

#endif /*SUPERSPEED_SNAKE_H*/