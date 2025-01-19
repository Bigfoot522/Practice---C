#include "main.h"

ChessGame chessBoard = {
	.board = {
   { 'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r' },
   { 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p' },
   { '-', '-', '-', '-', '-', '-', '-', '-' },
   { '-', '-', '-', '-', '-', '-', '-', '-' },
   { '-', '-', '-', '-', '-', '-', '-', '-' },
   { '-', '-', '-', '-', '-', '-', '-', '-' },
   { 'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P' },
   { 'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R' }
},
	.turn = 0
};

ChessGame copyChessBoard;

ChessGame* CB = &chessBoard;

struct WillEnp {
	int count;
	int one[2];
	int two[2];
}canEnp;

int Wcastling[2] = { 0, 0 };
int Bcastling[2] = { 0, 0 };

int staleMate = 0;

void PrintBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			printf("%c ", CB->board[i][j]);
		printf("\n");
	}
	printf("\n");
}

void Turn() {
	if (CB->turn == 0) {
		CB->turn = 1;
	}
	else if (CB->turn == 1)
		CB->turn = 0;
}

void PieceMove(char* leavePoint, char* arrivePoint) {

	int lp_rows = leavePoint[0] - 97;						// 입력받은 위치를 행렬 인덱스로 변환
	int lp_cols = (leavePoint[1] - 48) * 9 % 10 - 2;
	int ap_rows = arrivePoint[0] - 97;						// 가로
	int ap_cols = (arrivePoint[1] - 48) * 9 % 10 - 2;		// 세로

	char lp = CB->board[lp_cols][lp_rows];			// 시작위치 char 변수
	char ap = CB->board[ap_cols][ap_rows];			// 도착위치 char 변수

	char selectPiece = CB->board[lp_cols][lp_rows];
	int i = lp_rows - ap_rows;								// 도착 위치가 올바른지 판단하기 위한 변수
	int j = lp_cols - ap_cols;
	int possible;											// 위치 이동 가능 여부

	// 색에 맞는 움직임을 입력했는지
	if (CB->turn == 0) {
		if (!(selectPiece > 65 && selectPiece < 83))
			return;
	}
	if (CB->turn == 1) {
		if (!(selectPiece > 97 && selectPiece < 115))
			return;
	}

	// 이동이 가능한 위치인지
	if (!(ap_rows > -1 && ap_rows < 8))
		return;
	else if (!(ap_cols > -1 && ap_cols < 8))
		return;

	if (selectPiece == 'K' || selectPiece == 'k') {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		cCB->board[ap_cols][ap_rows] = 'K';
		cCB->board[lp_cols][lp_rows] = '-';

		if (CheckMate(cCB, ap_rows, ap_cols))
			return;
	}
	else {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		cCB->board[ap_cols][ap_rows] = selectPiece;
		cCB->board[lp_cols][lp_rows] = '-';

		if (CheckMate(cCB, 0, 0))
			return;
	}

	// 기물과 위치 선택에 따른 움직임 (M(ove) W(hite) P(awn))
	if (selectPiece == 'P') {
		enPassantTrigger(i, j, lp_rows, lp_cols, ap);
		possible = moveP(i, j, lp_cols, ap);
		if (possible == 1) {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
		if (possible == 0)
			possible = enPassant(CB, lp_rows, lp_cols, ap_rows, ap_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
		Promotion(ap_rows, ap_cols);
	}
	else if (selectPiece == 'p') {
		enPassantTrigger(i, j, lp_rows, lp_cols, ap);
		possible = movep(i, j, lp_cols, ap);
		if (possible == 1) {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
		if (possible == 0)
			possible = enPassant(CB, lp_rows, lp_cols, ap_rows, ap_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
		Promotion(ap_rows, ap_cols);
	}
	else if (selectPiece == 'N') {
		possible = moveN(i, j);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'n') {
		possible = moven(i, j);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'B') {
		possible = moveB(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'b') {
		possible = moveb(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'R') {
		possible = moveR(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			if (lp_rows == 0)
				Wcastling[0] = 1;
			else if (lp_rows == 7)
				Wcastling[1] = 1;
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'r') {
		possible = mover(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			if (lp_rows == 0)
				Bcastling[0] = 1;
			else if (lp_rows == 7)
				Bcastling[1] = 1;
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'Q') {
		possible = moveQ(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'q') {
		possible = moveq(ap, i, j, lp_rows, lp_cols);
		if (possible == 0)
			return;
		else {
			CB->board[ap_cols][ap_rows] = lp;
			CB->board[lp_cols][lp_rows] = '-';
		}
	}
	else if (selectPiece == 'K') {
		int kMove[3] = { -1, 0, 1 };

		if (ap_cols == 7 && ap_rows == 2) {
			possible = castling(1);
			if (possible == 0) {
				return;
			}
		}
		else if (ap_cols == 7 && ap_rows == 6) {
			possible = castling(2);
			if (possible == 0) {
				return;
			}
		}
		else
			possible = moveK(i, j);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (CB->board[ap_cols + kMove[i]][ap_rows + kMove[j]] == 'k')
					return;
			}
		}

		CB->board[ap_cols][ap_rows] = lp;
		CB->board[lp_cols][lp_rows] = '-';
	}
	else if (selectPiece == 'k') {
		int kMove[3] = { -1, 0, 1 };

		if (ap_cols == 0 && ap_rows == 2) {
			possible = castling(3);
			if (possible == 0) {
				return;
			}
		}
		else if (ap_cols == 0 && ap_rows == 6) {
			possible = castling(4);
			if (possible == 0) {
				return;
			}
		}
		else
			possible = moveK(i, j);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (CB->board[ap_cols + kMove[i]][ap_rows + kMove[j]] == 'K')
					return;
			}
		}

		CB->board[ap_cols][ap_rows] = lp;
		CB->board[lp_cols][lp_rows] = '-';
	}

	if (!(canEnp.count == 0))
		canEnp.count -= 1;

	Turn();
	isCheckMate();
	// isStaleMate();
}

int CheckMate(ChessGame* chessBoard, int king_y, int king_x) {
	int row, col;
	int find = 0;

	if (chessBoard->turn == 0) {							// 킹 위치 확인
		if (king_x == 0 && king_y == 0) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (chessBoard->board[i][j] == 'K') {
						row = i;
						col = j;
						find = 1;
						break;
					}
				}
				if (find == 1)
					break;
			}
		}
		else {
			row = king_x;
			col = king_y;
		}
		if (chessBoard->board[row - 1][col - 1] == 'p' || chessBoard->board[row - 1][col + 1] == 'p')	// 폰
			return 1;
		if (chessBoard->board[row + 2][col + 1] == 'n' || chessBoard->board[row + 2][col - 1] == 'n')	// 나이트
			return 1;
		if (chessBoard->board[row - 2][col + 1] == 'n' || chessBoard->board[row - 2][col - 1] == 'n')
			return 1;
		if (chessBoard->board[row + 1][col + 2] == 'n' || chessBoard->board[row - 1][col + 2] == 'n')
			return 1;
		if (chessBoard->board[row + 1][col - 2] == 'n' || chessBoard->board[row - 1][col - 2] == 'n')
			return 1;
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col + i] == 'b' || chessBoard->board[row + i][col + i] == 'q') // 비숍, 퀸
				return 1;
			else if (!(chessBoard->board[row + i][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col + i] == 'b' || chessBoard->board[row - i][col + i] == 'q')
				return 1;
			else if (!(chessBoard->board[row - i][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col - i] == 'b' || chessBoard->board[row + i][col - i] == 'q')
				return 1;
			else if (!(chessBoard->board[row + i][col - i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col - i] == 'b' || chessBoard->board[row - i][col - i] == 'q')
				return 1;
			else if (!(chessBoard->board[row - i][col - i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col] == 'r' || chessBoard->board[row + i][col] == 'q')	// 룩, 퀸
				return 1;
			else if (!(chessBoard->board[row + i][col] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col] == 'r' || chessBoard->board[row - i][col] == 'q')
				return 1;
			else if (!(chessBoard->board[row - i][col] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row][col + i] == 'r' || chessBoard->board[row][col + i] == 'q')
				return 1;
			else if (!(chessBoard->board[row][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row][col - i] == 'r' || chessBoard->board[row][col - i] == 'q')
				return 1;
			else if (!(chessBoard->board[row][col - i] == '-'))
				break;
		}
	}
	else {
		if (king_x == 0 && king_y == 0) {					// 킹 위치 확인
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (chessBoard->board[i][j] == 'k') {
						row = i;
						col = j;
						find = 1;
						break;
					}
				}
				if (find == 1)
					break;
			}
		}
		else {
			row = king_x;
			col = king_y;
		}
		if (chessBoard->board[row + 1][col + 1] == 'P' || chessBoard->board[row + 1][col - 1] == 'P')	// 폰
			return 1;
		if (chessBoard->board[row + 2][col + 1] == 'N' || chessBoard->board[row + 2][col - 1] == 'N')	// 나이트
			return 1;
		if (chessBoard->board[row - 2][col + 1] == 'N' || chessBoard->board[row - 2][col - 1] == 'N')
			return 1;
		if (chessBoard->board[row + 1][col + 2] == 'N' || chessBoard->board[row - 1][col + 2] == 'N')
			return 1;
		if (chessBoard->board[row + 1][col - 2] == 'N' || chessBoard->board[row - 1][col - 2] == 'N')
			return 1;
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col + i] == 'B' || chessBoard->board[row + i][col + i] == 'Q') // 비숍, 퀸
				return 1;
			else if (!(chessBoard->board[row + i][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col + i] == 'B' || chessBoard->board[row - i][col + i] == 'Q')
				return 1;
			else if (!(chessBoard->board[row - i][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col - i] == 'B' || chessBoard->board[row + i][col - i] == 'Q')
				return 1;
			else if (!(chessBoard->board[row + i][col - i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col - i] == 'B' || chessBoard->board[row - i][col - i] == 'Q')
				return 1;
			else if (!(chessBoard->board[row - i][col - i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row + i][col] == 'R' || chessBoard->board[row + i][col] == 'Q')	// 룩, 퀸
				return 1;
			else if (!(chessBoard->board[row + i][col] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row - i][col] == 'R' || chessBoard->board[row - i][col] == 'Q')
				return 1;
			else if (!(chessBoard->board[row - i][col] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row][col + i] == 'R' || chessBoard->board[row][col + i] == 'Q')
				return 1;
			else if (!(chessBoard->board[row][col + i] == '-'))
				break;
		}
		for (int i = 1; i < 8; i++) {
			if (chessBoard->board[row][col - i] == 'R' || chessBoard->board[row][col - i] == 'Q')
				return 1;
			else if (!(chessBoard->board[row][col - i] == '-'))
				break;
		}
	}
	return 0;
}

void isCheckMate() {
	if (CB->turn == 0) { // 흑이 움직였을 때
		if (CheckMate(CB, 0, 0)) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (CB->board[i][j] > 65 && CB->board[i][j] < 83) {
						if (defence(CB->board[i][j], i, j))					// 체크 상태이면 실행
							continue;
						else if (!(defence(CB->board[i][j], i, j)))			// 체크 상태가 아니면 실행
							break;
					}
					if (i == 7 && j == 7)
						end();
				}
			}
		}
	}
	else {				// 백이 움직였을 때
		if (CheckMate(CB, 0, 0)) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (CB->board[i][j] > 97 && CB->board[i][j] < 115) {
						if (defence(CB->board[i][j], i, j))					// 체크 상태이면 실행
							continue;
						else if (!(defence(CB->board[i][j], i, j)))			// 체크 상태가 아니면 실행
							break;
					}
					if (i == 7 && j == 7)
						end();
				}
			}
		}
	}
}

