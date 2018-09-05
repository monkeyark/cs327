#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define ROW 21
#define COL 80
#define ROCK 'X'
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

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col] != ROCK) return false;
	//is room on edge or oversize
	if (row == 0 || col == 0 || row+height >= ROW || col+width >= COL) return false;
	//is touching on left and right
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1] != ROCK) return false; //left
		if (dungeon[i][col+width+1] != ROCK) return false;//right
	}
	//is touching on top and bottom
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
	r.width = getRandom(7, 3);
	r.height = getRandom(6, 2);

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);
	printf("      row=%2d   col=%2d   width=%2d   height=%2d", r.row, r.col, r.width, r.height);
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

float distance (int aRow, int aCol, int bRow, int bCol)
{
	float row = abs(aRow - bRow);
	float col = abs(bCol - bCol);

	return sqrt(row*row + col*col);
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
}

int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	//set up random
	int seed = time(NULL);
	srand(seed);
	printf("seed = %d;\n", seed);

	//generate random number of rooms
	int n = getRandom(7, 5);
	Room rooms[n];
	for (int i=0; i<n; i++)
	{
		rooms[i] = newRoom();
		//printf("ROOM  row=%2d   col=%2d   width=%2d   height=%2d\n\n", rooms[i].row, rooms[i].col, rooms[i].width, rooms[i].height);
	}

	for (int i=0; i<n-1; i++)//test loop
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
