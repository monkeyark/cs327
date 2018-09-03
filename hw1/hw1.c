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

/*
struct object
{
	char *type;
}
*/
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

bool roomCheck(int row, int col, int width, int height)
{
	if (dungeon[row][col] == ROCK)
	{
		if (row+height <= ROW && col+width <= COL)
		{
			return true;
		}
	}
	return false;
}

void generateRoom(int seed)
{
	srand(seed);

	int row = rand() % ROW;
	int col = rand() % COL;
	int width = rand() % 4 + 3;
	int height = rand() % 5 + 2;

//	int row = 20;
//	int col = 79;
//	int width = 3;
//	int height = 2;

	bool check = roomCheck(row, col, width, height);
	printf("seed=%d   row=%d   col=%d   width=%d   height=%d",seed, row, col, width, height);
	printf("   %s\n", check?"true":"false");

	if (check)
	{
		for (int i=row; i<row+height; i++)
		{
			for (int j=col; j<col+width; j++)
			{
				dungeon[i][j] = ROOM;
			}
		}
	}
	else
	{
		generateRoom(seed + 1);
	}
}

void printDungeon()
{
	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	printf("   ");
	for (int i=0; i<COL; i++)
	{
		printf("%d", i % 10);
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



int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	int seed = time(NULL);
	//seed = 1535978302;
	generateRoom(seed);

	/*
	//randomly generate rooms over min
	int R = 0;
	while (R < 5)
	{
		R = rand() % 10;
	}

	//generate random room positon
	struct object dungeon[ROW][COL];
	for (int i=0; i<R; i++)
	{
		
		room[i] = rand() % ROW;
	}
	*/

	printDungeon();

	return 0;
}
