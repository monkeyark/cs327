#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define ROCK_H 255
#define ROOM_H 0
#define CORRIDOR_H 0


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
Room *room;
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

void addRoom(int row, int col, int width, int height)
{
	room->row = row;
	room->col = col;
	room->width = width;
	room->height = height;
	room++;

	for (int i=row; i<row+height; i++)
	{
		for (int j=col; j<col+width; j++)
		{
			dungeon[i][j].space = ROOM;
			dungeon[i][j].hardness = ROOM_H;
		}
	}
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

void loadFile(char *path)
{
	printf("%s\n", path);
	FILE *f = fopen(path, "r");
	
//	printf();
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");

		return;
	}

	char marker[12];
	fread(marker, 1, 12, f);

	u_int32_t version[1];
	fread(version, 4, 1, f);

	u_int32_t file_size[1];
	fread(file_size, 4, 1, f);
	int filesize = be32toh(*file_size);

	u_int8_t PCcol[1];
	fread(PCcol, 1, 1, f);
	u_int8_t PCrow[1];
	fread(PCrow, 1, 1, f);

	u_int8_t hard[1680];
	fread(hard, 1, 1680, f);
	initDungeon();
	for (int row=0; row<ROW; row++)
	{
		for (int col=0; col<COL; col++)
		{
			int h = hard[ROW*row + col];
			dungeon[row][col].hardness = h;
			if (h == 0)
			{
				dungeon[row][col].space = CORRIDOR;
			}
			else
			{
				dungeon[row][col].space = ROCK;
			}
		}
	}

	u_int8_t room[filesize - 1702];
	fread(room, 1, filesize - 1702, f);
	int n = filesize - 1702;
	for (int i=0; i<n; i++)
	{
		int row = room[i++];
		int col = room[i++];
		int width = room[i++];
		int height = room[i];

		addRoom(row, col, width, height);
	}

	
	fclose(f);
}

void saveFile(char *path)
{
/*
	FILE *f;
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

	char *marker = "RLG327-F2018";
	fwrite(marker, 1, 12, f);

	int version = 0;
	version = htobe32(version);
	fwrite(&version, 4, 1, f);

	int filesize = 0;
	filesize = heobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	//write hardness
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			uint8_t hard;
			dungeon[i][j].space = ROCK;
		}
	}

	//write rooms

	fclose(f);
*/
}

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	//seed = 1536656664; seed = 1536656798; seed = 1536657024; seed = 1536657138; seed = 1536807801;	

	bool load = false;
	bool save = false;

	if (argc != 1)
	{

		for (int i=1; i<argc; i++)
		{
			if (strcmp(argv[i], "--save") != 0 && strcmp(argv[i], "--load") != 0)
			{
				fprintf(stderr, "Bad argument\n");
				return -1;
			}
			if (strcmp(argv[i], "--save") == 0)
			{
				save = true;
			}
			if (strcmp(argv[i], "--load") == 0)
			{
				load = true;
			}
		}

	}

	if (load)
	{
		printf("loading dungeon...\n");
		loadFile(path);
	}
	else
	{
		generateDungeon(seed);
	}
	printDungeon();

	if (save)
	{
		printf("saving dungeon...\n");
		saveFile(path);
	}

	return 0;
}

