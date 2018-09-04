#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define ROW 21
#define COL 80
#define ROCK 'X'
#define ROOM '.'
#define CORRIDOR '#'


char dungeon[ROW][COL];

//struct Room
//{
//	int row;
//	int col;
//	int width;
//	int height;
//};
//
//struct Room *rooms;

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

Room newRoom(int seed)
{
	srand(seed);
	Room r;
	r.row = rand() % ROW;
	r.col = rand() % COL;
	r.width = rand() % 7 + 3;
	r.height = rand() % 6 + 2;

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);
	printf("seed=%d;   row=%2d   col=%2d   width=%2d   height=%2d", seed, r.row, r.col, r.width, r.height);
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
		return newRoom(seed + 1);
	}

	return r;
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

Room * generateRoom(int min, int seed)
{
	//randomly generate rooms over min
	int total = rand() % 6 + min;

	total = 5;
	Room * r;
	r = malloc(total * sizeof(Room));

	for (int i=0; i<total; i++)
	{
		*r = newRoom(seed);
		printf("\nROOM: row=%2d   col=%2d   width=%2d   height=%2d\n", r->row, r->col, r->width, r->height);
		r++;
	}
	r = r - total;

	return r;
}

int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	int seed = time(NULL);
	int min = 5;

	//seed=1536023625; //touch?
	//seed=1536023678; //touch?
	generateRoom(min, seed);

	printDungeon();

	return 0;
}
