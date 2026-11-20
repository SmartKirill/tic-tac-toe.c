#pragma once
#include <windows.h>

void clearConsole();
void changeCursorColor();
void moveCursorPos(int dx, int dy);
void writeOnPos(int x, int y, const char str[]);