void isStaleMate() {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;

	int row;
	int col;
	int find = 0;
	int kMove[3] = { -1, 0, 1 };
	int possible = 0;

	if (CB->turn == 0) {							// 흑이 움직였을 때
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (cCB->board[i][j] == 'K') {
					row = i;
					col = j;
					find = 1;
					break;
				}
			}
			if (find == 1)
				break;
		}

		find = 0;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (row + kMove[i] < 0 || row + kMove[i] > 7 || col + kMove[j] < 0 || col + kMove[j] > 7)
					continue;
				if (cCB->board[row + kMove[i]][col + kMove[j]] > 97 && cCB->board[row + kMove[i]][col + kMove[j]] < 115 || cCB->board[row + kMove[i]][col + kMove[j]] == '-') {
					for (int x = 0; x < 3; x++) {
						for (int y = 0; y < 3; y++) {
							if (cCB->board[row + kMove[i] + kMove[x]][col + kMove[j] + kMove[y]] == 'k') {
								find = 1;
								break;
							}
						}
						if (find == 1)
							break;
					}
					if (!(CheckMate(cCB, row + kMove[i], col + kMove[i])))
						return;
				}
			}
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				row = i;
				col = j;
				if (cCB->board[row][col] > 65 && cCB->board[row][col] < 83) {
					if (cCB->board[row][col] == 'P') {
						possible = enPassant(cCB, col, row, col - 1, row - 1);
						if (possible == 1) {
							cCB->board[row - 1][col - 1] = 'P';
							cCB->board[row][col] = '-';
						}
						else
							possible = enPassant(cCB, col, row, col + 1, row - 1);
						if (possible == 1) {
							cCB->board[row - 1][col + 1] = 'P';
							cCB->board[row][col] = '-';
						}

						if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
							return;

						memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));

						if (possible == 0 && cCB->board[row - 1][col] == '-') {
							copyChessBoard = chessBoard;

							cCB->board[row - 1][col] = 'P';
							cCB->board[row][col] = '-';
						}

						if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
							return;

						memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
					}
					else if (cCB->board[row][col] == 'N') {
						int moves[] = { 2, -2, 1, -1 };

						for (int i = 0; i < 4; i++) {
							for (int j = 0; j < 4; j++) {

								if ((row + moves[i]) < 0 || (row + moves[i]) > 8 || (col + moves[j]) < 0 || (col + moves[j]) > 8)
									continue;

								cCB->board[row + moves[i]][col + moves[j]] = 'N';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'B') {
						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col + i) > 7)
								break;

							if (cCB->board[row + i][col + i] == '-' || i == 7) {

								cCB->board[row + i][col + i] = 'B';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col - i) < 0)
								break;

							if (cCB->board[row + i][col - i] == '-' || i == 7) {

								cCB->board[row + i][col - i] = 'B';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row - i) < 0 || (col - i) < 0)
								break;

							if (!(cCB->board[row - i][col - i] == '-')) {
								cCB->board[row - i][col - i] = 'B';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;
							}

							else if (cCB->board[row - i][col - i] == '-' || i == 7) {

								cCB->board[row - i][col - i] = 'B';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'R') {
						for (int i = 1; i < 8; i++) {
							if (!(cCB->board[row + i][col] == '-') || row + i > 7)
								break;

							if (cCB->board[row + i][col] == '-' || i == 7) {

								cCB->board[row + i][col] = 'R';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'Q') {
						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col + i) > 7)
								break;

							if (cCB->board[row + i][col + i] == '-' || i == 7) {

								cCB->board[row + i][col + i] = 'Q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col - i) < 0)
								break;

							if (cCB->board[row + i][col - i] == '-' || i == 7) {

								cCB->board[row + i][col - i] = 'Q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row - i) < 0 || (col - i) < 0)
								break;

							if (!(cCB->board[row - i][col - i] == '-')) {
								cCB->board[row - i][col - i] = 'Q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;
							}

							else if (cCB->board[row - i][col - i] == '-' || i == 7) {

								cCB->board[row - i][col - i] = 'Q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
						for (int i = 1; i < 8; i++) {
							if (!(cCB->board[row + i][col] == '-') || row + i > 7)
								break;

							if (cCB->board[row + i][col] == '-' || i == 7) {

								cCB->board[row + i][col] = 'Q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
				}
			}
		}
		staleMate = 1;
		end();
	}
	else {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (cCB->board[i][j] == 'k') {
					row = i;
					col = j;
					find = 1;
					break;
				}
			}
			if (find == 1)
				break;
		}

		find = 0;

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (row + kMove[i] < 0 || row + kMove[i] > 7 || col + kMove[j] < 0 || col + kMove[j] > 7)
					continue;
				if (cCB->board[row + kMove[i]][col + kMove[j]] > 65 && cCB->board[row + kMove[i]][col + kMove[j]] < 83 || cCB->board[row + kMove[i]][col + kMove[j]] == '-') {
					for (int x = 0; x < 3; x++) {
						for (int y = 0; y < 3; y++) {
							if (cCB->board[row + kMove[i] + kMove[x]][col + kMove[j] + kMove[y]] == 'K') {
								find = 1;
								break;
							}
						}
						if (find == 1)
							break;
					}
					if (!(CheckMate(cCB, row + kMove[i], col + kMove[i])))
						return;
				}
			}
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				row = i;
				col = j;
				if (cCB->board[row][col] > 97 && cCB->board[row][col] < 115) {
					if (cCB->board[row][col] == 'p') {
						possible = enPassant(cCB, col, row, col - 1, row - 1);
						if (possible == 1) {
							cCB->board[row - 1][col - 1] = 'p';
							cCB->board[row][col] = '-';
						}
						else
							possible = enPassant(cCB, col, row, col + 1, row - 1);
						if (possible == 1) {
							cCB->board[row - 1][col + 1] = 'p';
							cCB->board[row][col] = '-';
						}

						if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
							return;

						memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));

						if (possible == 0 && cCB->board[row - 1][col] == '-') {
							copyChessBoard = chessBoard;

							cCB->board[row - 1][col] = 'p';
							cCB->board[row][col] = '-';
						}

						if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
							return;

						memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
					}
					else if (cCB->board[row][col] == 'n') {
						int moves[] = { 2, -2, 1, -1 };

						for (int i = 0; i < 4; i++) {
							for (int j = 0; j < 4; j++) {

								if ((row + moves[i]) < 0 || (row + moves[i]) > 8 || (col + moves[j]) < 0 || (col + moves[j]) > 8)
									continue;

								cCB->board[row + moves[i]][col + moves[j]] = 'n';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'b') {
						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col + i) > 7)
								break;

							if (cCB->board[row + i][col + i] == '-' || i == 7) {

								cCB->board[row + i][col + i] = 'b';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col - i) < 0)
								break;

							if (cCB->board[row + i][col - i] == '-' || i == 7) {

								cCB->board[row + i][col - i] = 'b';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									break;
								else
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row - i) < 0 || (col - i) < 0)
								break;

							if (!(cCB->board[row - i][col - i] == '-')) {
								cCB->board[row - i][col - i] = 'b';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;
							}

							else if (cCB->board[row - i][col - i] == '-' || i == 7) {

								cCB->board[row - i][col - i] = 'b';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'r') {
						for (int i = 1; i < 8; i++) {
							if (!(cCB->board[row + i][col] == '-') || row + i > 7)
								break;

							if (cCB->board[row + i][col] == '-' || i == 7) {

								cCB->board[row + i][col] = 'r';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
					else if (cCB->board[row][col] == 'q') {
						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col + i) > 7)
								break;

							if (cCB->board[row + i][col + i] == '-' || i == 7) {

								cCB->board[row + i][col + i] = 'q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row + i) > 7 || (col - i) < 0)
								break;

							if (cCB->board[row + i][col - i] == '-' || i == 7) {

								cCB->board[row + i][col - i] = 'q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}

						for (int i = 1; i < 8; i++) {

							if ((row - i) < 0 || (col - i) < 0)
								break;

							if (!(cCB->board[row - i][col - i] == '-')) {
								cCB->board[row - i][col - i] = 'q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;
							}

							else if (cCB->board[row - i][col - i] == '-' || i == 7) {

								cCB->board[row - i][col - i] = 'q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
						for (int i = 1; i < 8; i++) {
							if (!(cCB->board[row + i][col] == '-') || row + i > 7)
								break;

							if (cCB->board[row + i][col] == '-' || i == 7) {

								cCB->board[row + i][col] = 'q';
								cCB->board[row][col] = '-';

								if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
									return;

								memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
							}
						}
					}
				}
			}
		}
		staleMate = 1;
		end();
	}
}

