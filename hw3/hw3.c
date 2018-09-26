#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>

#include "heap.h"

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

#define hardnesspair(pair) (d->hardness[pair[dim_row]][pair[dim_col]])

typedef struct dungeonCell
{
	char space;
	int hardness;
	int costtoPC;
} Cell;

typedef struct rooms
{
	int row;
	int col;
	int width;
	int height;
} Room;

typedef struct dungeonLevel
{
	int num_room;
	int pc_row;
	int pc_col;
	int version;
	Room *rooms;
	Cell map[ROW][COL];
} Dungeon;

typedef struct path
{
	heap_node_t *hn;
	uint8_t pos[2];
	uint8_t from[2];
	int32_t cost;
} path_t;

typedef enum dim
{
	dim_row, dim_col, num_dims
} dim_t;

typedef int16_t pair[num_dims];

Dungeon dungeon;

void initDungeon()
{
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			dungeon.map[i][j].space = ROCK;
			dungeon.map[i][j].hardness = ROCK_H;
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
			printf("%c", dungeon.map[i][j].space);
			//int x = dungeon[i][j].hardness / 85 + 1;
			//printf("%d", x);
		}
		printf("|\n");
	}

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");
}

bool isInside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW && col < COL;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon.map[row][col].space != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//touch or overlap another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon.map[i][col-1].space != ROCK) return false;//left touch
		if (dungeon.map[i][col].space != ROCK) return false;//left overlap
		if (dungeon.map[i][col+width+1].space != ROCK) return false;//right touch
		if (dungeon.map[i][col+width].space != ROCK) return false;//right overlap
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon.map[row-1][j].space != ROCK) return false;//top touch
		if (dungeon.map[row][j].space != ROCK) return false;//top overlap
		if (dungeon.map[row+height+1][j].space != ROCK) return false;//bottom touch
		if (dungeon.map[row+height][j].space != ROCK) return false;//bottom overlap
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
				dungeon.map[i][j].space = ROOM;
				dungeon.map[i][j].hardness = ROOM_H;
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
	for (int i=row; i<row+height; i++)
	{
		for (int j=col; j<col+width; j++)
		{
			dungeon.map[i][j].space = ROOM;
			dungeon.map[i][j].hardness = ROOM_H;
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
	return dungeon.map[row-1][col].space == CORRIDOR
		|| dungeon.map[row+1][col].space == CORRIDOR
		|| dungeon.map[row][col-1].space == CORRIDOR
		|| dungeon.map[row][col+1].space == CORRIDOR;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0) return;
	if (dungeon.map[aRow][aCol].space == CORRIDOR && isConnected(bRow, bCol)) return;
	if (dungeon.map[aRow][aCol].space == ROCK) dungeon.map[aRow][aCol].space = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (isInside(aRow-1, aCol))
	{
		top = distance(aRow-1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (isInside(aRow+1, aCol))
	{
		down = distance(aRow+1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (isInside(aRow, aCol-1))
	{
		left = distance(aRow, aCol-1, bRow, bCol);
		min = minimum(min, left);
	}
	if (isInside(aRow, aCol+1))
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

void generateDungeon(int n)
{
	//initial dungeon
	initDungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));

	for (int i=0; i<n; i++)
	{
		dungeon.rooms[i] = newRoom();
//		printf("  ROOM%2d    row=%2d   col=%2d   width=%2d   height=%2d\n", i, rooms[i].row, rooms[i].col, rooms[i].width, rooms[i].height);//TODO
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i+1].row, dungeon.rooms[i+1].col);
	}

	//add initial player loaction
	dungeon.pc_row = dungeon.rooms[0].row;
	dungeon.pc_col = dungeon.rooms[0].col;
	dungeon.map[dungeon.pc_row][dungeon.pc_col].space = '@';
	dungeon.map[dungeon.pc_row][dungeon.pc_col].hardness = 0;
}

void loadFile(FILE *f)
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
	dungeon.version = be32toh(ver);

	uint32_t file_size;
	fread(&file_size, 4, 1, f);
	int filesize = be32toh(file_size);

	uint8_t pc_col;
	fread(&pc_col, 1, 1, f);
	dungeon.pc_col = pc_col;
	uint8_t pc_row;
	fread(&pc_row, 1, 1, f);
	dungeon.pc_row = pc_row;


	uint8_t hard[1680];
	fread(hard, 1, 1680, f);

	for (int row=0; row<ROW; row++)
	{
		for (int col=0; col<COL; col++)
		{
			int index = COL*row + col;
			int h = hard[index];
			dungeon.map[row][col].hardness = h;
			if (h == 0)
			{
				dungeon.map[row][col].space = CORRIDOR;
			}
			else
			{
				dungeon.map[row][col].space = ROCK;
			}
		}
	}

	dungeon.num_room = (filesize - 1702) / 4;
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));

	uint8_t roomRead[filesize - 1702];
	fread(roomRead, 1, filesize - 1702, f);

	int n = 0;
	for (int i=0; i<dungeon.num_room; i++)
	{
		dungeon.rooms[i].col = roomRead[n++];
		dungeon.rooms[i].row = roomRead[n++];
		dungeon.rooms[i].width = roomRead[n++];
		dungeon.rooms[i].height = roomRead[n++];

		addRoom(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i].width, dungeon.rooms[i].height);
	}

	//add PC
	dungeon.map[dungeon.pc_row][dungeon.pc_col].space = PC;
	dungeon.map[dungeon.pc_row][dungeon.pc_col].hardness = PC_H;

	fclose(f);
}

