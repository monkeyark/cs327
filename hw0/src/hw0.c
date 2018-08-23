#include <stdio.h>
#define N 5

int main()
{
  initialBoard();

  /*
    move from top-left in clockwise direction
    move   1   2   3   4   5   6   7   8
    row   -2  -2  -1  +1  +2  +2  +1  -1
    col   -1  +1  +2  +2  +1  -1  -2  -2
   */
  


  return 0;
}

int move(int n)
{
  /*
    move from top-left in clockwise direction
    move   1   2   3   4   5   6   7   8
    row   -2  -2  -1  +1  +2  +2  +1  -1
    col   -1  +1  +2  +2  +1  -1  -2  -2
   */

  int rowMove[8] = {-2,  -2,  -1,  +1,  +2,  +2,  +1,  -1};
  int colMove[8] = {-1,  +1,  +2,  +2,  +1,  -1,  -2,  -2};

  
  return 0;
}


void solveKT()
{
  int board[N][N];

  /* initializing the board */
  int number = 1;
  for (int row=0; row<N; row++)
    for (int col=0; col<N; col++)
      board[row][col] = number++;
}

