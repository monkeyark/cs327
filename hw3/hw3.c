#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <stdint.h>

//#include "heap.h"

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

typedef struct cell
{
	char space;
	int hardness;
	int distance;
} Cell;

typedef struct rooms
{
	int row;
	int col;
	int width;
	int height;
} Room;

typedef struct level
{
	int num_room;
	int pc_row;
	int pc_col;
	int version;
	Room *rooms;
	Cell map[ROW][COL];
} Dungeon;

Dungeon dungeon;

void initDungeon()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
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
	for (int i = 0; i < COL; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");

	for (int i = 0; i < COL + 2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i = 0; i < ROW; i++)
	{
		printf("%2d|", i);
		for (int j = 0; j < COL; j++)
		{
			printf("%c", dungeon.map[i][j].space);
			//int x = dungeon[i][j].hardness / 85 + 1;
			//printf("%d", x);
		}
		printf("|\n");
	}

	for (int i = 0; i < COL + 2; i++)
	{
		printf("-");
	}
	printf("\n");
}

bool isInside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW-1 && col < COL-1;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon.map[row][col].space != ROCK)
		return false;
	if (!isInside(row, col) || !isInside(row + height, col + width))
		return false;
	//touch or overlap another room
	for (int i = row - 1; i < row + height + 2; i++)
	{
		if (dungeon.map[i][col - 1].space != ROCK)
			return false; //left touch
		if (dungeon.map[i][col].space != ROCK)
			return false; //left overlap
		if (dungeon.map[i][col + width + 1].space != ROCK)
			return false; //right touch
		if (dungeon.map[i][col + width].space != ROCK)
			return false; //right overlap
	}
	for (int j = col - 1; j < col + width + 2; j++)
	{
		if (dungeon.map[row - 1][j].space != ROCK)
			return false; //top touch
		if (dungeon.map[row][j].space != ROCK)
			return false; //top overlap
		if (dungeon.map[row + height + 1][j].space != ROCK)
			return false; //bottom touch
		if (dungeon.map[row + height][j].space != ROCK)
			return false; //bottom overlap
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
		for (int i = r.row; i < r.row + r.height; i++)
		{
			for (int j = r.col; j < r.col + r.width; j++)
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
	for (int i = row; i < row + height; i++)
	{
		for (int j = col; j < col + width; j++)
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

	return row * row + col * col;
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
	return dungeon.map[row - 1][col].space == CORRIDOR || dungeon.map[row + 1][col].space == CORRIDOR || dungeon.map[row][col - 1].space == CORRIDOR || dungeon.map[row][col + 1].space == CORRIDOR;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0)
		return;
	if (dungeon.map[aRow][aCol].space == CORRIDOR && isConnected(bRow, bCol))
		return;
	if (dungeon.map[aRow][aCol].space == ROCK)
		dungeon.map[aRow][aCol].space = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (isInside(aRow - 1, aCol))
	{
		top = distance(aRow - 1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (isInside(aRow + 1, aCol))
	{
		down = distance(aRow + 1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (isInside(aRow, aCol - 1))
	{
		left = distance(aRow, aCol - 1, bRow, bCol);
		min = minimum(min, left);
	}
	if (isInside(aRow, aCol + 1))
	{
		right = distance(aRow, aCol + 1, bRow, bCol);
		min = minimum(min, right);
	}

	if (min == top)
	{
		newCorridor(aRow - 1, aCol, bRow, bCol);
	}
	else if (min == down)
	{
		newCorridor(aRow + 1, aCol, bRow, bCol);
	}
	else if (min == left)
	{
		newCorridor(aRow, aCol - 1, bRow, bCol);
	}
	else if (min == right)
	{
		newCorridor(aRow, aCol + 1, bRow, bCol);
	}
}

void generateDungeon(int n)
{
	//initial dungeon
	initDungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));

	for (int i = 0; i < n; i++)
	{
		dungeon.rooms[i] = newRoom();
	}

	for (int i = 0; i < n - 1; i++)
	{
		newCorridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
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

	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COL; col++)
		{
			int index = COL * row + col;
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
	for (int i = 0; i < dungeon.num_room; i++)
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
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			hard[COL * i + j] = (char)dungeon.map[i][j].hardness;
		}
	}
	fwrite(hard, 1, 1680, f);

	//write room
	unsigned char *loc = malloc(4 * dungeon.num_room);
	int n = 0;
	for (int i = 0; i < dungeon.num_room; i++)
	{
		loc[n++] = (unsigned char)dungeon.rooms[i].col;
		loc[n++] = (unsigned char)dungeon.rooms[i].row;
		loc[n++] = (unsigned char)dungeon.rooms[i].width;
		loc[n++] = (unsigned char)dungeon.rooms[i].height;
	}
	fwrite(loc, 1, 4 * dungeon.num_room, f);

	free(hard);
	free(loc);
	fclose(f);
}

typedef struct Node
{
	int priority;
	struct Node *next;
} Node;

Node *newNode(int priority)
{
	Node *temp = malloc(sizeof(Node));
	temp->priority = priority;
	temp->next = NULL;

	return temp;
}

void insert(Node **head, int priority, int dis[ROW * COL])
{
	Node *temp = (*head);
	Node *new = newNode(priority);

	if (dis[(*head)->priority] >= dis[new->priority])
	{
		new->next = (*head);
		(*head) = new;
	}
	else
	{
		while (temp->next != NULL && dis[temp->next->priority] < dis[new->priority])
		{
			temp = temp->next;
		}

		new->next = temp->next;
		temp->next = new;
	}
}

int pop(Node **head)
{
	int n = (*head)->priority;
	(*head) = (*head)->next;
	return n;
}

bool isEmpty(Node **head)
{
	return (*head) == NULL;
}

int getHardness(int hardness)
{
	if (hardness == 255)
	{
		return 3;
	}
	else if (hardness == 0)
	{
		return 1;
	}
	else
	{
		return 1 + (hardness / 85);
	}
}

void print_dijkstra_bounded(int dis[ROW * COL], int row, int col)
{
	printf("print_dijkstra_bounded\n");
	putchar('\n');
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (row == i && col == j)
			{
				printf("%c", PC);
			}
			else if (dis[i * COL + j] != -1)
			{
				int n = dis[i * COL + j] % 10;
				printf("%d", n);
			}
			else
			{
				printf("%c", ROCK);
			}
		}
		printf("\n");
	}
	putchar('\n');
}

