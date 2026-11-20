#include <windows.h>
#include "game.h"

int main(void) {
	SetConsoleOutputCP(65001);
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &(CONSOLE_CURSOR_INFO) { 100, FALSE });
	while (game());
	return 0;
}
