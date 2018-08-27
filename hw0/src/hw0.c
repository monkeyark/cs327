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


int main()
{
  /* initializing the board */
  int board[N][N];

  int number = 1;
  for (int row=0; row<N; row++)
    for (int col=0; col<N; col++)
      board[row][col] = number++;

  /* initializing the board record */
  bool record[N][N];
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
    	record[i][j] = false;

  /* for each block on board start tour */
  for (int i=0; i<N; i++)
    for (int j=0; j<N; j++)
      move(i, j);

  /*
    move from top-left in clockwise direction
    move   1   2   3   4   5   6   7   8
    row   -2  -2  -1  +1  +2  +2  +1  -1
    col   -1  +1  +2  +2  +1  -1  -2  -2
   */

  return 0;
}

int move(int row, int col, int numTour)
{
  if (recordBoard[i][j] == true)
    return -1;
  else
    int numMove = numTour++;
  /*
    move from top-left in clockwise direction
    move   1   2   3   4   5   6   7   8
    row   -2  -2  -1  +1  +2  +2  +1  -1
    col   -1  +1  +2  +2  +1  -1  -2  -2
   */
  int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
  int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

  row = row + rowMove[numMove];
  col = col + colMove[numMove];

  move(row, col, numMove);
  return numMove;
}


bool recordBoard(bool record[N][N], int row, int col)
{
  if (record[N][N])
	  return false;
  else

  return true;
}

