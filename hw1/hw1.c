#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'


//char *dungeon[ROW][COL];

struct object
{
	char *type;
}

int initDungeon (int row, int col)
{
	struct object Dungeon[row][col];
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
			Dungeon[row][col].type = "ROCK";
		}
	}
}

int main(int argc, char *argv[])
{
	int seed = time(NULL);
	srand(time(seed));

	//initial dungeon
	initDungeon(int ROW, int COL;


	//generate random number of room
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

	return 0;
}

