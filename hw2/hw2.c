#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>

#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define PC '@'
#define ROCK_H 255
#define ROOM_H 0
#define CORRIDOR_H 0
#define PC_H 0

typedef struct dungeon
{
	int num_room;
	int pc_row;
	int pc_col;
	int version;
} Dungeon;

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


Cell dungeon[ROW][COL];
Room *dungeonRoom;
Dungeon map;

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

int get_random(int modulus, int min)
{
	return rand() % modulus + min;
}


/*
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
*/



void print_dungeon()
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


bool is_inside(int row, int col)
{
	//is room not on edge or outside of dungeon
	return row > 0 && col > 0 && row < ROW-1 && col < COL-1;
}

bool is_valid_room(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col].space != ROCK) return false;
	if (!is_inside(row, col) || !is_inside(row+height, col+width)) return false;
	//touch or overlap another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1].space != ROCK) return false;//left touch
		if (dungeon[i][col].space != ROCK) return false;//left overlap
		if (dungeon[i][col+width+1].space != ROCK) return false;//right touch
		if (dungeon[i][col+width].space != ROCK) return false;//right overlap
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j].space != ROCK) return false;//top touch
		if (dungeon[row][j].space != ROCK) return false;//top overlap
		if (dungeon[row+height+1][j].space != ROCK) return false;//bottom touch
		if (dungeon[row+height][j].space != ROCK) return false;//bottom overlap
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
				dungeon[i][j].space = ROOM;
				dungeon[i][j].hardness = ROOM_H;
			}
		}
	}
	else
	{
		return new_room();
	}

	return r;
}

void add_room(int row, int col, int width, int height)
{
	for (int i=row; i<row+height; i++)
	{
		for (int j=col; j<col+width; j++)
		{
			dungeon[i][j].space = ROOM;
			dungeon[i][j].hardness = ROOM_H;
		}
	}
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

bool isConnected(int row, int col)
{
	return dungeon[row-1][col].space == CORRIDOR
		|| dungeon[row+1][col].space == CORRIDOR
		|| dungeon[row][col-1].space == CORRIDOR
		|| dungeon[row][col+1].space == CORRIDOR;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0) return;
	if (dungeon[aRow][aCol].space == CORRIDOR && isConnected(bRow, bCol)) return;
	if (dungeon[aRow][aCol].space == ROCK) dungeon[aRow][aCol].space = CORRIDOR;

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
		newCorridor(aRow-1, aCol, bRow, bCol);

	}
	else if (min == down)
	{
		newCorridor(aRow+1, aCol, bRow, bCol);

	}
	else if (min == left)
	{
		newCorridor(aRow, aCol-1, bRow, bCol);
	}
	else if (min == right)
	{
		newCorridor(aRow, aCol+1, bRow, bCol);
	}

/*
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
*/
}

void generate_dungeon(int n)
{
	//initial dungeon
	initDungeon();
	dungeonRoom = malloc(map.num_room * sizeof(Room));

	for (int i=0; i<n; i++)
	{
		dungeonRoom[i] = new_room();
		printf("  ROOM%2d    row=%2d   col=%2d   width=%2d   height=%2d\n", i, dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i].width, dungeonRoom[i].height);//TODO
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i+1].row, dungeonRoom[i+1].col);
	}
	
	//add initial player loaction
	map.pc_row = dungeonRoom[0].row;
	map.pc_col = dungeonRoom[0].col;
	dungeon[map.pc_row][map.pc_col].space = '@';
	dungeon[map.pc_row][map.pc_col].hardness = 0;
}

void load_file(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	initDungeon();

	char marker[12];
	fread(&marker, 1, 12, f);
	
	uint32_t ver;
	fread(&ver, 4, 1, f);
	map.version = be32toh(ver);

	uint32_t file_size;
	fread(&file_size, 4, 1, f);
	int filesize = be32toh(file_size);

	uint8_t pc_col;
	fread(&pc_col, 1, 1, f);
	map.pc_col = pc_col;
	uint8_t pc_row;
	fread(&pc_row, 1, 1, f);
	map.pc_row = pc_row;


	uint8_t hard[1680];
	fread(hard, 1, 1680, f);

	for (int row=0; row<ROW; row++)
	{
		for (int col=0; col<COL; col++)
		{
			int index = COL*row + col;
			int h = hard[index];
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

	map.num_room = (filesize - 1702) / 4;
	dungeonRoom = malloc(map.num_room * sizeof(Room));

	uint8_t roomRead[filesize - 1702];
	fread(roomRead, 1, filesize - 1702, f);

	int n = 0;
	for (int i=0; i<map.num_room; i++)
	{
		dungeonRoom[i].col = roomRead[n++];
		dungeonRoom[i].row = roomRead[n++];
		dungeonRoom[i].width = roomRead[n++];
		dungeonRoom[i].height = roomRead[n++];

		add_room(dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i].width, dungeonRoom[i].height);
	}
	
	//add PC
	dungeon[map.pc_row][map.pc_col].space = PC;
	dungeon[map.pc_row][map.pc_col].hardness = PC_H;

	fclose(f);
}

void save_file(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	char *marker = "RLG327-F2018";
	fwrite(marker, 1, 12, f);

	int ver = htobe32(map.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * map.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = map.pc_col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = map.pc_row;
	fwrite(&pc_y, 1, 1, f);

	char *hard = malloc(1680);
	//write hardness
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			hard[COL*i + j] = (char) dungeon[i][j].hardness;
		}
	}
	fwrite(hard, 1, 1680, f);

	//write room
	unsigned char *loc = malloc(4 *map.num_room);
	int n = 0;
	for (int i=0; i<map.num_room; i++)
	{
		loc[n++] = (unsigned char) dungeonRoom[i].col;
		loc[n++] = (unsigned char) dungeonRoom[i].row;
		loc[n++] = (unsigned char) dungeonRoom[i].width;
		loc[n++] = (unsigned char) dungeonRoom[i].height;
	}
	fwrite(loc, 1, 4*map.num_room, f);
	
	free(hard);
	free(loc);
	fclose(f);
}

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	printf("\nseed = %d;\n", seed);//TODD
	srand(seed);

	//generate random number of rooms
	map.num_room = get_random(7, 5);
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
		FILE *f = fopen(path, "r");
		load_file(f);
	}
	else
	{
		generate_dungeon(map.num_room);
	}
	print_dungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		save_file(f);
	}

	return 0;
}

