#include "main.h"

// ����Ƽ �߰� ����

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
		printf("�����ϸ���Ʈ! ���º��Դϴ�.");
	}
	else if (CB->turn == 0) {
		PrintBoard();
		printf("\n\n �� ��! ���ϵ帳�ϴ�.");
	}
	else {
		PrintBoard();
		printf("\n\n �� ��!���ϵ帳�ϴ�.");
	}

	system("pause");
	return 0;
}

// idea : e2 d4 �� �̷� ��ǥ���� �׳� ���ڷ� �ٲٸ� �� �� ������ ������? a8 = 1, b8 = 2 �̷�������