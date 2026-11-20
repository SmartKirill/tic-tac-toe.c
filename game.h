#pragma once

#define MAX 12

extern int N, M;
extern char playersSymb[2];
extern int board[MAX][MAX];

void clearBoard();
void writeWhoMove(int playerNow);
void drawWinLine();
int checkWin(char player);
int checkDraw();
void makeMove(int* playerNow);
void writeBoard();
int game();
