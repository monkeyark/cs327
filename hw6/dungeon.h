#ifndef DUNGEON_H
# define DUNGEON_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

/*
class Character
{
    public:
        unsigned int characteristics;
        int row;
        int col;
        int birth;
        int speed;
        bool dead;
        int dist[ROW * COL];
        bool vision[ROW][COL];
};

class PC
{
    public:
        bool vision[ROW][COL];
};

class NPC
{
    public:
        unsigned int characteristics;
        int pc_row;
        int pc_col;
        int dist[ROW * COL];
}
*/

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
        bool dead;
        int dist[ROW * COL];
        bool vision[ROW][COL];
};

class Room
{
    public:
        int row;
        int col;
        int width;
        int height;
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
        Character PC;
        int teleport_row;
        int teleport_col;
        Terrain map[ROW][COL];
        Queue pq_tunel;
        Queue pq_nontunel;
};

extern Dungeon dungeon;

void init_dungeon();
void new_room(int row, int col, int width, int height);
void generate_dungeon();
void delete_dungeon();
void print_dungeon();
int get_random(int modulus, int min);
int is_monster(int row, int col);
bool is_inside(int row, int col);
void move_dungeon();

#endif
