#ifndef DUNGEON_H
# define DUNGEON_H


#include <stdio.h>
#include <stdlib.h>

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
#define PC_VISION_RADIUS 2
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

using namespace std;

class Terrain
{
    public:
	    char terrain;
        char space;
        char fog;
	    int hardness;
};

class Room
{
    public:
        int row;
        int col;
        int width;
        int height;
};

class Character
{
    public:
        unsigned int characteristics;
        int row;
        int col;
        int birth;
        int speed;
        int pc_row;
        int pc_col;
        int dead;
        int dist[ROW * COL];
        int vision[ROW][COL];
};

class Dungeon
{
    public:
        int seed;
        int num_mon;
        int num_room;
        int version;
        Room *rooms;
        Character *monster;
        Character pc;
        int teleport_row;
        int teleport_col;
        Terrain map[ROW][COL];
        Queue pq_tunel;
        Queue pq_nontunel;
};

extern Dungeon dungeon;

void init_dungeon();
void generate_dungeon();
void print_dungeon();
void load_dungeon(FILE *f);
void save_dungeon(FILE *f);
void delete_dungeon();

int get_random(int modulus, int min);
int is_visible_terrain(int i, int j);
int is_room_corridor_stair(int row, int col);
void remember_map_PC();
int is_monster(int row, int col);
int is_inside(int row, int col);
void move_dungeon();

#endif
