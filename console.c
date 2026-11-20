#include <stdio.h>
#include "console.h"
#include "game.h"

void clearConsole() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	printf("\033[2J\033[H");
	GetConsoleScreenBufferInfo(hConsole, &csbi);
}

void changeCursorColor() {
	static int isVisible = 0;

	if (isVisible) {
		printf("\033[?25l");
		isVisible = 0;
	}
	else {
		printf("\033[1 q\033]12;#FFFFFF\007\033[?25h");
		isVisible = 1;
	}
	fflush(stdout);
}

void moveCursorPos(int dx, int dy) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		int col = ((csbi.dwCursorPosition.X - 2) / 4 + dx + N) % N;
		int row = ((csbi.dwCursorPosition.Y - 1) / 2 + dy + M) % M;

		COORD newPos = { (SHORT)(col * 4 + 2), (SHORT)(row * 2 + 1) };
		SetConsoleCursorPosition(hConsole, newPos);
	}
}

void writeOnPos(int x, int y, const char str[]) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	printf("\033[%d;%dH%s", y, x, str);
	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}
