#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define ROW 21
#define COL 80
#define ROCK 'X'
#define ROOM '.'
#define CORRIDOR '#'

/*
- Dungeon measures 80 units in the x (horizontal) direction and 21 units in the y (vertical) direction. A
standard terminal is 80 × 24, and limiting the dungeon to 21 rows leaves three rows for text, things
like gameplay messages and player status, which come later.
- Require at least 5 rooms per dungeon
- Each room measures at least 3 units in the x direction and at least 2 units in the y direction.
- Rooms need not be rectangular, but neither may they contact one another. There must be at least 1
cell of non-room between any two different rooms.
- The outermost cells of the dungeon are immutable, thus they must remain rock and cannot be part of
any room or corridor.
- Room cells should be drawn with periods, corridor cells with hashes, and rock with spaces.
- The dungeon should be fully connected, meaning that from any position on the floor, your adventurer
should be able to walk to any other position on the floor without passing through rock.
- Corridors should not extend into rooms, e.g., no hashes should be rendered inside rooms.
*/

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
	//TODO need to check room touching
	if (dungeon[row][col] == ROCK)
	{
		if (row+height <= ROW && col+width <= COL)
		{
			return true;
		}
	}
	return false;
}

void newRoom(int seed)
{
	srand(seed);

	int row = rand() % ROW;
	int col = rand() % COL;
	int width = rand() % 4 + 3;
	int height = rand() % 5 + 2;

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
		newRoom(seed + 1);
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

void generateRoom(int min, int seed)
{
	//randomly generate rooms over min
	int total = rand() % 3 + min;
	for (int i=0; i<total; i++)
	{
		newRoom(seed);
	}
}

int main(int argc, char *argv[])
{
	//initial dungeon
	initDungeon();

	int seed = time(NULL);
	//seed = 1535978302;
	//newRoom(seed);
	int min = 5;

	generateRoom(min, seed);

	printDungeon();

	return 0;
}
