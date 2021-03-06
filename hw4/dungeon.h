#ifndef DUNGEON_H
# define DUNGEON_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>
#include <ncurses.h>

#include "queue.h"

#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define PLAYER '@'
#define MONSTER 'M'
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
	char terrain;
	int hardness;
} Terrain;

/*
typedef struct Player_Character
{
	//int characteristics;
	int row;
	int col;
	int birth;
	int speed;
	bool dead;
} PC;
*/
typedef struct Character
{
	unsigned int characteristics;
	int row;
	int col;
	int birth;
	int speed;
	int pc_row;
	int pc_col;
	bool dead;
	int dist[ROW * COL];
} Character;

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
	Character *monster;
	Character PC;
	Terrain map[ROW][COL];
} Dungeon;

Dungeon dungeon;

void initDungeon();

int get_random(int modulus, int min);

void print_dungeon();

void generate_dungeon();

void load_file(FILE *f);

void save_file(FILE *f);

void dijkstra_tunneling();

void dijkstra_nontunneling();

void move_npc();

#endif