void print_dijkstra_map(int dis[ROW * COL], int row, int col)
{
	printf("print_dijkstra_map\n");
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (row == i && col == j)
			{
				printf("%c", PC);
			}
			else if (dis[i * COL + j] != -1)
			{
				int n = dis[i * COL + j] % 10;
				printf("%d", n);
			}
			else
			{
				printf(" ");
			}
		}

		printf("\n");
	}
}

static void dijkstra_map()
{
	//	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	//	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	int colMove[8] = {-80, -80, -80, 0, 0, 80, 80, 80};
	int rowMove[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	//initialization
	int i, j;
	int dis[ROW * COL];
	int pos = dungeon.pc_row * COL + dungeon.pc_col;
	Node *node = newNode(pos);
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (!isInside(i, j))
			{
				dis[i * COL + j] = -1;
			}
			else if (dungeon.map[i][j].space != PC)
			{
				dis[i * COL + j] = ROW * COL + 1;
				//dungeon.map[i][j].distance = ROW*COL+1;
				insert(&node, i * COL + j, dis);
			}
		}
	}
	dis[dungeon.pc_row * COL + dungeon.pc_col] = 0;
	//dungeon.map[dungeon.pc_row][dungeon.pc_col].distance = 0;

	while (!isEmpty(&node))
	{
		int u = pop(&node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i];
			if (0 > v || v > ROW * COL)
			{
				continue;
			}

			if (dis[v] >= 0)
			{
				alt = dis[u] + getHardness(dungeon.map[u / COL][u % COL].hardness);
				if (alt < dis[v])
				{
					dis[v] = alt;
					insert(&node, v, dis);
				}
			}
		}
	}
	print_dijkstra_map(dis, dungeon.pc_row, dungeon.pc_col);
	free(node);
}

static void dijkstra_bounded()
{
	//	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	//	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	int colMove[8] = {-80, -80, -80, 0, 0, 80, 80, 80};
	int rowMove[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
	//initialization
	int i, j;
	int dis[ROW * COL];
	int pos = dungeon.pc_row * COL + dungeon.pc_col;
	Node *node = newNode(pos);
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.map[i][j].space == ROOM || dungeon.map[i][j].space == CORRIDOR)
			{
				dis[i * COL + j] = ROW * COL + 1;
				//dungeon.map[i][j].distance = ROW*COL+1;
				insert(&node, i * COL + j, dis);
			}
			else if (dungeon.map[i][j].space == ROCK)
			{
				dis[i * COL + j] = -1;
				//dungeon.map[i][j].distance = -1;
			}
		}
	}
	dis[dungeon.pc_row * COL + dungeon.pc_col] = 0;
	//dungeon.map[dungeon.pc_row][dungeon.pc_col].distance = 0;

	while (!isEmpty(&node))
	{
		int u = pop(&node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i];
			if (0 > v || v > ROW * COL)
			{
				continue;
			}

			if (dis[v] >= 0)
			{
				alt = dis[u] + 1;
				if (alt < dis[v])
				{
					dis[v] = alt;
					insert(&node, v, dis);
				}
			}
		}
	}
	print_dijkstra_bounded(dis, dungeon.pc_row, dungeon.pc_col);
	free(node);
}

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	seed = 1016644798;
	printf("\nseed = %d;\n", seed);
	srand(seed);

	//generate random number of rooms
	dungeon.num_room = getRandom(7, 5);
	bool load = false;
	bool save = false;

	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
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
	dijkstra_bounded();
	dijkstra_map();

	return 0;
}