int enPassant(ChessGame* chessBoard, int lp_rows, int lp_cols, int ap_rows, int ap_cols) {
	if (canEnp.count == 1) {
		if (CB->turn == 0) {
			if (canEnp.one[0] == lp_rows && canEnp.one[1] == lp_cols) {
				if ((ap_rows - lp_rows) == 1 && (ap_cols - lp_cols) == -1) {
					CB->board[ap_cols + 1][ap_rows] = '-';
					return 1;
				}
			}
			else if (canEnp.two[0] == lp_rows && canEnp.two[1] == lp_cols) {
				if ((ap_rows - lp_rows) == -1 && (ap_cols - lp_cols) == -1) {
					CB->board[ap_cols + 1][ap_rows] = '-';
					return 1;
				}
			}
			else
				return 0;
		}
		else if (CB->turn == 1) {
			if (canEnp.one[0] == lp_rows && canEnp.one[1] == lp_cols) {
				if ((ap_rows - lp_rows) == 1 && (ap_cols - lp_cols) == 1) {
					CB->board[ap_cols - 1][ap_rows] = '-';
					return 1;
				}
			}
			else if (canEnp.two[0] == lp_rows && canEnp.two[1] == lp_cols) {
				if ((ap_rows - lp_rows) == -1 && (ap_cols - lp_cols) == 1) {
					CB->board[ap_cols - 1][ap_rows] = '-';
					return 1;
				}
			}
		}
	}
	return 0;
}

