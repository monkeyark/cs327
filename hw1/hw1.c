#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'


char dungeon[ROW][COL];

typedef struct dungeonRoom
{
	int row;
	int col;
	int width;
	int height;
} Room;


void initDungeon()
{
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			dungeon[i][j] = ROCK;
		}
	}
}


int getRandom(int modulus, int min)
{
	return rand() % modulus + min;
}

void printDungeon()
{
	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i=0; i<ROW; i++)
	{
		printf("|");
		for (int j=0; j<COL; j++)
		{
			printf("%c", dungeon[i][j]);
		}
		printf("|\n");
	}

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");
}

bool isInside (int row, int col)
{
	//is room not on edge or outside of dungeon
	return row > 0 && col > 0 && row < ROW && col < COL;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col] != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//touch or overlap another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1] != ROCK) return false;//left touch
		if (dungeon[i][col] != ROCK) return false;//left overlap
		if (dungeon[i][col+width+1] != ROCK) return false;//right touch
		if (dungeon[i][col+width] != ROCK) return false;//right overlap
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j] != ROCK) return false;//top touch
		if (dungeon[row][j] != ROCK) return false;//top overlap
		if (dungeon[row+height+1][j] != ROCK) return false;//bottom touch
		if (dungeon[row+height][j] != ROCK) return false;//bottom overlap
	}

	return true;
}

Room newRoom()
{
	Room r;
	r.row = getRandom(ROW, 0);
	r.col = getRandom(COL, 0);
	r.width = getRandom(7, 3);
	r.height = getRandom(6, 2);

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);
	if (validRoom)
	{
		for (int i=r.row; i<r.row+r.height; i++)
		{
			for (int j=r.col; j<r.col+r.width; j++)
			{
				dungeon[i][j] = ROOM;
			}
		}
	}
	else
	{
		return newRoom();
	}

	return r;
}

int distance(int aRow, int aCol, int bRow, int bCol)
{
	int row = abs(aRow - bRow);
	int col = abs(aCol - bCol);

	return row*row + col*col;
}

int minimum(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

bool isConnected(int row, int col)
{
	return dungeon[row-1][col] == CORRIDOR
		|| dungeon[row+1][col] == CORRIDOR
		|| dungeon[row][col-1] == CORRIDOR
		|| dungeon[row][col+1] == CORRIDOR;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0) return;
	if (dungeon[aRow][aCol] == CORRIDOR && isConnected(bRow, bCol)) return;
	if (dungeon[aRow][aCol] == ROCK) dungeon[aRow][aCol] = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (isInside(aRow-1, aCol))
	{
		top = distance(aRow-1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (isInside(aRow+1, aCol))
	{
		down = distance(aRow+1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (isInside(aRow, aCol-1))
	{
		left = distance(aRow, aCol-1, bRow, bCol);
		min = minimum(min, left);
	}
	if (isInside(aRow, aCol+1))
	{
		right = distance(aRow, aCol+1, bRow, bCol);
		min = minimum(min, right);
	}

	if (min == top)
	{
		newCorridor(aRow-1, aCol, bRow, bCol);

	}
	else if (min == down)
	{
		newCorridor(aRow+1, aCol, bRow, bCol);

	}
	else if (min == left)
	{
		newCorridor(aRow, aCol-1, bRow, bCol);
	}
	else if (min == right)
	{
		newCorridor(aRow, aCol+1, bRow, bCol);
	}
}

int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	//set up random
	int seed = time(NULL);
	seed = 1536656664;
	//seed = 1536656798; seed = 1536831996;
	//seed = 1536657024; seed = 1536657138; seed = 1536807801;

	srand(seed);

	//generate random number of rooms
	int n = getRandom(7, 5);
	Room rooms[n];
	for (int i=0; i<n; i++)
	{
		rooms[i] = newRoom();
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(rooms[i].row, rooms[i].col, rooms[i+1].row, rooms[i+1].col);
	}

	printDungeon();

	return 0;
}
