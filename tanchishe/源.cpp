/****************************************************************************************
* 	����һ��̰������Ϸ����
* 	�����������İ汾����4�·���ɵģ�������ʱ�����C++�ı�׼���Լ��������ĳ���
* ��ƶ�ֻ��һ֪�����ѣ����д�����Ĵ���Խ��Խ�ң�C++98��C++11��׼�Ĵ��������һ��
* ��ֱ���񳵻��ֳ���
*	��������ѧϰ��һЩ���֪ʶ��֪���˹��캯������װ�ȵȵ����ã��Լ�����д��ͷ�ļ���ĺ�
* ����ͬʱҲ���������ѧϰ��STL��ص�֪ʶ�����ǣ��Ҿ�����һ����ʱ����д�������
*	��ԭ��������У����˾���ģʽ�⣬4.0�汾�������µ��淨��ը��ģʽ�����������ڴ������
* ��̫�ά���ɱ�ʵ��̫�ߣ��Ǹ������д���ٳ���������һ�������漰���̳еĸ���Ҿ���
* �ȵ�ѧ��������ʵ��������ܡ�
*	�������������ƹ����У��һ���Ҫ��лԶ��Ӣ����У��֣��ҫ�������ҵİ������ݺ��ӣ�Ҳ
* ������һ��UMLͼ�����ҽ���������ġ���װ����˼�룬�Ҳŵ��Ի���˳���ؿ��������̵�����
* ���������ż���
*                                     Produced by GKWuxi, Wuxi Big Bridge Academy.
*****************************************************************************************/

#include "Classic_mode.h"
#include "SuperSpeed_mode.h"

using namespace std;

void gotoxy(int x, int y) {  //�ƶ���굽��x�е�y��
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
	cout << "����ģʽ �밴2";
	cout << flush;
	while (1) { //�ȴ����������Ϸ 
		if (_getch() == 49) return 1;
		if (_getch() == 50) return 2;
	}
}

void run_mode(Classic_mode& mode) { //��������
	mode.help();
	while (1) if (_getch() == 32) break;
	mode.init();
	mode.run();
}

int main() {
	HideCursor();
	bool restart;
	do {
		restart = 0;
		int mode = drawMenu();
		switch (mode) {
		case 1: {	/*����ģʽ*/
			Classic_mode cm(20);
			run_mode(cm);
			break;
		    }
		case 2: {	/*����ģʽ*/
			SuperSpeed_mode ssm(20);
			run_mode(ssm);
			break;
			}      
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