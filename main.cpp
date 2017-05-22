#include <iostream>
#include <math.h>
#include <conio.h>
#define ESC 27
#define ENTER 13
#define DOWN 'D'
#define UP 'U'

using namespace std;

char dan[2][20] = {"Calculate ", "Exit      "};
int menu(int n);
int calculate();

int main()
{
	int i, t;
	while(1)
	{
		textbackground(7);
		window(1, 1, 80, 25);
		clrscr();
		window(5, 4, 30, 7);
		textattr(16 * 3 + 3);
		clrscr();
		textcolor(0);
		for (i = 0; i < 2; i++)
		{
			gotoxy(3, i + 1);
			cout << dan[i];
		}
		t = menu(4);
		switch(t)
		{
			case 1: calculate(); break;
			case 2: case 27:
				window(1, 1, 80, 25);
				textbackground(7);
				clrscr();
				exit(0);
		}
	}
	return 0;
}

int menu(int n)
{
	int y = 1;
	char c;
	gotoxy(3, y);
	textattr(16 * 1 + 14);
	gotoxy(3, y);
	cout << dan[0];
	do
	{
		c = getch();
		textattr(16 * 3 + 0);
		gotoxy(3, y);
		cout << dan[y - 1];
		switch(c)
		{
			case DOWN: y++; break;
			case UP: y--; break;
			case ENTER: return y;
		}
		if (y > n) y = 1;
		if (y < 1) y = n;
		gotoxy(3, y);
		textattr(16 * 1 + 14);
		cout << dan[y - 1];
	}
	while (c != ESC);
	return c;
}