void enPassantTrigger(int i, int j, int lp_rows, int lp_cols, char arrivePoint) {
	if (lp_cols == 6) {
		if (j == 2 && i == 0 && arrivePoint == '-') {
			if (CB->board[lp_cols - 2][lp_rows - 1] == 'p' && (CB->board[lp_cols - 2][lp_rows + 1] == 'p')) {
				canEnp.one[1] = lp_cols - 2;
				canEnp.one[0] = lp_rows - 1;
				canEnp.two[1] = lp_cols - 2;
				canEnp.two[0] = lp_rows + 1;
				canEnp.count = 2;
			}
			else if (CB->board[lp_cols - 2][lp_rows - 1] == 'p') {
				canEnp.one[1] = lp_cols - 2;
				canEnp.one[0] = lp_rows - 1;
				canEnp.count = 2;
			}
			else if (CB->board[lp_cols - 2][lp_rows + 1] == 'p') {
				canEnp.two[1] = lp_cols - 2;
				canEnp.two[0] = lp_rows + 1;
				canEnp.count = 2;
			}
		}
	}
	else if (lp_cols == 1) {
		if (j == -2 && i == 0 && arrivePoint == '-') {
			if (CB->board[lp_cols + 2][lp_rows - 1] == 'P' && (CB->board[lp_cols + 2][lp_rows + 1] == 'P')) {
				canEnp.one[1] = lp_cols + 2;
				canEnp.one[0] = lp_rows - 1;
				canEnp.two[1] = lp_cols + 2;
				canEnp.two[0] = lp_rows + 1;
				canEnp.count = 2;
			}
			else if (CB->board[lp_cols + 2][lp_rows - 1] == 'P') {
				canEnp.one[1] = lp_cols + 2;
				canEnp.one[0] = lp_rows - 1;
				canEnp.count = 2;
			}
			else if (CB->board[lp_cols + 2][lp_rows + 1] == 'P') {
				canEnp.two[1] = lp_cols + 2;
				canEnp.two[0] = lp_rows + 1;
				canEnp.count = 2;
			}
		}
	}
}

