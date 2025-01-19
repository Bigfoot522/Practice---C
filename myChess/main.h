#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
	char board[8][8];
	int turn;
} ChessGame;

extern ChessGame* CB;

int isRunning;
int Wcastling[2];
int Bcastling[2];
int staleMate;

void PrintBoard();
void Turn();
void PieceMove(char* leavePoint, char* arrivePoint);
int CheckMate(ChessGame* chessBoard, int king_x, int king_y);
void isCheckMate();
void isStaleMate();
int enPassant(ChessGame* chessBoard, int lp_rows, int lp_cols, int ap_rows, int ap_cols);
void enPassantTrigger(int i, int j, int lp_rows, int lp_cols, char arrivePoint);
void Promotion(int ap_rows, int ap_cols);
int castling(int direction);
int defence(char piece, int row, int col);
void end();

int defenceP(char piece, int row, int col);
int defencep(char piece, int row, int col);
int defenceN(char piece, int row, int col);
int defencen(char piece, int row, int col);
int defenceB(char piece, int row, int col);
int defenceb(char piece, int row, int col);
int defenceR(char piece, int row, int col);
int defencer(char piece, int row, int col);
int defenceQ(char piece, int row, int col);
int defenceq(char piece, int row, int col);
int defenceK(char piece, int row, int col);
int defencek(char piece, int row, int col);

int moveP(int i, int j, int lp_cols, char arrivePoint);
int movep(int i, int j, int lp_cols, char arrivePoint);
int moveN(int i, int j);
int moven(int i, int j);
int moveB(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int moveb(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int moveR(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int mover(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int moveQ(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int moveq(char arrivePoint, int i, int j, int lp_rows, int lp_cols);
int moveK(int i, int j);
int movek(int i, int j);