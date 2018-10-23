#ifndef DUNGEON_H
# define DUNGEON_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <endian.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <curses.h>

#include "queue.h"

#define ROW 21
#define COL 80
#define TERMINAL_ROW 24
#define TERMINAL_COL 85
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define PLAYER '@'
#define STAIR_UP '<'
#define STAIR_DOWN '>'
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

#define MAX(x,y)			\
({							\
    typeof(x) _x = (x);     \
    typeof(y) _y = (y);     \
    (void) (&_x == &_y);    \
    _x > _y ? _x : _y;})

#define NPC_SMART         0x00000001
#define NPC_TELEPATH      0x00000002
#define NPC_TUNNEL        0x00000004
#define NPC_ERRATIC       0x00000008

class Terrain
{
	char space;
	int hardness;
};

class Character
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
};

class Room
{
	int row;
	int col;
	int width;
	int height;
};

class Dungeon
{
	int num_mon;
	int num_room;
	int version;
	Room *rooms;
	Character *monster;
	Character PC;
	Terrain map[ROW][COL];
	Queue pq_tunel;
	Queue pq_nontunel;
};

Dungeon dungeon;

int get_random(int modulus, int min);

void print_dungeon();

void generate_dungeon();

void load_file(FILE *f);

void save_file(FILE *f);

int is_monster(int row, int col);

void dijkstra_tunneling();

void dijkstra_nontunneling();

void move_dungeon();

#endif