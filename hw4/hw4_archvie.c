/*
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
#define PLAYER '@'
#define MONSTER 'D'
#define ROCK_H 255
#define ROOM_H 0
#define CORRIDOR_H 0
#define PC_H 0
#define MIN(x,y)			\
({							\
    typeof(x) _x = (x);     \
    typeof(y) _y = (y);     \
    (void) (&_x == &_y);    \
    _x < _y ? _x : _y;})

#define NPC_SMART         0x00000001
#define NPC_TELEPATH      0x00000002
#define NPC_TUNNEL        0x00000004
#define NPC_ERRATIC       0x00000008

typedef struct Terrain
{
	char space;
	int hardness;
//	int distance;
} Terrain;

typedef struct Player_Character
{
	int characteristics;
	int row;
	int col;
	int birth;
	int speed;
} PC;

typedef struct Non_Player_Character
{
	int characteristics;
	int row;
	int col;
	int birth;
	int speed;
	int pc_row;
	int pc_col;
} NPC;

typedef struct rooms
{
	int row;
	int col;
	int width;
	int height;
} Room;

typedef struct level
{
	int num_mon;
	int num_room;
	int version;
	Room *rooms;
	NPC *monster;
	PC PC;
	Terrain map[ROW][COL];
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

int get_random(int modulus, int min)
{
	return rand() % modulus + min;
}

void print_dungeon()
{
	printf("   ");
	for (int i = 0; i < COL; i++)
	{
		int row = (i - i % 10) / 10;
		if (i % 10 == 0)
		{
			printf("%d", row);
		}
		else
		{
			putchar(' ');
		}
	}
	printf("\n");

	printf("   ");
	for (int i = 0; i < COL; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");

	printf("  ");
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
		}
		printf("|\n");
	}
	printf("  ");
	for (int i = 0; i < COL + 2; i++)
	{
		printf("-");
	}
	printf("\n");
}

bool is_inside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW-1 && col < COL-1;
}

bool is_valid_room(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon.map[row][col].space != ROCK)
		return false;
	if (!is_inside(row, col) || !is_inside(row + height, col + width))
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
		return new_room();
	}

	return r;
}

void add_room(int row, int col, int width, int height)
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

bool is_connected(int row, int col)
{
	return dungeon.map[row - 1][col].space == CORRIDOR
	|| dungeon.map[row + 1][col].space == CORRIDOR
	|| dungeon.map[row][col - 1].space == CORRIDOR
	|| dungeon.map[row][col + 1].space == CORRIDOR;
}

void new_corridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0)
		return;
	if (dungeon.map[aRow][aCol].space == CORRIDOR && is_connected(bRow, bCol))
		return;
	if (dungeon.map[aRow][aCol].space == ROCK)
		dungeon.map[aRow][aCol].space = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (is_inside(aRow - 1, aCol))
	{
		top = distance(aRow - 1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (is_inside(aRow + 1, aCol))
	{
		down = distance(aRow + 1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (is_inside(aRow, aCol - 1))
	{
		left = distance(aRow, aCol - 1, bRow, bCol);
		min = minimum(min, left);
	}
	if (is_inside(aRow, aCol + 1))
	{
		right = distance(aRow, aCol + 1, bRow, bCol);
		min = minimum(min, right);
	}

	if (min == top)
	{
		new_corridor(aRow - 1, aCol, bRow, bCol);
	}
	else if (min == down)
	{
		new_corridor(aRow + 1, aCol, bRow, bCol);
	}
	else if (min == left)
	{
		new_corridor(aRow, aCol - 1, bRow, bCol);
	}
	else if (min == right)
	{
		new_corridor(aRow, aCol + 1, bRow, bCol);
	}
}

NPC newMonster(int birth)
{
	NPC npc;
	//creating NPC with all four characteristics having 1/2 probability, clean unused bits
	npc.characteristics = rand() & 0xf;
	npc.birth = birth;
	npc.speed = get_random(20, 5);

	npc.row = get_random(ROW, 0);
	npc.col = get_random(COL, 0);

	if (npc.characteristics & NPC_TELEPATH)  //monster is
	{
		npc.pc_row = dungeon.PC.row;
		npc.pc_col = dungeon.PC.col;
	}

	//add monster into map
	if (dungeon.map[npc.row][npc.col].space == ROOM ||
			dungeon.map[npc.row][npc.col].space == CORRIDOR)
	{
		dungeon.map[npc.row][npc.col].space = MONSTER;
		dungeon.map[npc.row][npc.col].hardness = 0;

	}
	else
	{
		newMonster(birth);
	}

//	if (npc.characteristics & NPC_TUNNEL)  //monster is TUNNEL
//	{
//		printf("birth = %d; npc.row = %d; npc.col = %d\n", birth, npc.row, npc.col);//TODO
//		if (dungeon.map[npc.row][npc.col].space == ROOM ||
//				dungeon.map[npc.row][npc.col].space == CORRIDOR ||
//				dungeon.map[npc.row][npc.col].space == ROCK)
//		{
//			dungeon.map[npc.row][npc.col].space = MONSTER;
//		}
//		else
//		{
//			newMonster(birth);
//		}
//	}
//	else
//	{
//		if (dungeon.map[npc.row][npc.col].space == ROOM ||
//				dungeon.map[npc.row][npc.col].space == CORRIDOR)
//		{
//			dungeon.map[npc.row][npc.col].space = MONSTER;
//		}
//		else
//		{
//			newMonster(birth);
//		}
//	}

	return npc;
}

void generate_dungeon()
{
	//initialize dungeon
	initDungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = malloc(dungeon.num_mon * sizeof(Room));
	int i = 0;
	for (i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i] = new_room();
	}

	for (i = 0; i < dungeon.num_room - 1; i++)
	{
		new_corridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
	}

	for (i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = newMonster(i);
		printf("birth = %d; npc.row = %d; npc.col = %d\n", dungeon.monster[i].birth, dungeon.monster[i].row, dungeon.monster[i].col);//TODO
		;
	}

	//add initial player location
	dungeon.PC.birth = -1;
	dungeon.PC.speed = 10;
	dungeon.PC.row = dungeon.rooms[0].row;
	dungeon.PC.col = dungeon.rooms[0].col;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].space = '@';
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = 0;
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
	dungeon.version = be32toh(ver);

	uint32_t file_size;
	fread(&file_size, 4, 1, f);
	int filesize = be32toh(file_size);

	uint8_t pc_col;
	fread(&pc_col, 1, 1, f);
	dungeon.PC.col = pc_col;
	uint8_t pc_row;
	fread(&pc_row, 1, 1, f);
	dungeon.PC.row = pc_row;

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

		add_room(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i].width, dungeon.rooms[i].height);
	}

	//add PC
	dungeon.map[dungeon.PC.row][dungeon.PC.col].space = PLAYER;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = PC_H;

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

	int ver = htobe32(dungeon.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * dungeon.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = dungeon.PC.col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = dungeon.PC.row;
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
	struct Node *next;
	int priority;
} Node;

Node *node_new(int priority)
{
	Node *temp = malloc(sizeof(Node));
	temp->priority = priority;
	temp->next = NULL;

	return temp;
}

void pq_insert(Node **head, int priority, int *dist)
{
	Node *temp = *head;
	Node *new = node_new(priority);

	if (dist[(*head)->priority] >= dist[new->priority])
	{
		new->next = (*head);
		(*head) = new;
	}
	else
	{
		while (temp->next != NULL && dist[temp->next->priority] < dist[new->priority])
		{
			temp = temp->next;
		}

		new->next = temp->next;
		temp->next = new;
	}
}

int pq_pop(Node **head)
{
	int n = (*head)->priority;
	Node *t = *head;
	(*head) = (*head)->next;
	free(t);
	return n;
}

bool pq_isEmpty(Node **head)
{
	return (*head) == NULL;
}

int get_hardness_cost(int hardness)
{
	if (hardness == 255) return 3;
	if (hardness == 0) return 1;
	return 1 + (hardness / 85);
}

void print_dijkstra_path(int dist[ROW * COL], int row, int col)
{
	putchar('\n');
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (row == i && col == j)
			{
				printf("%c", PLAYER);
			}
			else if (dist[i * COL + j] != -1)
			{
				int n = dist[i * COL + j] % 10;
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

void dijkstra_tunneling()
{
	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	//initialization
	int i, j;
	int dist[ROW * COL];
	memset(dist, 0, sizeof (dist));
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (!is_inside(i, j))
			{
				dist[i * COL + j] = -1;
			}
			else if (dungeon.map[i][j].space != PLAYER)
			{
				dist[i * COL + j] = ROW * COL + 1;
				//dungeon.map[i][j].distance = ROW*COL+1;
				pq_insert(&node, i * COL + j, dist);
			}
			
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;
	//dungeon.map[dungeon.pc_row][dungeon.pc_col].distance = 0;

	while (!pq_isEmpty(&node))
	{
		int u = pq_pop(&node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i] * COL;
			if (0 > v || v > ROW * COL) continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + get_hardness_cost(dungeon.map[u / COL][u % COL].hardness);
				if (alt < dist[v])
				{
					dist[v] = alt;
					pq_insert(&node, v, dist);
				}
			}
		}
	}
	print_dijkstra_path(dist, dungeon.PC.row, dungeon.PC.col);
}

void dijkstra_nontunneling()
{
	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	//initialization
	int i, j;
	int dist[ROW * COL];
	memset(dist, 0, sizeof (dist));
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.map[i][j].space == ROOM ||
					dungeon.map[i][j].space == CORRIDOR ||
					dungeon.map[i][j].space == MONSTER)
			//if (dungeon.map[i][j].space == ROOM || dungeon.map[i][j].space == CORRIDOR)
			{
				dist[i * COL + j] = ROW * COL + 1;
				//dungeon.map[i][j].distance = ROW*COL+1;
				pq_insert(&node, i * COL + j, dist);
			}
			else if (dungeon.map[i][j].space == ROCK)
			{
				dist[i * COL + j] = -1;
				//dungeon.map[i][j].distance = -1;
			}
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;
	//dungeon.map[dungeon.pc_row][dungeon.pc_col].distance = 0;

	while (!pq_isEmpty(&node))
	{
		int u = pq_pop(&node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i] * COL;
			if (0 > v || v > ROW * COL) continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + 1;
				if (alt < dist[v])
				{
					dist[v] = alt;
					pq_insert(&node, v, dist);
				}
			}
		}
	}
	print_dijkstra_path(dist, dungeon.PC.row, dungeon.PC.col);
}


int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	//seed = 1538543175;
	seed = 1538556186;

	printf("\nseed = %d;\n\n", seed);
	srand(seed);

	//generate random number of rooms
	dungeon.num_room = get_random(7, 5);
	//generate random number of monster
	//dungeon.num_mon = get_random(5, 8);
	dungeon.num_mon = 8;

	bool load = false;
	bool save = false;
	//bool nummon = false;

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
		load_file(f);
	}
	else
	{
		generate_dungeon();
	}
	print_dungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		save_file(f);
	}
	dijkstra_nontunneling();
	dijkstra_tunneling();

	free(dungeon.rooms);
	free(dungeon.monster);

	return 0;
}
*/
