#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <conio.h>
#define ESC 27
#define ENTER 13
#define DOWN 80
#define UP 72

char dan[2][20] = {"Calculate ", "Exit      "};
int menu(int n);
void calculate();

void main()
{
	int i, t;
	while(1)
	{
		textbackground(7);
		window(1, 1, 80, 25);
		clrscr();
		window(5, 4, 30, 5);
		textattr(16 * 3 + 4);
		clrscr();
		textcolor(0);
		for (i = 0; i < 2; i++)
		{
			gotoxy(3, i + 1);
			cout << dan[i];
		}
		t = menu(2);
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
}

int menu(int n)
{
	int y = 1;
	char c;
	gotoxy(3, y);
	textattr(16 * 1 + 14);
	textcolor(0);
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

void calculate()
{
	textattr(16 * 4 + 7);
	window(40, 4, 70, 5);
	clrscr();
	getch();

}