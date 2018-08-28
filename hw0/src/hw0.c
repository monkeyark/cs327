#include <stdio.h>
#include "stdbool.h"
#define N 5


int board[N][N];
bool isVisit[N][N];
int solu[N*N];
int numSolu = 0;

bool isMove(int row, int col, int step)
{
	return row >= 0 && row < N
		&& col >= 0 && col < N
		&& !isVisit[row][col]
		&& step < N*N;
}

void printSolu(int solu[])
{
	//for (int i=0; i<(sizeof(solu)/sizeof(solu[0])); i++)

	for (int i=0; i<N*N; i++)
	{
		printf("%d ", solu[i]);
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
bool knightTour(int row, int col, int step)
{
	int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
	int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

	isVisit[row][col] = true;

	if (step == N*N-1) return true;

	for (int i=0; i<8; i++)
	{
		row += rowMove[i];
		col += colMove[i];

		if (isMove(row, col, step))
		{
			//add to soluPath
			solu[step] = board[row][col];
			numSolu++;

			if (knightTour(row, col, step))
			{
				//numSolu++;
				printSolu(solu);
				return true;
			}
			//backtrack
			solu[step--] = 0;
		}
	}

	isVisit[row][col] = false;
	return false;
}

int main()
{
	//initializing the board and the record

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
			knightTour(row, col, 0);
		}
	}

	printf("\ntotal solution: %d\n", numSolu);

	return 0;
}
