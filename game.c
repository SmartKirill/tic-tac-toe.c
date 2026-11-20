#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include "game.h"
#include "console.h"

int N, M;
char playersSymb[2];
int board[MAX][MAX];

static int winR = -1, winC = -1;
static int winDR = 0, winDC = 0;
static int winLen = 0;

void clearBoard() {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			board[i][j] = 0;
		}
	}
}

void writeWhoMove(int playerNow) {
	if (!playerNow) {
		writeOnPos(4 * N + 10, 5, "Ходит игрок 1");
	}
	else {
		writeOnPos(4 * N + 10, 5, "Ходит игрок 2");
	}
}

void drawWinLine() {
	const char* lineSymb = "-";
	if (winDR == 0 && winDC != 0) lineSymb = "─";
	else if (winDR != 0 && winDC == 0) lineSymb = "│";
	else if (winDR == winDC) lineSymb = "\\";
	else lineSymb = "/";

	for (int k = 0; k < winLen - 1; ++k) {
		int r1 = winR + k * winDR;
		int c1 = winC + k * winDC;

		int x1 = c1 * 4 + 3;
		int y1 = r1 * 2 + 2;

		writeOnPos(x1 + winDC * 2, y1 + winDR, lineSymb);
	}
}

int checkWin(char player) {
	int count, len;

	for (int i = 0; i < M; i++) {
		count = 0;
		for (int j = 0; j < N; j++) if (board[i][j] == player) count++;
		if (count == N) {
			winR = i; winC = 0; winDR = 0; winDC = 1; winLen = N;
			return 1;
		}
	}

	for (int j = 0; j < N; j++) {
		count = 0;
		for (int i = 0; i < M; i++) if (board[i][j] == player) count++;
		if (count == M) {
			winR = 0; winC = j; winDR = 1; winDC = 0; winLen = M;
			return 1;
		}
	}

	for (int i = 0; i < M; i++) {
		count = 0; len = 0;
		for (int r = i, c = 0; r < M && c < N; r++, c++) {
			len++;
			if (board[r][c] == player) count++;
		}
		if (len > 1 && count == len) {
			winR = i; winC = 0; winDR = 1; winDC = 1; winLen = len;
			return 1;
		}
	}
	for (int j = 1; j < N; j++) {
		count = 0; len = 0;
		for (int r = 0, c = j; r < M && c < N; r++, c++) {
			len++;
			if (board[r][c] == player) count++;
		}
		if (len > 1 && count == len) {
			winR = 0; winC = j; winDR = 1; winDC = 1; winLen = len;
			return 1;
		}
	}

	for (int i = M - 1; i >= 0; i--) {
		count = 0; len = 0;
		for (int r = i, c = 0; r >= 0 && c < N; r--, c++) {
			len++;
			if (board[r][c] == player) count++;
		}
		if (len > 1 && count == len) {
			winR = i; winC = 0; winDR = -1; winDC = 1; winLen = len;
			return 1;
		}
	}
	for (int j = 1; j < N; j++) {
		count = 0; len = 0;
		for (int r = M - 1, c = j; r >= 0 && c < N; r--, c++) {
			len++;
			if (board[r][c] == player) count++;
		}
		if (len > 1 && count == len) {
			winR = M - 1; winC = j; winDR = -1; winDC = 1; winLen = len;
			return 1;
		}
	}

	return 0;
}

int checkDraw() {
	for (int i = 0; i < M; ++i) {
		for (int j = 0; j < N; ++j) {
			if (board[i][j] == '\0' || board[i][j] == ' ') {
				return 0;
			}
		}
	}
	return 1;
}

void makeMove(int* playerNow) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		int col = (csbi.dwCursorPosition.X - 2) / 4;
		int row = (csbi.dwCursorPosition.Y - 1) / 2;
		if (board[row][col] == 0 || board[row][col] == ' ') {
			char symb = playersSymb[*playerNow];
			board[row][col] = symb;
			printf("%c", symb);
			SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
			*playerNow = *playerNow == 0 ? 1 : 0;
		}
	}
}