void saveFile(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	char *marker = "RLG327-F2018";
	fwrite(marker, 1, 12, f);

	int ver = htobe32(dungeon.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * dungeon.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = dungeon.pc_col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = dungeon.pc_row;
	fwrite(&pc_y, 1, 1, f);

	char *hard = malloc(1680);
	//write hardness
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			hard[COL*i + j] = (char) dungeon.map[i][j].hardness;
		}
	}
	fwrite(hard, 1, 1680, f);

	//write room
	unsigned char *loc = malloc(4 *dungeon.num_room);
	int n = 0;
	for (int i=0; i<dungeon.num_room; i++)
	{
		loc[n++] = (unsigned char) dungeon.rooms[i].col;
		loc[n++] = (unsigned char) dungeon.rooms[i].row;
		loc[n++] = (unsigned char) dungeon.rooms[i].width;
		loc[n++] = (unsigned char) dungeon.rooms[i].height;
	}
	fwrite(loc, 1, 4*dungeon.num_room, f);

	free(hard);
	free(loc);
	fclose(f);
}

static int32_t path_cmp(const void *key, const void *with)
{
	return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

static void dijkstra(Dungeon *d, pair from, pair to)
{
	heap_t h;
	static path_t path[ROW][COL], *p;
	static uint32_t initialized = 0;
	uint32_t row, col;

	if (!initialized)
	{
		for (row = 0; row < ROW; row++)
		{
			for (col = 0; col < COL; col++)
			{
				path[row][col].pos[dim_row] = row;
				path[row][col].pos[dim_col] = col;
			}
		}
		initialized = 1;
	}

	for (row = 0; row < ROW; row++)
	{
		for (col = 0; col < COL; col++)
		{
			path[row][col].cost = ROW*COL+1;
		}
	}

	path[from[dim_row]][from[dim_col]].cost = 0;

	heap_init(&h, path_cmp, NULL);

	for (row = 0; row < ROW; row++)
	{
		for (col = 0; col < COL; col++)
		{
			if (dungeon.map[row][col].hardness != 255)
			//if (dungeon.map[row][col] != ter_wall_immutable)
			{
				path[row][col].hn = heap_insert(&h, &path[row][col]);
			}
			else
			{
				path[row][col].hn = NULL;
			}
		}
	}

	while ((p = heap_remove_min(&h)))
	{
		p->hn = NULL;

		if ((p->pos[dim_row] == to[dim_row]) && p->pos[dim_col] == to[dim_col])
		{
			for (row = to[dim_col], row = to[dim_row];
					(col != from[dim_col]) || (row != from[dim_row]);
					p = &path[row][col], col = p->from[dim_col], row = p->from[dim_row])
			{
				dungeon.map[row][col].costtoPC = 0;

				if (dungeon.map[row][col].space != ROOM)
				//if (dungeon.map[row][col] != ter_floor_room)
				{
					dungeon.map[row][col].costtoPC = 0;
					dungeon.map[row][col].space = CORRIDOR;
					dungeon.map[row][col].hardness = 0;
					//TODO change make corridor to find path
				}
			}
			heap_delete(&h);
			return;
		}

		//updating cost to PC
		if ((path[p->pos[dim_row] - 1][p->pos[dim_col]].hn)
				&& (path[p->pos[dim_row] - 1][p->pos[dim_col]].cost > p->cost + (dungeon.map[row][col].hardness[(p->pos)[dim_row]][(p->pos)[dim_col]]))
		{
			path[p->pos[dim_row] - 1][p->pos[dim_col]].cost = p->cost + hardnesspair(p->pos);
			path[p->pos[dim_row] - 1][p->pos[dim_col]].from[dim_row] = p->pos[dim_row];
			path[p->pos[dim_row] - 1][p->pos[dim_col]].from[dim_col] = p->pos[dim_col];
			heap_decrease_key_no_replace(&h, path[p->pos[dim_row] - 1][p->pos[dim_col]].hn);
		}
		if ((path[p->pos[dim_row]][p->pos[dim_col] - 1].hn)
				&& (path[p->pos[dim_row]][p->pos[dim_col] - 1].cost > p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_row]][p->pos[dim_col] - 1].cost = p->cost + hardnesspair(p->pos);
			path[p->pos[dim_row]][p->pos[dim_col] - 1].from[dim_row] = p->pos[dim_row];
			path[p->pos[dim_row]][p->pos[dim_col] - 1].from[dim_col] = p->pos[dim_col];
			heap_decrease_key_no_replace(&h, path[p->pos[dim_row]][p->pos[dim_col] - 1].hn);
		}
		if ((path[p->pos[dim_row]][p->pos[dim_col] + 1].hn)
				&& (path[p->pos[dim_row]][p->pos[dim_col] + 1].cost > p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_row]][p->pos[dim_col] + 1].cost = p->cost + hardnesspair(p->pos);
			path[p->pos[dim_row]][p->pos[dim_col] + 1].from[dim_row] = p->pos[dim_row];
			path[p->pos[dim_row]][p->pos[dim_col] + 1].from[dim_col] = p->pos[dim_col];
			heap_decrease_key_no_replace(&h, path[p->pos[dim_row]][p->pos[dim_col] + 1].hn);
		}
		if ((path[p->pos[dim_row] + 1][p->pos[dim_col]].hn)
				&& (path[p->pos[dim_row] + 1][p->pos[dim_col]].cost > p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_row] + 1][p->pos[dim_col]].cost = p->cost + hardnesspair(p->pos);
			path[p->pos[dim_row] + 1][p->pos[dim_col]].from[dim_row] = p->pos[dim_row];
			path[p->pos[dim_row] + 1][p->pos[dim_col]].from[dim_col] = p->pos[dim_col];
			heap_decrease_key_no_replace(&h, path[p->pos[dim_row] + 1][p->pos[dim_col]].hn);
		}
	}
}

/*
int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	printf("\nseed = %d;\n", seed);
	srand(seed);

	//generate random number of rooms
	dungeon.num_room = getRandom(7, 5);
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
		loadFile(f);
	}
	else
	{
		generateDungeon(dungeon.num_room);
	}
	printDungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		saveFile(f);
	}

	return 0;
}
*/
