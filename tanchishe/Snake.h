#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <random>
#include <iostream>

//�⼸��������Ҫ��"Classic_mode.h"����
extern const int UP = 'w';
extern const int DOWN = 's';
extern const int LEFT = 'a';
extern const int RIGHT = 'd';

void gotoxy(int, int);      //ȫ�ֺ�����������"Դ.cpp"��

struct Body {        //һ������
	int x, y, direction;
	Body() = default;
	Body(int ix, int iy, int id) : x(ix), y(iy), direction(id) { }
};

class Snake {
protected:
	std::deque<Body> body;
	Body& head() { return body.front(); }
public:
	Snake(int length = 3, int gameSize = 20) {                //����һ������Ϊlength���ߣ��ߵ�λ���������ͬʱ��Ҳ��Ĭ�Ϲ��캯����
		std::random_device rd;                                                       //����һ�����������
		std::uniform_int_distribution<int> u(1 + length, gameSize + 2 - length);     //��֤�߲���ײǽ
		std::uniform_int_distribution<int> d(0, 3);                                  //���ѡ��һ������
		std::default_random_engine e(rd());
		int ds[4] = { UP,DOWN,LEFT,RIGHT };
		body.emplace_back(u(e) * 2 - 1, u(e), ds[d(e)]);                             //����һ����ͷ����֤�ߵ�x����������
		for (int i = 1; i != length; ++i) {                                          //��������length-1������
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
	//��������ĵ�����������ĵ���������const_iterator
	auto begin()->std::deque<Body>::const_iterator const { return body.cbegin(); }
	auto end()->std::deque<Body>::const_iterator const { return body.cend(); }
	//������ͷ����β������ǰ��ļ�����������ʹ�ñ�׼��淶��
	const Body& front() const { return body.front(); }
	const Body& back() const { return body.back(); }
	void setDirection(int d) { head().direction = d; }         //�����ߵ�ǰ������
	bool ifEat(int food_x, int food_y) {                       //�ж��Ƿ�Ե���ʳ��
		return front().x == food_x && front().y == food_y;
	}
	bool ifTouchWall(int lwall, int rwall, int uwall, int dwall) { //�ж��Ƿ�ײǽ
		return front().x == lwall || front().x == rwall || front().y == uwall || front().y == dwall;
	}
	bool ifTouchBody() {                                       //�ж��Ƿ�ײ�����Լ�
		auto it = begin();
		while (++it != end())
			if (it->x == front().x && it->y == front().y) return 1;
		return 0;
	}
	void move_fd() {                       //��ǰ��
		body.pop_back();                   //��ɾ���ɵ���β
        Body new_head = head();            //�ھɵ���ͷ�Ļ����ϸı� 
		switch (front().direction) {
		case UP: --new_head.y; break;
		case DOWN: ++new_head.y; break;
		case LEFT: ----new_head.x; break;  //��˵����д�ǺϺ��﷨��
		case RIGHT: ++++new_head.x; break; //�ں�ݵġ�STLԴ���������������3��operator++��д������ʯ��־��
		}
		body.push_front(new_head);         //����µ���ͷ
	}
	void restore_back(Body old_back) { body.push_back(old_back); }  //�ڳ�ʳ���ʱ����Ҫ�ָ���ɾ������β
	void draw();
	void drawHead();
	void drawTail();
	void cleanTail();
};

void Snake::draw() {                  //��ȫ��
	auto it = begin();
	gotoxy(it->x, it->y);
	std::cout << "��" << std::flush;
	while (++it != end()) {
		gotoxy(it->x, it->y);
		std::cout << "��" << std::flush;
	}
}

void Snake::drawHead() {              //���˻��µ���ͷ֮�⣬��Ҫ��ԭ������ͷ��������Ϊ��ͨ����
	gotoxy(front().x, front().y); std::cout << "��" << std::flush;
	gotoxy((begin() + 1)->x, (begin() + 1)->y); std::cout << "��" << std::flush;
}

void Snake::drawTail() {
	gotoxy(back().x, back().y); std::cout << "��" << std::flush;
}

void Snake::cleanTail() {
	gotoxy(back().x, back().y); std::cout << "  " << std::flush;
}

#endif /* SNAKE_H */