void writeBoard() {
	printf("┌");
	for (int i = 0; i < N - 1; ++i) {
		printf("───┬");
	}
	printf("───┐\n");
	for (int i = 0; i < M - 1; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("│ %c ", board[i][j] == 0 ? ' ' : board[i][j]);
		}
		printf("│\n");
		printf("├");
		for (int j = 0; j < N - 1; ++j) {
			printf("───┼");
		}
		printf("───┤\n");
	}
	for (int j = 0; j < N; ++j) {
		printf("│ %c ", board[M - 1][j] == 0 ? ' ' : board[M - 1][j]);
	}
	printf("│\n");
	printf("└");
	for (int i = 0; i < N - 1; ++i) {
		printf("───┴");
	}
	printf("───┘\n");

	printf("\n\nПеремещение на стрелочки ←↑→↓\nСделать ход: enter↵\n0 - остановить игру");
}

int game() {
	int playerNow = 0;
	clearBoard();
	clearConsole();
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
	while (1) {
		writeOnPos(2, 2, "1 - начать игру\n 0 - выход");
		int key = _getch();
		if (key == '1') {
			clearConsole();
			break;
		}
		else if (key == '0') {
			return 0;
		}
	}
	while (1) {
		printf("Введите размеры доски:");
		scanf_s("%d %d", &N, &M);
		if (N > MAX || M > MAX || N < 1 || M < 1) {
			printf("Максимальный размер доски %dx%d\n", MAX, MAX);
		}
		else {
			break;
		}
	}
	printf("Выберите символ игрока 1:");
	scanf_s(" %c", &playersSymb[0], 1);
	while (1) {
		printf("Выберите символ игрока 2:");
		scanf_s(" %c", &playersSymb[1], 1);
		if (playersSymb[0] == playersSymb[1]) {
			printf("Символы игрока 1 и игрока 2 должны быть разные!\n");
		}
		else {
			break;
		}
	}
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);
	changeCursorColor();
	clearConsole();
	writeBoard();
	writeWhoMove(playerNow);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)2, (SHORT)1 };
	SetConsoleCursorPosition(hConsole, pos);
	while (1) {
		writeWhoMove(playerNow);
		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			if (key == 72) moveCursorPos(0, -1);
			if (key == 80) moveCursorPos(0, 1);
			if (key == 75) moveCursorPos(-1, 0);
			if (key == 77) moveCursorPos(1, 0);
		}
		else if (key == 13) {
			char playerSymb = playersSymb[playerNow];
			makeMove(&playerNow);
			if (checkWin(playerSymb)) {
				playerNow = playerNow == 0 ? 1 : 0;
				writeWhoMove(playerNow);
				drawWinLine();
				Sleep(1500);
				clearConsole();
				changeCursorColor();
				printf("\n");
				printf("  ╔═════════════════════════════════════════════════════════════════════════════╗\n");
				printf("  ║                                                                             ║\n");
				printf("  ║   ███  ███  ███  ███  ███  █  █  ███      █  █  ███  ███  ███  █  █    %d    ║\n", playerNow + 1);
				printf("  ║   █ █  █ █  █    █    █ █  █  █  █ █      █  █  █    █ █  █ █  █ █    ███   ║\n");
				printf("  ║   █ █  █ █  ███  ███  █ █  █ ██  █ █      █ ██  █    █ █  █ █  ██      █    ║\n");
				printf("  ║   █ █  █ █  █ █  █    ███  ██ █  █ █      ██ █  █    ███  █ █  █ █     █    ║\n");
				printf("  ║   █ █  ███  ███  ███  █ █  █  █  █ █      █  █  █    █    ███  █  █   ███   ║\n");
				printf("  ║                                                                             ║\n");
				printf("  ╚═════════════════════════════════════════════════════════════════════════════╝\n");
				printf("\n\n                   Нажмите любую клавишу, чтобы продолжить...");
				_getch();
				return 1;
			}
			else if (checkDraw()) {
				clearConsole();
				changeCursorColor();
				printf("\n");
				printf("  ╔═════════════════════════════════════╗\n");
				printf("  ║                                     ║\n");
				printf("  ║   █  █  █  █  █  █  █     ███   █   ║\n");
				printf("  ║   █  █  █  █  █  █  █     █ █   █   ║\n");
				printf("  ║   ████  █ ██  ████  ███   ███   █   ║\n");
				printf("  ║   █  █  ██ █     █  █ █  █  █       ║\n");
				printf("  ║   █  █  █  █     █  ███  █  █   █   ║\n");
				printf("  ║                                     ║\n");
				printf("  ╚═════════════════════════════════════╝\n");
				printf("\n\n  Нажмите любую клавишу, чтобы продолжить...");
				_getch();
				return 1;
			}
		}
		else if (key == '0') {
			changeCursorColor();
			return 1;
		}
	}
}
