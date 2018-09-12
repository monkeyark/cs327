#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <endian.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define ROCK_H 255
#define ROOM_H 0
#define CORRIDOR_H 0

enum action
{
	action_print,
	action_load,
	action_save,
	action_load_save,
	action_save_load
};

typedef struct dungeonCell
{
	char space;
	int hardness;
} Cell;

typedef struct dungeonRoom
{
	int row;
	int col;
	int width;
	int height;
} Room;

typedef struct player
{
	int row;
	int col;
} Player;

Cell dungeon[ROW][COL];
Player me;

void initDungeon()
{
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			dungeon[i][j].space = ROCK;
			dungeon[i][j].hardness = ROCK_H;
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
			printf("%c", dungeon[i][j].space);
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
	if (dungeon[row][col].space != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//is touching another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1].space != ROCK) return false; //left
		if (dungeon[i][col+width+1].space != ROCK) return false;//right
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j].space != ROCK) return false;//top
		if (dungeon[row+height+1][j].space != ROCK) return false;//bottom
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
				dungeon[i][j].space = ROOM;
				dungeon[i][j].hardness = ROOM_H;
			}
		}
	}
	else
	{
		return newRoom();
	}

	return r;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (aRow <= bRow) 	//now at dungeon[aRow][aCol]
	{
		for (int i=aRow; i<=bRow; i++)
		{
			if (dungeon[i][aCol].space == ROCK)
			{
				dungeon[i][aCol].space = CORRIDOR;
				dungeon[i][aCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[bRow][aCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}
	else
	{
		for (int i=bRow; i<=aRow; i++)
		{
			if (dungeon[i][bCol].space == ROCK)
			{
				dungeon[i][bCol].space = CORRIDOR;
				dungeon[i][bCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[aRow][bCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}

}

void generateDungeon(int seed)
{
	//initial dungeon
	initDungeon();

	printf("\nseed = %d;\n", seed);
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
	
	//add initial player loaction
	me.row = rooms[0].row;
	me.col = rooms[0].col;
	dungeon[me.row][me.col].space = '@';
	dungeon[me.row][me.col].hardness = 0;
}

void load()
{
	FILE *f;
	char *path;
	path = malloc(strlen(getenv("HOME") + strlen("/.rlg327/dungeon") + 1));
	strcpy(path, getenv("HOME"));
	strcat(path, "/.rlg327/dungeon");
	f = fopen(path, "f");
	free(path);
	if (!f)
	{
		fprintf(stderr, "Failed to open file%s\n", path);
		return;
	}

	char *marker;
	fread(marker, 1, 12, f);

	uint32_t *version;
	fread(version, 4, 1, f);
	uint32_t *file_size;
	fread(file_size, 4, 1, f);
	uint8_t *PCcol;
	fread(PCcol, 1, 1, f);
	uint8_t *PCrow;
	fread(PCrow, 1, 1, f);
	

	uint8_t hard[1680];
	for (int i=0; i<1680; i++)
	{
		fread(hard, 1, 1, f);
	}


}

void save()
{
	FILE f;
	char *path;
	path = malloc(strlen(getenv("HOME") + strlen("/.rlg327/dungeon") + 1));
	strcpy(path, getenv("HOME"));
	strcat(path, "/.rlg327/dungeon");
	f = fopen(path, "w");
	free(path);
	if (!f)
	{
		fprintf(stderr, "Failed to open file%s\n", path);
		return;
	}


}

int main(int argc, char *argv[])
{
	//set up random seed
	int seed = time(NULL);
	//seed = 1536656664; seed = 1536656798; seed = 1536657024; seed = 1536657138; 

	enum action act = action_print;
	bool load = false;
	bool save = false;

	for (int i=1; i<argc; i++)
	{
		if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "--load"))
		{
			fprintf(stderr, "Bad argument\n");
			return -1;
		}
		if (!strcmp(argv[i], "--save"))
		{
			save = true;
			if (load)
			{
				act = action_load_save;
			}
			else
			{
				act = action_save;
			}
		}
		if (!strcmp(argv[i], "--load"))
		{
			load = true;
			if (save)
			{
				act = action_load_save;
			}
			else
			{
				act = action_load;
			}
		}
	}

	switch (act)
	{
		case action_print:
			generateDungeon(seed);
			printDungeon();
		case action_load:
			load();
			printDungeon();
			break;
		case action_save:
			generateDungeon(seed);
			printDungeon();
			save();
			break;
		case action_load_save:
			load();
			printDungeon();
			save();
			break;
		case action_save_load:
			generateDungeon(seed);
			printDungeon();
			save();
			load();
			break;
	}


	return 0;
}

