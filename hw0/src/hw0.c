#include <stdio.h>
#include "stdbool.h"
#define N 5


int board[N][N];
bool isVisit[N][N];
int solu[N*N];

bool isMove(int row, int col, int tour)
{
	return row >= 0 && row < N
		&& col >= 0 && col < N
		&& !isVisit[row][col]
		&& tour < N*N;
}

int soluPath(int solu[])
{
	//TODO
	return 0;
}

/*
tour from top-left in clockwise direction
move       0   1   2   3   4   5   6   7
row       -2  -2  -1  +1  +2  +2  +1  -1
col       -1  +1  +2  +2  +1  -1  -2  -2
*/
bool knightTour(int row, int col, int move, int tour)
{
	int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
	int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

	isVisit[row][col] = true;

	if (tour == N*N-1) return true;

	for (int i=0; i<8; i++)
	{
		row += rowMove[i];
		col += colMove[i];
		tour++;

		if (isMove(row, col, tour))
		{
			if (knightTour(row, col, i, tour))
			{
				solu[tour] = board[row][col];
				//TODO add to soluPath
				return true;
			}
			//TODO backtrack
			row -= rowMove[i];
			col -= colMove[i];

//			knightTour(row, col, move++, tour--);
		}
	}

	isVisit[row][col] = false;
	return false;
}

/*
main()
{
  For each square s
    tour(s)
}

tour(start)
{
  mark start as visited
  for each unmarked square s reachable by a knight's move from start
    tour(s)
  unmark start
}

And you'll need to do a little bit of bookkeeping to record your move orders so that you can print.
*/

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
			knightTour(row, col, 0, 0);
		}
	}

	//TODO print(soluPath)


	return 0;
}
