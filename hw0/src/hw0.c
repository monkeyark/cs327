#include <stdio.h>
#include <stdlib.h>
#define N 5

int main(void)
{
  int board[N][N];

  /* initializing the board */
  int number = 1;
  for (int row=0; row<N; row++)
    for (int col=0; col<N; col++)
      board[row][col] = number++;

  /*
    move from top-left in clockwise direction
    move   1   2   3   4   5   6   7   8
    row   -2  -2  -1  +1  +2  +2  +1  -1
    col   -1  +1  +2  +2  +1  -1  -2  -2
   */


  return EXIT_SUCCESS;
}
