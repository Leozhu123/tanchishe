#pragma once
#ifndef SUPERSPEED_MODE_H
#define SUPERSPEED_MODE_H

#include "Classic_mode.h"
#include "SuperSpeed_snake.h"
#include <vector>

//extern std::ofstream debug;

extern const int UP, DOWN, LEFT, RIGHT;     //������"Snake.h"��
extern const int SPEED_LV[8];               //������"Classic_mode.h"��
extern const int PAUSE;                     //������"Classic_mode.h"��

void gotoxy(int, int);

class SuperSpeed_mode : public Classic_mode {
private:
	SuperSpeed_snake ssnake;
	void drawInterface();                   //�����˻���Classic_mode::drawInterface()����
	const int nFood = 5;                    //�����洢����ʳ��ĸ���
	std::vector<Food> foods;
	void drawHelp();
	int receiveCommand();                   //�����˻���Classic_mode::receiveCommand()����
protected:
	std::vector<Food> generateFood();       //�����˻���Classic_mode::generateFood()����
public:
	//����Ĺ��캯�����ȵ��û���Ĺ��캯���������
	SuperSpeed_mode(int gs = 20) : Classic_mode(gs, 7), ssnake(5, gameSize), foods(generateFood()) { } //{ debug << "SuperSpeed_mode constructor" << std::endl; }
	void help() {
//		debug << "SuperSpeed_mode::help()" << std::endl;
		system("cls");
		print("helpBox");
		drawHelp();         /* SuperSpeed_mode ����������Ϣ���� */
	}
	void init() {
//		debug << "SuperSpeed_mode::init()" << std::endl;
		system("cls");
		drawInterface();    /* SuperSpeed_mode ����Ϸ���� */
		for (auto& fd : foods) fd.draw();   //���λ���ʳ��
		ssnake.draw();                      //����
		print("speed");
	}
	void run() {
		while (!(died || win)) {                                          //��û��ҲûӮ
			int command = receiveCommand();                /* SuperSpeed_mode::receiveCommand() */
			if (command != -1) ssnake.setDirection(command);              //���÷���
			Body old_back = ssnake.back();                                //����·
			ssnake.cleanTail();  
			ssnake.move_fd();
			ssnake.drawHead();
			auto eaten = ssnake.ifEat(foods.begin(), foods.end());        //���Ե���ʳ��ĵ�����
			if (eaten != foods.end()) {                                   //��ʳ�ﱻ�Ե�
				ssnake.restore_back(old_back); 
				ssnake.drawTail();
				++score;
				print("score");
				if (score % nFood) {                                      //ֻҪ���²���һ��ʳ��
					*eaten = construct_random_food(gameSize, ssnake);     //����һ��ʳ��
					eaten->draw();                                        //����������
				}
				else {
					for (auto& fd : foods) if (fd != *eaten) fd.clean();  //ֻ�ܲ���û���Ե�ʳ����򽫻����ͷҲ����
					foods = generateFood();                               //���²�������ʳ��
					for (auto& fd : foods) fd.draw();                     //ȫ��������
				}
			}
			if (ssnake.ifTouchBody() || ssnake.ifTouchWall(1, 2 * gameSize + 3, 1, gameSize + 2)) died = 1; //ײǽ��ײ�����Լ�
			if (score == 50) win = 1;
		}
		if (died) print("died");
		if (win) print("win");
	}
};

void SuperSpeed_mode::drawInterface() {
	print("gameInterface");
	gotoxy(gameSize * 2 + 11, 5);
	std::cout << "����ģʽ";
	gotoxy(gameSize * 2 + 17, 15); std::cout << "�������ã�";
	gotoxy(gameSize * 2 + 17, 16); std::cout << "�������ã�";
	std::cout << std::flush;
}

std::vector<Food> SuperSpeed_mode::generateFood() {           //����һvector��Food
//	debug << "SuperSpeed_mode::generateFood()" << std::endl;
	std::vector<Food> f;
	for (int i = 0; i != nFood; ++i) 
		f.push_back(construct_random_food(gameSize, ssnake));
	return f;
}

void SuperSpeed_mode::drawHelp() {
	using std::cout;
	using std::flush;
	gotoxy(14, 6); cout << "����ģʽ";
	gotoxy(11, 8); cout << "W:����  S:����";
	gotoxy(11, 9); cout << "A:����  D:����";
	gotoxy(8, 11); cout << "�߻��Զ������ٶ�ǰ��!";
	gotoxy(10, 12); cout << "����ʳ��Ҳ����";
	gotoxy(10, 13); cout << "Ŭ���õ�50�ְ�!";
	gotoxy(18, 16); cout << "[�ո�] ��ʼ";
	cout << flush;
}

int SuperSpeed_mode::receiveCommand() {                             //���ٴ�����ٺͼ��ٵ�ָ��
	int command;
	bool got = 0;
	std::set<int> significative = { UP,DOWN,LEFT,RIGHT,PAUSE };     //��Ч��ָ���
	std::map<int, int> opposite = { { UP,DOWN },{ DOWN,UP },{ LEFT,RIGHT },{ RIGHT,LEFT } };  //�����洢�����ϵ��෴��ϵ
	for (int i = 1; i <= SPEED_LV[speed]; ++i) {
		Sleep(SLEEPTIME);
		if (_kbhit()) {
			command = _getch();
			command = std::tolower(command);   //һ�ɲ���Сд��ĸ�ж�
			if (significative.find(command) == significative.end()) continue;                            //������Чָ��� 
			auto map_it = opposite.find(command);
			if (map_it != opposite.end() && map_it->second == (ssnake.begin() + 1)->direction) continue; //��������ͷ��������ǰ��  
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

#endif /*SUPERSPEED_MODE_H*/