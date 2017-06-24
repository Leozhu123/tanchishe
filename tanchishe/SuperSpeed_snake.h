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
		                       std::vector<Food>::iterator end) {    //�����˻���Snake::ifEat()����������ʹ�ñ�׼��淶
		while (begin != end) {
			if (begin->x == front().x && begin->y == front().y) break;
			++begin;
		}
		return begin;                                                //����һ����������ָ�򱻳Ե���ʳ���û�гԵ�������end
	}
};

#endif /*SUPERSPEED_SNAKE_H*/