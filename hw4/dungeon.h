#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>

#include "queue.h"

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

void initDungeon();

int getRandom(int modulus, int min);

void printDungeon();

bool isInside(int row, int col);

bool isValidRoom(int row, int col, int width, int height);

Room newRoom();

void addRoom(int row, int col, int width, int height);

int distance(int aRow, int aCol, int bRow, int bCol);

int minimum(int a, int b);

bool isConnected(int row, int col);

void newCorridor(int aRow, int aCol, int bRow, int bCol);

NPC newMonster(int birth);

void generateDungeon();

void loadFile(FILE *f);

void saveFile(FILE *f);

int getHardnessCost(int hardness);

void print_dijkstra_path(int dist[ROW * COL], int row, int col);

void dijkstra_tunneling();

void dijkstra_nontunneling();
