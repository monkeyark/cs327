#include <stdio.h>
#include "stdbool.h"
#define N 5

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

int board[N][N];
bool isVisit[N][N];
int solu[N*N];
int path[];

int main()
{
	//initializing the board and the record
	
	int number = 1;
	for (int i=0; row<N; row++)
	{
		for (int j=0; col<N; col++)
		{
			board[i][j] = number++;
			isVisit[i][j] = false;
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

	/*
	tour from top-left in clockwise direction
	move       0   1   2   3   4   5   6   7
	row       -2  -2  -1  +1  +2  +2  +1  -1
	col       -1  +1  +2  +2  +1  -1  -2  -2
	*/
	
	//print(soluPath)
	

	return 0;
}

bool knightTour(int row, int col, int move, int tour)
{
	int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
	int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

	if (isSafe(row, col, move, tour))
	{
		isVisit[row][col] = true;
		solu[tour] = board[row][col];
		
		int nextRow = row + rowMove[move];
		int nextCol = col + colMove[move];
		
		if (knightTour(nextRow, nextCol, 0, ++tour))
		{
			//add to soluPath
			return true;
		}
	}
	else //TODO backtrack happens when move is 7 and no knightTour is true
	{
		isVisit[row][col] = false;
		solu[--tour] = 0;
		knightTour(row, col, ++move, tour);
	}
	
	return false;
}

bool isSafe(int row, int col, int move)
{
	return row >= 0 && row <= 4
		&& col >= 0 && col <= 4
		&& !isVisit[row][col]
		&& move >=0 && move <= 7
		&& tour < 25;
}

int soluPath(int solu[])
{
	return 0;
}

