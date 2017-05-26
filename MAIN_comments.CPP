#include <iostream.h>

// библиотека для работы с файлами
#include <fstream.h>

#include <stdlib.h>

// библиотека для работы с графикой (GUI)
#include <conio.h>

// константы для кодов клавиш, чтобы код программы был понятнее
// сами коды мы будем получать из функции getch()
#define ESC 27
#define ENTER 13
#define DOWN 80
#define UP 72
#define LEFT 75
#define RIGHT 77

// массив с пунктами меню
char dan[2][20] = {"Calculate ", "Exit      "};

// массив с суммой температур для каждого дня
// всего их у нас 17 июльских + 15 августовских = 32
// по умолчанию значения 0
int data[32] = {0,};

// Т.к. нам нужно искать идущие подряд дни, то проще всего будет запоминать
// только первый день из серии
// Этого достаточно, чтобы восстановить её
// В этом массиве храним первые дни самых тёплых недель
// Всего их может быть до 32 (если во все дни температура одинакова)
int best_first_days[32];

// в этой переменной храним количество таких первых дней тёплых недель
int best_days = 0;

// прототип функции для выбора пункта меню
int menu(int n);

// прототип основной функции для вычеслений
void calculate();

// прототип функции для чтения данных
int read_the_data();

// прототип функции для поиска самых тёплых семидневок
void find_the_best_days();

// прототип функции для вывода дней из одной семидневки
void write_the_result(int page);

// прототип функции для стирания массива первых дней
// нужно в случае, если мы нашли более тёплые семидневки, чем те, что были
// тогда нам все предыдузие нужно стереть
void clear_best_first_days();


void main()
{
	int i, t; // i - переменная для итерации, t - переменная, хранящая выбранный в меню пункт
	// бесконечный цикл, который завершается выбором Exit в меню
	while(1)
	{	
		// выбираем цвет фона
		textbackground(7);
		// создаём окно на весь экран
		window(1, 1, 80, 25);
		// заливаем его
		clrscr();
		// создаём окошко для меню
		window(5, 4, 30, 5);
		// выбираем цвет фона и текста для меню
		textattr(16 * 3 + 4);
		// заливаем
		clrscr();
		textcolor(0);
		// в этом цикле выводим пункты меню на экран
		for (i = 0; i < 2; i++)
		{
			// перемещаемся на строку ниже
			gotoxy(3, i + 1);
			// выводим текст пункта
			cout << dan[i];
		}
		// вызываем функцию выбора пункта меню
		t = menu(2);
		// выбираем действие в зависимости от выбора
		switch(t)
		{
			// если выбран первый пункт (calculate), то вызываем функцию calculate
			case 1: calculate(); break;
			// если второй либо нажат Esc
			case 2: case 27:
				// Очищаем экран
				window(1, 1, 80, 25);
				textbackground(7);
				clrscr();
				// и завершаем программу с кодом 0 (всё ок)
				exit(0);
		}
	}
}

int menu(int n) // n - количество строк в меню
{
	// номер текущей строки
	int y = 1;

	// нажатая пользователем клавиша
	char c;

	// переходим на текущую строку с отступом слева в 3 символа (для красоты)
	gotoxy(3, y);

	// выбираем цвет фона и текста
	textattr(16 * 1 + 14);
	textcolor(0);
	gotoxy(3, y);
	// выводим текст первого пункта меню на экран
	cout << dan[0];
	do
	{
		// ждём, пока пользователь нажмёт клавишу
		c = getch();
		textattr(16 * 3 + 0);
		gotoxy(3, y);
		cout << dan[y - 1];

		// в зависимости от нажатой клавиши
		switch(c)
		{
			// если вниз, то текущей становится строка ниже
			case DOWN: y++; break;
			// если вверх, то строка выше
			case UP: y--; break;
			// если enter, то функция возвращает значение текущей строки
			case ENTER: return y;
		}
		// если строка получилась ниже последней, то выделяется первый пункт
		if (y > n) y = 1;
		// если строка выше первой, то последний
		if (y < 1) y = n;
		gotoxy(3, y);
		textattr(16 * 1 + 14);
		cout << dan[y - 1];
	}
	// также цикл завершается, если нажат Esc
	while (c != ESC);
	// в конце возвращаем надатую пользователем клавишу (то есть Esc, другой быть не может)
	return c;
}

// основная функция для вычисления дней
void calculate()
{
	// страница результата по умолчанию
	int current_page = 1;

	// следующая страница (её значение зависит от того, нажа пользователь влево или вправо)
	// хранит код клавиши
	char next;
	// выбираем цвет текста и фона
	textattr(16 * 4 + 7);
	// создаём окошко справа для вывода результата
	window(40, 4, 70, 13);
	// заливаем его
	clrscr();
	// переходи к первой строке с отступом в символ слева
	gotoxy(2, 1);
	// выводим эту строку
	cout << "Result:";
	// переходим на вторую строку
	gotoxy(2, 2);

	// считываем данные из файла
	// при этом функция возвращает 0, если всё ок
	// и 1, если файл не считался
	if (read_the_data() == 0)
	{	
		// если файл считался, то ищем самые тёплые семидневки
		find_the_best_days();
		do
		{
			// очищаем строку
			gotoxy(10, 1);
			cout << "                      ";
			// и выводим туда номер семидневки и общее их число
			gotoxy(10, 1);
			cout << "Page " << current_page << "/" << best_days;
			// выводим сами дни
			// -1 здесь потому, что нумерация семидневок у нас с 1
			// а в массиве с 0
			write_the_result(current_page - 1);
			// ждём, что нажмёт пользователь
			next = getch();
			switch(next)
			{
				// если влево, то предыдущая страница
				case LEFT: current_page--; break;
				// если вправо, то следующая
				case RIGHT: current_page++; break;
			}
			// если номер страницы меньше первого, то остаёмся на первой
			if (current_page < 1) current_page = 1;
			// если больше последнего, то остаёмся на последней
			if (current_page > best_days) current_page = best_days;
		}
		// если пользователь нажал Enter или Esc, то выходим
		while (next != ENTER && next != ESC);
	}
	// если файл не считался, то ждём нажатия любой клавиши
	// после чего происходит возврат к выбору пункта меню
	else getch();
}

// функция для чтения данных из файла
int read_the_data()
{
	// в каждой строке будет месяц, день и температура
	int month, day, temp;
	// объявляем входной поток
	ifstream finput;
	// открываем файл (нужно будет изменить путь на свой)
	finput.open("C:\\DOSBOX\\WONDAYS\\data3.txt", ios::in);
	// если файл не открыт
	if (!finput)
	{
		// выводим сообщение об ошибке
		cout << "Error: the file can't be open";
		// возвращаем код ошибки 1
		return 1;
	}
	//
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
	char deg = 248;

	for (i = best_first_days[page]; i < best_first_days[page] + 7; i++)
	{
		gotoxy(2, y);
		cout << "                    ";
		gotoxy(2, y);
		if (i > 16) cout<<i - 16<<" of August: "<<data[i]/10<<deg<<"C";
		else cout << i + 15 << " of July: " << data[i] / 10<<deg<<"C";
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