void Promotion(int ap_rows, int ap_cols) {
	char P;
	int bool = 1;
	if (ap_cols == 0) {
		do {
			system("cls");
			PrintBoard();
			printf("\n\n프로모션할 기물을 선택하세요( N, B, R, Q ): ");
			while (getchar() != '\n');
			scanf("%c", &P);
			if (P == 'N' || P == 'B' || P == 'R' || P == 'Q')
				bool = 0;
		} while (bool);
		CB->board[ap_cols][ap_rows] = P;
	}
	else if (ap_cols == 7) {
		if (ap_cols == 0) {
			do {
				system("cls");
				PrintBoard();
				printf("\n\n프로모션할 기물을 선택하세요( n, b, r, q ): ");
				while (getchar() != '\n');
				scanf("%c", &P);
				if (P == 'n' || P == 'b' || P == 'r' || P == 'q')
					bool = 0;
			} while (bool);
			CB->board[ap_cols][ap_rows] = P;
		}
	}
}

int castling(int direction) {
	if (Wcastling[0] == 0 && direction == 1 && CB->board[7][1] == '-' && CB->board[7][2] == '-' && CB->board[7][3] == '-') {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		for (int i = 1; i < 4; i++) {
			if (CheckMate(cCB, i, 7))
				return 0;
		}

		CB->board[7][0] = '-';
		CB->board[7][2] = 'K';
		CB->board[7][3] = 'R';
		CB->board[7][4] = '-';
	}

	else if (Wcastling[1] == 0 && direction == 2 && CB->board[7][5] == '-' && CB->board[7][6] == '-') {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		if (CheckMate(cCB, 5, 7) && CheckMate(cCB, 6, 7)) {
			return 0;
		}

		CB->board[7][4] = '-';
		CB->board[7][5] = 'R';
		CB->board[7][6] = 'K';
		CB->board[7][7] = '-';
	}

	else if (Bcastling[0] == 0 && direction == 3 && CB->board[0][1] == '-' && CB->board[0][2] == '-' && CB->board[0][3] == '-') {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		for (int i = 1; i < 4; i++) {
			if (CheckMate(cCB, i, 0))
				return 0;
		}

		CB->board[0][0] = '-';
		CB->board[0][2] = 'k';
		CB->board[0][3] = 'r';
		CB->board[0][4] = '-';
	}

	else if (Bcastling[1] == 0 && direction == 4 && CB->board[0][5] == '-' && CB->board[0][6] == '-') {
		memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		ChessGame* cCB = &copyChessBoard;

		if (CheckMate(cCB, 5, 0) && CheckMate(cCB, 6, 0)) {
			return 0;
		}

		CB->board[0][4] = '-';
		CB->board[0][5] = 'r';
		CB->board[0][6] = 'k';
		CB->board[0][7] = '-';
	}

	else
		return 0;
}

