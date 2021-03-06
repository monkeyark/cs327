#include <stdio.h>
#include <stdbool.h>
#define N 5


int board[N][N];
bool isVisit[N][N];
int total = 0;

bool isMove(int row, int col)
{
	return row >= 0 && row < N && col >= 0 && col < N;
}

void printSolu(int solu[])
{
	for (int i=0; i<N*N; i++)
	{
		printf("%2d ", solu[i]);
	}
	printf("\n");
	return;
}

/*
tour from top-left in clockwise direction
move       0   1   2   3   4   5   6   7
row       -2  -2  -1  +1  +2  +2  +1  -1
col       -1  +1  +2  +2  +1  -1  -2  -2
*/
void knightTour(int row, int col, int step, int solu[])
{
	int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
	int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

	isVisit[row][col] = true;
	solu[step] = board[row][col];

	if (step >= N*N-1)
	{
		printf("line %d:  ", ++total);
		printSolu(solu);

		isVisit[row][col] = false;
		return;
	}

	//try all 8 posslible knight move

	for (int i=0; i<8; i++)
	{
		int nextRow = row + rowMove[i];
		int nextCol = col + colMove[i];

		//is valid move
		if (isMove(nextRow, nextCol) && !isVisit[nextRow][nextCol])
		{
			knightTour(nextRow, nextCol, step+1, solu);
		}
	}

	isVisit[row][col] = false;
	solu[step] = 0;
}

int main()
{
	//initializing the board and the record
	int solu[N*N];
	int number = 1;
	for (int row=0; row<N; row++)
	{
		for (int col=0; col<N; col++)
		{
			board[row][col] = number++;
			isVisit[row][col] = false;
		}
	}

	for (int i=0; i<N*N; i++)
	{
		solu[i] = 0;
	}

	//for each block on board start tour

	for (int row=0; row<N; row++)
	{
		for (int col=0; col<N; col++)
		{
			knightTour(row, col, 0, solu);
		}
	}

	printf("\ntotal solution: %d\n", total);

	return 0;
}
