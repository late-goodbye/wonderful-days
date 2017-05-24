#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <conio.h>

#define ESC 27
#define ENTER 13
#define DOWN 80
#define UP 72
#define LEFT 75
#define RIGHT 77

char dan[2][20] = {"Calculate ", "Exit      "};
int data[32] = {0,};
int week[7];
int best_first_days[32];
int best_days = 0;

int menu(int n);
void calculate();
int read_the_data();
void find_the_best_days();
void write_the_result(int page);
void clear_best_first_days();

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
	int current_page = 1;
	char next;
	textattr(16 * 4 + 7);
	window(40, 4, 70, 13);
	clrscr();
	gotoxy(2, 1);
	cout << "Result:";
	gotoxy(2, 2);

	if (read_the_data() == 0)
	{
		find_the_best_days();
		do
		{
			gotoxy(10, 1);
			cout << "                      ";
			gotoxy(10, 1);
			cout << "Page " << current_page << "/" << best_days;
			write_the_result(current_page - 1);
			next = getch();
			switch(next)
			{
				case LEFT: current_page--; break;
				case RIGHT: current_page++; break;
			}
			if (current_page < 1) current_page = 1;
			if (current_page > best_days) current_page = best_days;
		}
		while (next != ENTER && next != ESC);
	}
	else getch();
}

int read_the_data()
{
	int month, day, temp, count = 0;
	ifstream finput;
	finput.open("C:\\DOSBOX\\WONDAYS\\data3.txt", ios::in);
	if (!finput)
	{
		cout << "Error: the file can't be open";
		return 1;
	}
	while (!finput.eof())
	{
		finput >> month >> day >> temp;
		if (month == 8) day += 31;
		data[day - 15] += temp;
		count++;
	}
	finput.close();
	return 0;
}

void find_the_best_days()
{
	int i, cur_temp = 0, max_temp = 0, first_day = 0;
	clear_best_first_days();

	for (i = 0; i < 7; i++)
	{
		cur_temp += data[i];
	}

	max_temp = cur_temp;
	best_days = 1;
	best_first_days[0] = 0;

	for (first_day = 1; first_day < 26; first_day++)
	{
		cur_temp -= data[first_day - 1];
		cur_temp += data[first_day + 6];
		if (cur_temp == max_temp)
		{
			for (i = 0; i < 32; i++)
			{
				if (best_first_days[i] == -1)
				{
					best_days++;
					best_first_days[i] = first_day;
					break;
				}
			}
		}
		else if (cur_temp > max_temp)
		{
			clear_best_first_days();
			best_first_days[0] = first_day;
			best_days = 1;
			max_temp = cur_temp;
		}
	}
}

void write_the_result(int page)
{
	int i, y = 3;

	for (i = best_first_days[page]; i < best_first_days[page] + 7; i++)
	//for (i = 23; i < 30; i++)
	{
		gotoxy(2, y);
		cout << "                    ";
		gotoxy(2, y);
		if (i > 16) cout<<i - 16<<" of August: "<<data[i]/10<<"C";
		else cout << i + 15 << " of July: " << data[i] / 10<<"C";
		y++;
	}
}

void clear_best_first_days()
{
	int i;
	for (i = 0; i < 32; i++)
	{
		best_first_days[i] = -1;
	}
}