int defence(char piece, int row, int col) {
	int mate;
	if (piece == 'P')
		mate = defenceP(piece, row, col);			// 체크 상태면 1
	else if (piece == 'p')
		mate = defencep(piece, row, col);
	else if (piece == 'N')
		mate = defenceN(piece, row, col);
	else if (piece == 'n')
		mate = defencen(piece, row, col);
	else if (piece == 'B')
		mate = defenceB(piece, row, col);
	else if (piece == 'b')
		mate = defenceb(piece, row, col);
	else if (piece == 'R')
		mate = defenceR(piece, row, col);
	else if (piece == 'r')
		mate = defencer(piece, row, col);
	else if (piece == 'Q')
		mate = defenceQ(piece, row, col);
	else if (piece == 'q')
		mate = defenceq(piece, row, col);
	else if (piece == 'K')
		mate = defenceK(piece, row, col);
	else if (piece == 'k')
		mate = defencek(piece, row, col);

	return mate;
}

int defenceP(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;
	int possible;									// 1이면 체크메이트라는 뜻

	possible = enPassant(cCB, col, row, col - 1, row - 1);
	if (possible == 1) {
		cCB->board[row - 1][col - 1] = 'P';
		cCB->board[row][col] = '-';
	}
	else
		possible = enPassant(cCB, col, row, col + 1, row - 1);
	if (possible == 1) {
		cCB->board[row - 1][col + 1] = 'P';
		cCB->board[row][col] = '-';
	}

	if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
		return 1;

	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));

	if (possible == 0 && cCB->board[row - 1][col] == '-') {
		copyChessBoard = chessBoard;

		cCB->board[row - 1][col] = 'P';
		cCB->board[row][col] = '-';
	}

	if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
		return 1;

	return 0;
}

int defencep(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;
	int possible;									// 1이면 체크메이트라는 뜻

	possible = enPassant(cCB, col, row, col - 1, row - 1);
	if (possible == 1) {
		cCB->board[row - 1][col - 1] = 'p';
		cCB->board[row][col] = '-';
	}
	else
		possible = enPassant(cCB, col, row, col + 1, row - 1);
	if (possible == 1) {
		cCB->board[row - 1][col + 1] = 'p';
		cCB->board[row][col] = '-';
	}

	if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
		return 1;

	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));

	if (possible == 0 && cCB->board[row - 1][col] == '-') {
		copyChessBoard = chessBoard;

		cCB->board[row - 1][col] = 'p';
		cCB->board[row][col] = '-';
	}

	if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
		return 1;

	return 0;
}

