#pragma once
#ifndef CLASSIC_MODE_H
#define CLASSIC_MODE_H

//#include <fstream>
//#include <cassert>
//extern std::ofstream debug("debug_out.txt");

#include <Windows.h>
#include <conio.h>
#include "Snake.h"
#include <random>
#include <set>
#include <cctype>
#include <map>
#include <string>
#include <iostream>
#include "Food.h"

#define SLEEPTIME 10

extern const int UP, DOWN, LEFT, RIGHT; //��Щ����������"Snake.h"��
extern const int SPEED_LV[8] = { 300,200,100,50,25,15,10,5 }; //����8���ٶ� 
extern const int PAUSE = 32;        //�ո��ASCII��Ϊ32 
const int SPEEDUP = 'j';
const int SPEEDDOWN = 'k';

void gotoxy(int, int);       //ȫ�ֺ������ƶ���꣬��"Դ.cpp"�ж���

class Classic_mode {         //����ģʽ
protected:
	const int gameSize;
private:
	Snake snake;             //һ����ͨ����
	void drawInterface();    //������ģʽ�Ľ���
	Food food;               //�洢ʳ������
	Food generateFood();     //����ʳ��
	void drawHelp();
protected:
	int score = 0;
	int speed = 2;           //��ǰ�ٶ��ڵڼ���
	bool died = 0, win = 0;
	virtual int receiveCommand();    //����ָ��
	virtual void print(const std::string&); //ר�������������ĺ���
public:
	//����Classic_mode�Ķ����ʱ��ֱ�Ӱ�snake�����ˣ�˳�������ʳ��������������ڳ�ʼֵ
	Classic_mode(int gs = 20) : gameSize(gs), snake(3, gameSize), food(generateFood()) { } //{ debug << "Classic_mode constructor" << std::endl; }
	Classic_mode(int gs, int spd) : Classic_mode(gs) { speed = spd; } //ί�й��캯��
	virtual void help() {
//		debug << "Classic_mode::help()" << std::endl;
		system("cls");
		print("helpBox");       //������ʾ��
		drawHelp();         /* Classic_mode ����������Ϣ���� */
	}
	virtual void init() {
		//�Ѹû����Ȼ���
//		debug << "Classic_mode::init()" << std::endl;
		system("cls");
		drawInterface();    /* Classic_mode ����Ϸ���� */
		food.draw();
		snake.draw();
		print("speed");
	}
	virtual void run() {
		while (!(died || win)) {                //��û��ҲûӮ
			int command = receiveCommand();     //����ָ��
			if (command != -1) snake.setDirection(command);  //����ǰ������
			Body old_back = snake.back();       //��¼��ǰ����β���ڳ�ʳ���ʱ����Ҫ�����ָ�
			snake.cleanTail();
			snake.move_fd();                    //��ǰ�ƶ�һ��
			snake.drawHead();                   //���µ���ͷ�͵�һ��������
			if (snake.ifEat(food.x, food.y)) {  //�Ե���ʳ��
				snake.restore_back(old_back);   //�ָ�֮ǰɾ������β
				snake.drawTail();
				++score;
				print("score");
				food = generateFood();          //����һ���µ�ʳ��
				food.draw();
			}
			if (snake.ifTouchBody() || snake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //ײǽ��ײ�����Լ�
			if (score == gameSize * gameSize - 3) win = 1;        //��˭�������ô��������
		}
		if (died) print("died");
		if (win) print("win");
	}
};

Food Classic_mode::generateFood() {        //��������ʳ��
//	debug << "Classic_mode::generateFood()" << std::endl;
	Food f(construct_random_food(gameSize, snake));
	return f;
}

int Classic_mode::receiveCommand() {
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
			if (command == SPEEDUP) {          //����
				if (speed < 7) { ++speed; print("speed"); }
				continue;
			}
			if (command == SPEEDDOWN) {        //����
				if (speed > 0) { --speed; print("speed"); }
				continue; 
			}
			if (command == PAUSE) {            //��ͣ
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
	print("gameInterface");
	gotoxy(gameSize * 2 + 11, 5); std::cout << "����ģʽ"; //��ʵҲ����һ����Щ�ر�...��...
	std::cout << std::flush;
}

void Classic_mode::print(const std::string& obj) { //���һЩ���������ֶ����βεķ�ʽ�����Դ��system����
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
	else if (obj == "gameInterface") {
		for (int i = 1; i <= gameSize + 2; ++i) {
			gotoxy(2 * i - 1, 1); cout << "��";
			gotoxy(2 * i - 1, gameSize + 2); cout << "��";
		}
		for (int i = 2; i <= gameSize + 1; ++i) {
			gotoxy(1, i); cout << "��";
			gotoxy(2 * gameSize + 3, i); cout << "��";
		}
		gotoxy(gameSize * 2 + 11, 7); cout << "SCORE:" << score;
		gotoxy(gameSize * 2 + 11, 9); cout << "SPEED:" << speed + 1;
		gotoxy(gameSize * 2 + 11, 11); cout << "W:�����ƶ�";
		gotoxy(gameSize * 2 + 11, 12); cout << "S:�����ƶ�";
		gotoxy(gameSize * 2 + 11, 13); cout << "A:�����ƶ�";
		gotoxy(gameSize * 2 + 11, 14); cout << "D:�����ƶ�";
		gotoxy(gameSize * 2 + 11, 15); cout << "J:����";
		gotoxy(gameSize * 2 + 11, 16); cout << "K:����";
		gotoxy(gameSize * 2 + 11, 17); cout << "[�ո�] ��ͣ";
		gotoxy(gameSize * 2 + 9, 18); cout << "Produced By GKWuxi";
		gotoxy(gameSize * 2 + 9, 19); cout << "�����д���ʵ��ѧУ";
		cout << flush;
	}
	else if (obj == "score") { gotoxy(gameSize * 2 + 17, 7); cout << score << flush; }
	else if (obj == "speed") { gotoxy(gameSize * 2 + 17, 9); cout << speed + 1 << flush; }  //Ϊ�˱�����û����������ٶȵȼ���1��ʼ��
	else if (obj == "died") { gotoxy(15, 8); cout << "SNAKE DIED!" << flush; }
	else if (obj == "win") { gotoxy(17, 8); cout << "YOU WIN!" << flush; }
}

void Classic_mode::drawHelp() {
	using std::cout;
	using std::flush;
	gotoxy(14, 6); cout << "����ģʽ";
	gotoxy(11, 8); cout << "W:����  S:����";
	gotoxy(11, 9); cout << "A:����  D:����";
	gotoxy(11, 10); cout << "J:����  K:����";
	gotoxy(7, 12); cout << "���߾����ܳԵ�����ʳ��";
	gotoxy(9, 13); cout << "������ײ��ǽ���Լ���";
	gotoxy(18, 16); cout << "[�ո�] ��ʼ";
	cout << flush;
}

#endif /* CLASSIC_MODE_H */