#pragma once
#ifndef SNAKE_H
#define SNAKE_H

#include <deque>
#include <random>
#include <iostream>
extern int speed;

//�⼸��������Ҫ��"Classic_mode.h"����
extern const int UP = 'w';
extern const int DOWN = 's';
extern const int LEFT = 'a';
extern const int RIGHT = 'd';

struct Body {        //һ������
	int x, y, direction;
	Body() = default;
	Body(int ix, int iy, int id) : x(ix), y(iy), direction(id) { }
};

class Snake {
protected:
	//�������в���д��std::deque<Body> body(3);
	//��ΪC++�涨���ڶ���Ķ�����ֱ����Բ���Ÿ��������ڳ�ʼֵ
	//������ô��Ҳ��һ����̫�õ���ƣ���׼��˳������Ӧ�þ���ʹ��push��emplace�����Ԫ��
	//�����ܿ�����������������������Сûʲô����
	std::deque<Body> body;
	Body& head() { return body.front(); }
public:
	Snake(int length = 3, int gameSize=20) { 
		//����һ������Ϊlength���ߣ��ߵ�λ���������ͬʱ��Ҳ��Ĭ�Ϲ��캯����
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
		return head().x == food_x && head().y == food_y;
	}
	bool ifTouchWall(int lwall, int rwall, int uwall, int dwall) { //�ж��Ƿ�ײǽ
		return head().x == lwall || head().x == rwall || head().y == uwall || head().y == dwall;
	}
	bool ifTouchBody() {                                       //�ж��Ƿ�ײ�����Լ�
		auto it = body.cbegin();
		while (++it != body.end())
			if (it->x == head().x && it->y == head().y) return 1;
		return 0;
	}
	void move_fd() {                       //��ǰ��
		body.pop_back();                   //��ɾ���ɵ���β
        Body new_head = head();            //�ھɵ���ͷ�Ļ����ϸı� 
		switch (head().direction) {
		case UP: --new_head.y; break;
		case DOWN: ++new_head.y; break;
		case LEFT: ----new_head.x; break;  //��˵����д�ǺϺ��﷨��
		case RIGHT: ++++new_head.x; break; //�ں�ݵġ�STLԴ���������������3��operator++��д������ʯ��־��
		}
		body.push_front(new_head);         //����µ���ͷ
	}
	void restore_back(Body old_back) { body.push_back(old_back); }  //�ڳ�ʳ���ʱ����Ҫ�ָ���ɾ������β
};

#endif /* SNAKE_H */