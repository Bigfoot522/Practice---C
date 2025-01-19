#include "main.h"

// 주피티 추가 예정

int main() {
	char leavePoint[3], arrivePoint[3];

	isRunning = 1;

	while (isRunning) {
		PrintBoard();
		scanf("%2s %2s", &leavePoint, &arrivePoint);
		while (getchar() != '\n');
		PieceMove(leavePoint, arrivePoint);

		system("cls");
	}
	if (staleMate == 1) {
		printf("스테일메이트! 무승부입니다.");
	}
	else if (CB->turn == 0) {
		PrintBoard();
		printf("\n\n 흑 승! 축하드립니다.");
	}
	else {
		PrintBoard();
		printf("\n\n 백 승!축하드립니다.");
	}

	system("pause");
	return 0;
}

// idea : e2 d4 뭐 이런 좌표값을 그냥 숫자로 바꾸면 좀 더 편하지 않을까? a8 = 1, b8 = 2 이런식으로