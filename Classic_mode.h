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

#define SLEEPTIME 50

extern const int UP, DOWN, LEFT, RIGHT; //��Щ����������"Snake.h"��
const int PAUSE = 32;        //�ո��ASCII��Ϊ32 
const int SPEEDUP = 'j';
const int SPEEDDOWN = 'k';

void gotoxy(int, int);       //ȫ�ֺ������ƶ���꣬��"Դ.cpp"�ж���

class Classic_mode {         //����ģʽ
private:
	Snake snake;             //һ����ͨ����
	void drawInterface();    //������ģʽ�Ľ���
protected:
	const int gameSize = 20; //��Ϸ�����С
	int score = 0;
	int speed = 1;           //��ǰ�ٶ��ڵڼ���
	bool died = 0, win = 0;
	int foodX, foodY;        //�洢ʳ������
	void generateFood();     //����ʳ��
	int receiveCommand();    //����ָ��
	void draw(const std::string& obj); //ר�������������ĺ���
public:
	//����Classic_mode�Ķ����ʱ��ֱ�Ӱ�snake�����ˣ�˳�������ʳ��������������ڳ�ʼֵ
	Classic_mode() : snake(3, 20) { generateFood(); }
	void help() {
		system("cls");
		draw("helpBox");       //������ʾ��
		draw("classicHelp");   /* Classic_mode ����������Ϣ���� */
	}
	void init() {
		//�Ѹû����Ȼ���
		system("cls");
		drawInterface();    /* Classic_mode ����Ϸ���� */
		draw("food");
		draw("snake");
	}
	void run() {
		while (!(died || win)) {              //��û��ҲûӮ
			int command = receiveCommand();   //����ָ��
			if (command != -1) snake.setDirection(command);  //����ǰ������
			Body old_back = snake.back();     //��¼��ǰ����β���ڳ�ʳ���ʱ����Ҫ�����ָ�
			draw("cleanTail");
			snake.move_fd();                  //��ǰ�ƶ�һ��
			draw("newHead");                  //���µ���ͷ�͵�һ��������
			if (snake.ifEat(foodX, foodY)) {  //�Ե���ʳ��
				snake.restore_back(old_back); //�ָ�֮ǰɾ������β
				draw("snakeTail");
				++score;
				draw("score");
				generateFood();               //����һ���µ�ʳ��
				draw("food");
			}
			if (snake.ifTouchBody() || snake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //ײǽ��ײ�����Լ�
			if (score == 397) win = 1;        //��˭�������ô��������
			if (died) draw("died");
			if (win) draw("win");
		}
	}
};

void Classic_mode::generateFood() {        //��������ʳ��
	std::random_device rd;
	std::uniform_int_distribution<> u(2, gameSize + 1); //��gameSize������magic number
	std::default_random_engine e(rd());
	while (1) {
		foodX = u(e) * 2 - 1;
		foodY = u(e);
		bool coincide = 0;
		for (const auto bd : snake) //��˵ֻҪ�к��ʵ�begin()��end()������range for�����������صľ��Ǹ�deque<Body>::const_iterator
			if (bd.x == foodX && bd.y == foodY) {  //�ж��Ƿ������غ���
				coincide = 1; break;
			}
		if (!coincide) break;       //û�غϾʹ󹦸����
	}
}

int Classic_mode::receiveCommand() {
	const int SPEED_LV[6] = { 60,40,100,15,5,4}; //����4���ٶ� 
	bool got = 0;
	int command;
	//����������ֻ��Ϊ�˱���д���������Ŀ����ۻ����ҵ�if���
	std::set<int> significative = { UP,DOWN,LEFT,RIGHT,SPEEDUP,SPEEDDOWN,PAUSE };     //��Ч��ָ���
	std::map<int, int> opposite = { { UP,DOWN },{ DOWN,UP },{ LEFT,RIGHT },{ RIGHT,LEFT } };  //�����洢�����ϵ��෴��ϵ
	for (int i = 1; i <= SPEED_LV[speed]; ++i) {
		Sleep(SLEEPTIME);
		if (_kbhit()) {
			command = _getch();
			command = std::tolower(command); //һ�ɲ���Сд��ĸ�ж�
			if (significative.find(command) == significative.end()) continue;                           //������Чָ��� 
			auto map_it = opposite.find(command);
			if (map_it != opposite.end() && map_it->second == (snake.begin() + 1)->direction) continue; //��������ͷ��������ǰ��  
			if (command == SPEEDUP && speed < 5) { ++speed; continue; }       //����
			if (command == SPEEDDOWN && speed > 0) { --speed; continue; }     //����
			if (command == PAUSE) {                                           //��ͣ
				while (1) if (_getch() == 32) break;
				continue;
			}
			got = 1;         //�õ��������ķ���ָ��
			break;           //�˳�ѭ��
		}
	}
	if (got) return command;
	else return -1;          //û�еõ�����ָ���Ϊ-1
}

void Classic_mode::drawInterface() { //����Ϸ����
	draw("gameInterface");
	gotoxy(51, 5); std::cout << "����ģʽ"; //��ʵҲ����һ����Щ�ر�...��...
	std::cout << std::flush;
}

void Classic_mode::draw(const std::string& obj) { //���ܻ��ʡ����ֶ����βεķ�ʽ�����Դ��system����
	using std::cout;
	using std::endl;
	using std::flush;
	if (obj == "helpBox") {
		gotoxy(5, 5); cout << "��";
		gotoxy(29, 5); cout << "��";
		gotoxy(5, 15); cout << "��";
		gotoxy(29, 15); cout << "��";
		for (int i = 6; i <= 14; i++) {
			gotoxy(5, i); cout << "��";
			gotoxy(29, i); cout << "��";
		}
		for (int i = 7; i <= 28; i += 2) {
			gotoxy(i, 5); cout << "��";
			gotoxy(i, 15); cout << "��";
		}
		cout << flush;
	}
	else if (obj == "classicHelp") {
		gotoxy(14, 6); cout << "����ģʽ";
		gotoxy(7, 12); cout << "���߾����ܳԵ�����ʳ��";
		gotoxy(9, 13); cout << "������ײ��ǽ���Լ���";
		gotoxy(11, 8); cout << "W:����  S:����";
		gotoxy(11, 9); cout << "A:����  D:����";
		gotoxy(11, 10); cout << "J:����  K:����";
		gotoxy(18, 16); cout << "[�ո�] ��ʼ";
		cout << flush;
	}
	else if (obj == "gameInterface") {
		for (int i = 1; i <= gameSize + 2; i++) {
			gotoxy(2 * i - 1, 1); cout << "��";
			gotoxy(2 * i - 1, gameSize + 2); cout << "��";
		}
		for (int i = 2; i <= gameSize + 1; i++) {
			gotoxy(1, i); cout << "��";
			gotoxy(43, i); cout << "��";
		}
		gotoxy(51, 7); cout << "SCORE:" << score;
		gotoxy(51, 9); cout << "W:�����ƶ�";
		gotoxy(51, 10); cout << "S:�����ƶ�";
		gotoxy(51, 11);	cout << "A:�����ƶ�";
		gotoxy(51, 12); cout << "D:�����ƶ�";
		gotoxy(51, 13); cout << "J:����";
		gotoxy(51, 14);	cout << "K:����";
		gotoxy(51, 15);	cout << "[�ո�] ��ͣ";
		gotoxy(49, 17);	cout << "Produced By GKWuxi";
		gotoxy(49, 18);	cout << "�����д���ʵ��ѧУ";
		cout << flush;
	}
	else if (obj == "food") { gotoxy(foodX, foodY); cout << "��" << flush; }
	else if (obj == "snake") {
		auto it = snake.begin();
		gotoxy(it->x, it->y); cout << "��" << flush;
		while (++it != snake.end()) {
			gotoxy(it->x, it->y); cout << "��" << flush;
		}
	}
	else if (obj == "cleanTail") { gotoxy(snake.back().x, snake.back().y); cout << "  " << flush; }
	else if (obj == "newHead") {    //���˻��µ���ͷ֮�⣬��Ҫ��ԭ������ͷ��������Ϊ��ͨ����
		gotoxy(snake.front().x, snake.front().y); cout << "��" << flush;
		gotoxy((snake.begin() + 1)->x, (snake.begin() + 1)->y); cout << "��" << flush;
	}
	else if (obj == "snakeTail") { gotoxy(snake.back().x, snake.back().y); cout << "��" << flush; }
	else if (obj == "score") { gotoxy(57, 7); cout << score << flush; }
	else if (obj == "died") { gotoxy(15, 8); cout << "SNAKE DIED!" << flush; }
	else if (obj == "win") { gotoxy(17, 8); cout << "YOU WIN!" << flush; }
}

#endif /* CLASSIC_MODE_H */