int defenceN(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	int moves[] = { 2, -2, 1, -1 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			if ((row + moves[i]) < 0 || (row + moves[i]) > 8 || (col + moves[j]) < 0 || (col + moves[j]) > 8)
				continue;

			cCB->board[row + moves[i]][col + moves[j]] = 'N';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defencen(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	int moves[] = { 2, -2, 1, -1 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			if ((row + moves[i]) < 0 || (row + moves[i]) > 8 || (col + moves[j]) < 0 || (col + moves[j]) > 8)
				continue;

			cCB->board[row + moves[i]][col + moves[j]] = 'n';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defenceB(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	for (int i = 1; i < 8; i++) {

		if ((row + i) > 7 || (col + i) > 7)
			break;

		if (cCB->board[row + i][col + i] == '-' || i == 7) {

			cCB->board[row + i][col + i] = 'B';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row + i) > 7 || (col - i) < 0)
			break;

		if (cCB->board[row + i][col - i] == '-' || i == 7) {

			cCB->board[row + i][col - i] = 'B';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row - i) < 0 || (col + i) > 7)
			break;

		if (cCB->board[row - i][col + i] == '-' || i == 7) {

			cCB->board[row - i][col + i] = 'B';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row - i) < 0 || (col - i) < 0)
			break;

		if (!(cCB->board[row - i][col - i] == '-')) {
			cCB->board[row - i][col - i] = 'B';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;
		}

		else if (cCB->board[row - i][col - i] == '-' || i == 7) {

			cCB->board[row - i][col - i] = 'B';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defenceb(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	for (int i = 1; i < 8; i++) {

		if ((row + i) > 7 || (col + i) > 7)
			break;

		if (cCB->board[row + i][col + i] == '-' || i == 7) {

			cCB->board[row + i][col + i] = 'b';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row + i) > 7 || (col - i) < 0)
			break;

		if (cCB->board[row + i][col - i] == '-' || i == 7) {

			cCB->board[row + i][col - i] = 'b';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row - i) < 0 || (col + i) > 7)
			break;

		if (cCB->board[row - i][col + i] == '-' || i == 7) {

			cCB->board[row - i][col + i] = 'b';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 1; i < 8; i++) {

		if ((row - i) < 0 || (col - i) < 0)
			break;

		if (cCB->board[row - i][col - i] == '-' || i == 7) {

			cCB->board[row - i][col - i] = 'b';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defenceR(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	for (int i = 1; i < 8; i++) {
		if (!(cCB->board[row + i][col] == '-') || row + i > 7)
			break;

		if (cCB->board[row + i][col] == '-' || i == 7) {

			cCB->board[row + i][col] = 'R';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row - i][col] == '-') || row - i < 0)
			break;

		if (cCB->board[row - i][col] == '-' || i == 7) {

			cCB->board[row - i][col] = 'R';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row][col + i] == '-') || col + i > 7)
			break;

		if (cCB->board[row][col + i] == '-' || i == 7) {

			cCB->board[row][col + i] = 'R';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row][col - i] == '-') || col - i < 0)
			break;

		if (cCB->board[row][col - i] == '-' || i == 7) {

			cCB->board[row][col - i] = 'R';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defencer(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row + i][col] == '-') || (row + i) > 7)
			break;

		if (cCB->board[row + i][col] == '-' || i == 7) {

			cCB->board[row + i][col] = 'r';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row - i][col] == '-') || (row - i) < 0)
			break;

		if (cCB->board[row - i][col] == '-' || i == 7) {

			cCB->board[row - i][col] = 'r';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row][col + i] == '-') || (col + i) > 7)
			break;

		if (cCB->board[row][col + i] == '-' || i == 7) {

			cCB->board[row][col + i] = 'r';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	for (int i = 0; i < 8; i++) {
		if (!(cCB->board[row][col - i] == '-') || (col + i) < 0)
			break;

		if (cCB->board[row][col - i] == '-' || i == 7) {

			cCB->board[row][col - i] = 'r';
			cCB->board[row][col] = '-';

			if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
				return 1;

			memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
		}
	}

	return 0;
}

int defenceQ(char piece, int row, int col) {
	if (defenceB(piece, row, col))
		return 1;
	else if (defenceR(piece, row, col))
		return 1;
	else
		return 0;
}

int defenceq(char piece, int row, int col) {
	if (defenceb(piece, row, col))
		return 1;
	else if (defencer(piece, row, col))
		return 1;
	else
		return 0;
}

int defenceK(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	int moves[] = { -1, 0, 1 };

	if (piece == 'K') {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (row + moves[i] < 0 || row + moves[i] > 7 || col + moves[j] < 0 || col + moves[j] > 7)
					continue;

				cCB->board[row + moves[i]][col + moves[j]] = 'K';
				cCB->board[row][col] = '-';

				if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
					return 1;

				memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
			}
		}
	}
}

int defencek(char piece, int row, int col) {
	memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
	ChessGame* cCB = &copyChessBoard;				// 1이면 체크메이트라는 뜻

	int moves[] = { -1, 0, 1 };

	if (piece == 'K') {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (row + moves[i] < 0 || row + moves[i] > 7 || col + moves[j] < 0 || col + moves[j] > 7)
					continue;

				cCB->board[row + moves[i]][col + moves[j]] = 'K';
				cCB->board[row][col] = '-';

				if (!(CheckMate(cCB, 0, 0)))					// 체크 상태면 1
					return 1;

				memcpy(&copyChessBoard, &chessBoard, sizeof(ChessGame));
			}
		}
	}
}

void end() {
	isRunning = 0;
}

