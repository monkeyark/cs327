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


int get_random(int modulus, int min)
{
	return rand() % modulus + min;
}

void print_dungeon()
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

bool is_inside (int row, int col)
{
	//is room not on edge or outside of dungeon
	return row > 0 && col > 0 && row < ROW-1 && col < COL-1;
}

bool is_valid_room(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col] != ROCK) return false;
	if (!is_inside(row, col) || !is_inside(row+height, col+width)) return false;
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

Room new_room()
{
	Room r;
	r.row = get_random(ROW, 0);
	r.col = get_random(COL, 0);
	r.width = get_random(7, 3);
	r.height = get_random(6, 2);

	bool validRoom = is_valid_room(r.row, r.col, r.width, r.height);
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
		return new_room();
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

bool is_connected(int row, int col)
{
	return dungeon[row-1][col] == CORRIDOR
		|| dungeon[row+1][col] == CORRIDOR
		|| dungeon[row][col-1] == CORRIDOR
		|| dungeon[row][col+1] == CORRIDOR;
}

void new_corridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0) return;
	if (dungeon[aRow][aCol] == CORRIDOR && is_connected(bRow, bCol)) return;
	if (dungeon[aRow][aCol] == ROCK) dungeon[aRow][aCol] = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (is_inside(aRow-1, aCol))
	{
		top = distance(aRow-1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (is_inside(aRow+1, aCol))
	{
		down = distance(aRow+1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (is_inside(aRow, aCol-1))
	{
		left = distance(aRow, aCol-1, bRow, bCol);
		min = minimum(min, left);
	}
	if (is_inside(aRow, aCol+1))
	{
		right = distance(aRow, aCol+1, bRow, bCol);
		min = minimum(min, right);
	}

	if (min == top)
	{
		new_corridor(aRow-1, aCol, bRow, bCol);

	}
	else if (min == down)
	{
		new_corridor(aRow+1, aCol, bRow, bCol);

	}
	else if (min == left)
	{
		new_corridor(aRow, aCol-1, bRow, bCol);
	}
	else if (min == right)
	{
		new_corridor(aRow, aCol+1, bRow, bCol);
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
	int n = get_random(7, 5);
	Room rooms[n];
	for (int i=0; i<n; i++)
	{
		rooms[i] = new_room();
	}

	for (int i=0; i<n-1; i++)
	{
		new_corridor(rooms[i].row, rooms[i].col, rooms[i+1].row, rooms[i+1].col);
	}

	print_dungeon();

	return 0;
}
