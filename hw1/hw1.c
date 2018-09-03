#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'


char dungeon[ROW][COL];

/*
struct object
{
	char *type;
}

int initDungeon (int row, int col)
{
	//struct object Dungeon[row][col];
	for(int i=0; i<row; i++)
	{
		for(int j=0; j<col; j++)
		{
		}
	}
}
*/

void generateRoom()
{
	int x = rand() % 80;
	int y = rand() % 21;
	int height = rand() % 3 + 2;
	int width = rand() % 4 + 3;

	//roomCheck();
	for (int i=x; i<x+width; i++)
	{
		for (int j=y; j<y+height; j++)
		{
			dungeon[i][j] = '.';
		}
	}

	return;
}

void printDungeon()
{
	for (int i=0; i<ROW+2; i++)
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
	for (int i=0; i<ROW+2; i++)
	{
		printf("-");
	}
	printf("\n");
}



int main(int argc, char *argv[])
{
	int seed = time(NULL);
	srand(seed);

	//initial dungeon
	//initDungeon(int ROW, int COL);

	generateRoom();
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