int moveP(int i, int j, int lp_cols, char arrivePoint) {
	if (lp_cols == 6) {
		if (j == 2 && i == 0 && arrivePoint == '-')
			return 1;
	}
	if (j == 1) {
		if (i == 0 && arrivePoint == '-') {
			return 1;
		}
		else if (i == -1) {
			if (!(arrivePoint == '-'))
				return 1;
		}
		else if (i == 1) {
			if (!(arrivePoint == '-'))
				return 1;
		}
		else
			return 0;
	}
	return 0;
}
int movep(int i, int j, int lp_cols, char arrivePoint) {
	if (lp_cols == 1) {
		if (j == -2 && i == 0 && arrivePoint == '-')
			return 1;
	}
	if (j == -1) {
		if (i == 0 && arrivePoint == '-') {
			return 1;
		}
		else if (i == -1) {
			if (!(arrivePoint == '-'))
				return 1;
		}
		else if (i == 1) {
			if (!(arrivePoint == '-'))
				return 1;
		}
		else
			return 0;
	}
	return 0;
}
int moveN(int i, int j) {
	if ((i == 2 && (j == -1 || j == 1)) ||
		(i == -2 && (j == -1 || j == 1)) ||
		(j == 2 && (i == -1 || i == 1)) ||
		(j == -2 && (i == -1 || i == 1))) {
		return 1;
	}
	return 0;
}
int moven(int i, int j) {
	if ((i == 2 && (j == -1 || j == 1)) ||
		(i == -2 && (j == -1 || j == 1)) ||
		(j == 2 && (i == -1 || i == 1)) ||
		(j == -2 && (i == -1 || i == 1))) {
		return 1;
	}
	return 0;
}
int moveB(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	int ap_rows;

	if (!(abs(i) == abs(j)))
		return 0;
	if (i > 0 && j > 0) {
		ap_rows = lp_rows - i;
		for (i; i > 1; i--) {
			if (lp_rows > ap_rows) {
				lp_rows -= 1;
				lp_cols -= 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i > 0 && j < 0) {
		ap_rows = lp_rows - i;
		for (i; i > 1; i--) {
			if (lp_rows > ap_rows) {
				lp_rows -= 1;
				lp_cols += 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i < 0 && j > 0) {
		ap_rows = lp_rows + i;
		for (i; i < 1; i++) {

			if (lp_rows < ap_rows) {
				lp_rows += 1;
				lp_cols -= 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i < 0 && j < 0) {
		ap_rows = lp_rows + i;
		for (i; i < 1; i++) {
			if (lp_rows < ap_rows) {
				lp_rows += 1;
				lp_cols += 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	return 1;
}
int moveb(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	int ap_rows;

	if (!(abs(i) == abs(j)))
		return 0;
	if (i > 0 && j > 0) {
		ap_rows = lp_rows - i;
		for (i; i > 1; i--) {
			if (lp_rows > ap_rows) {
				lp_rows -= 1;
				lp_cols -= 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i > 0 && j < 0) {
		ap_rows = lp_rows - i;
		for (i; i > 1; i--) {
			if (lp_rows > ap_rows) {
				lp_rows -= 1;
				lp_cols += 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i < 0 && j > 0) {
		ap_rows = lp_rows + i;
		for (i; i < 1; i++) {

			if (lp_rows < ap_rows) {
				lp_rows += 1;
				lp_cols -= 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (i < 0 && j < 0) {
		ap_rows = lp_rows + i;
		for (i; i < 1; i++) {
			if (lp_rows < ap_rows) {
				lp_rows += 1;
				lp_cols += 1;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	return 1;
}
int moveR(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	if (!(i == 0 || j == 0))
		return 0;
	if (i == 0) {
		if (j > 0) {
			for (j; j > 1; j--) {
				lp_cols--;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
		else {
			for (j; j < 0; j++) {
				lp_cols++;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (j == 0) {
		if (i > 0) {
			for (i; i > 1; i--) {
				lp_rows--;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
		else {
			for (i; i < 0; i++) {
				lp_rows++;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	return 1;
}
int mover(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	if (!(i == 0 || j == 0))
		return 0;
	if (i == 0) {
		if (j > 0) {
			for (j; j > 1; j--) {
				lp_cols--;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
		else {
			for (j; j < 0; j++) {
				lp_cols++;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	else if (j == 0) {
		if (i > 0) {
			for (i; i > 1; i--) {
				lp_rows--;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
		else {
			for (i; i < 0; i++) {
				lp_rows++;
				if (!(CB->board[lp_cols][lp_rows] == '-'))
					return 0;
			}
		}
	}
	return 1;
}
int moveQ(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	int possible = moveB(arrivePoint, i, j, lp_rows, lp_cols);
	if (possible == 0)
		possible = moveR(arrivePoint, i, j, lp_rows, lp_cols);
	return possible;
}
int moveq(char arrivePoint, int i, int j, int lp_rows, int lp_cols) {
	int possible = moveb(arrivePoint, i, j, lp_rows, lp_cols);
	if (possible == 0)
		possible = mover(arrivePoint, i, j, lp_rows, lp_cols);
	return possible;
}
int moveK(int i, int j) {
	if (abs(i) == 0 || abs(i) == 1) {
		if (abs(j) == 0 || abs(j) == 1) {
			Wcastling[0] = 1;
			Wcastling[1] = 1;
			return 1;
		}
	}
	Wcastling[0] = 1;
	Wcastling[1] = 1;

	return 0;
}
int movek(int i, int j) {
	if (abs(i) == 0 || abs(i) == 1) {
		Bcastling[0] = 1;
		Bcastling[1] = 1;
		return 1;
	}
	Bcastling[0] = 1;
	Bcastling[1] = 1;

	return 0;
}