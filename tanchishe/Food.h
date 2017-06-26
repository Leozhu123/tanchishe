#pragma once
#ifndef FOOD_H
#define FOOD_H

#include <iostream>
#include <random>
#include "Snake.h"

//extern std::ofstream debug;

void gotoxy(int, int);      //ȫ�ֺ������ƶ���꣬������"Դ.cpp"��

struct Food { 
	int x, y;               //ʳ������
	Food() = default;
	Food(int ix, int iy) : x(ix), y(iy) { }
	Food(Food& f) : x(f.x), y(f.y) { }       //��������
	void draw() {                            //��ʳ��
		gotoxy(x, y);
		std::cout << "��" << std::flush; 
	}
	void clean() {                           //����ʳ��
		gotoxy(x, y);
		std::cout << "  " << std::flush;
	}
	bool operator!=(Food& f) {               //�ж�����ʳ�ﲻ���
		return f.x != x && f.y != y;
	}
};

Food construct_random_food(int gameSize, Snake& snake) {       //��������ʳ��
//	debug << "construct_random_food(int, Snake&)" << std::endl;
	static std::random_device rd;
	static std::uniform_int_distribution<> u(2, gameSize + 1); //��gameSize������magic number
	static std::default_random_engine e(rd());
	Food f;
	while (1) {
		f.x = u(e) * 2 - 1;
		f.y = u(e);
		bool coincide = 0;
		for (const auto bd : snake) //��˵ֻҪ�к��ʵ�begin()��end()������range for�����������صľ��Ǹ�deque<Body>::const_iterator
			if (bd.x == f.x && bd.y == f.y) {  //�ж��Ƿ������غ���
				coincide = 1; break;
			}
		if (!coincide) break;       //û�غϾʹ󹦸����
	}
	return f;
}

#endif /*FOOD_H*/