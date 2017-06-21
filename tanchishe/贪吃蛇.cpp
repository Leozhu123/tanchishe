#include "Classic_mode.h"
int gameSize = 50;
using namespace std;

void gotoxy(int x, int y) {  //�ƶ���굽��x�е�y�С����Ǻ�����ΪʲôҪ�������
	COORD c;
	c.X = x - 1;
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void HideCursor() {          //���ع��
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int drawMenu() {
	system("cls");
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
	gotoxy(9, 7);
	cout << "��ӭ����̰������Ϸ";
	gotoxy(15, 9);
	cout << "��ѡ��:";
	gotoxy(11, 11);
	cout << "����ģʽ �밴1";
	gotoxy(11, 12);
	cout << "ը��ģʽ ������";
	cout << flush;
	while (1) { //�ȴ�����ո������Ϸ 
		if (_getch() == 49) return 1;
		if (_getch() == 50) return 2;
	}
}

int main() {
	HideCursor();
	bool restart;
	do {
		restart = 0;
		int mode = drawMenu();
		switch (mode) {
		case 1: {	/*����ģʽ*/
			Classic_mode cm;
			cm.help();
			while (1) {
				char c;
				c = _getch();
				if (c == 32) {break;
				else if (c == 'z') {
					int a;
						cin >> a;
						gameSize = a;
				} 
			}
			cm.init();
			cm.run();
			break;
		}
				/*	case 2: {	ը��ģʽ
				Bomb_mode bm;
				bm.help();
				while (1) if (_getch() == 32) break;
				bm.init();
				bm.run();
				break;
				}      */
		}
		gotoxy(13, 13);
		cout << "[�ո�] �������˵�";
		gotoxy(14, 14);
		cout << "[�س�] �˳���Ϸ";
		while (1) {
			if (_getch() == 32) { restart = 1; break; }
			if (_getch() == 13) break;
		}
	} while (restart);
	return 0;
}