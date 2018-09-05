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
	printf("   ");
	for (int i=0; i<COL; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i=0; i<ROW; i++)
	{
		printf("%2d|", i);
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
	return row > 0 || col > 0 || row < ROW || col < COL;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col] != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//is touching another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1] != ROCK) return false; //left
		if (dungeon[i][col+width+1] != ROCK) return false;//right
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j] != ROCK) return false;//top
		if (dungeon[row+height+1][j] != ROCK) return false;//bottom
	}
	return true;
}

Room newRoom()
{
	Room r;
	r.row = getRandom(ROW, 0);
	r.col = getRandom(COL, 0);

	//TODO test case
//	r.width = 1;
//	r.height = 1;

	r.width = getRandom(7, 3);
	r.height = getRandom(6, 2);

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);
	printf("        row=%2d   col=%2d   width=%2d   height=%2d", r.row, r.col, r.width, r.height);
	printf("   %s\n", validRoom?"true":"false");

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

float distance(int aRow, int aCol, int bRow, int bCol)
{
	float row = abs(aRow - bRow);
	float col = abs(bCol - bCol);

	return sqrt(row*row + col*col);
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
//	float max = distance(0, 0, ROW, COL);
//	float min = max;
//	float top = max;
//	float down = max;
//	float left = max;
//	float right = max;
//
//	if (isInside(aRow-1, aCol))
//	{
//		top = (aRow-1, aCol, bRow, bCol);
//		if (top < min) min = top;
//	}
//	if (isInside(aRow+1, aCol))
//	{
//		down = (aRow+1, aCol, bRow, bCol);
//		if (down < min) min = down;
//	}
//	if (isInside(aRow, aCol-1))
//	{
//		left = (aRow, aCol-1, bRow, bCol);
//		if (left < min) min = left;
//	}
//	if (isInside(aRow, aCol+1))
//	{
//		right = (aRow, aCol+1, bRow, bCol);
//		if (right < min) min = right;
//	}
//
//	if (min == top)
//	{
//		dungeon[aRow-1][aCol] = CORRIDOR;
//		newCorridor(aRow-1, aCol, bRow, bCol);
//	}
//	else if (min == down)
//	{
//		dungeon[aRow+1][aCol] = CORRIDOR;
//		newCorridor(aRow+1, aCol, bRow, bCol);
//	}
//	else if (min == left)
//	{
//		dungeon[aRow][aCol-1] = CORRIDOR;
//		newCorridor(aRow-1, aCol-1, bRow, bCol);
//	}
//	else if (min == right)
//	{
//		dungeon[aRow][aCol+1] = CORRIDOR;
//		newCorridor(aRow-1, aCol+1, bRow, bCol);
//	}

	if (aRow <= bRow) 	//now at dungeon[aRow][aCol]
	{
		for (int i=aRow; i<=bRow; i++)
		{
			if (dungeon[i][aCol] == ROCK)
			{
				dungeon[i][aCol] = CORRIDOR;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[bRow][aCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[bRow][i] == ROCK)
				{
					dungeon[bRow][i] = CORRIDOR;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[bRow][i] == ROCK)
				{
					dungeon[bRow][i] = CORRIDOR;
				}
			}
		}
	}
	else
	{
		for (int i=bRow; i<=aRow; i++)
		{
			if (dungeon[i][bCol] == ROCK)
			{
				dungeon[i][bCol] = CORRIDOR;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[aRow][bCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[aRow][i] == ROCK)
				{
					dungeon[aRow][i] = CORRIDOR;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[aRow][i] == ROCK)
				{
					dungeon[aRow][i] = CORRIDOR;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	//set up random
	int seed = time(NULL);
	//seed = 1536125881;
	srand(seed);
	printf("seed = %d;\n", seed);

	//generate random number of rooms
	int n = getRandom(7, 5);
	Room rooms[n];
	for (int i=0; i<n; i++)
	{
		rooms[i] = newRoom();
	}

	for (int i=0; i<n; i++)//test loop
	{
		printf("ROOM%2d  row=%2d   col=%2d   width=%2d   height=%2d\n", i, rooms[i].row, rooms[i].col, rooms[i].width, rooms[i].height);
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(rooms[i].row, rooms[i].col, rooms[i+1].row, rooms[i+1].col);
		//printDungeon();
	}

	printDungeon();

	return 0;
}
