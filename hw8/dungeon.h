#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "queue.h"
#include "dice.h"

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

#define MIN(a, b) \
  ({                          \
    __typeof__ (a) _a = (a);\
    __typeof__ (b) _b = (b);\
     _a < _b ? _a : _b; })

#define MAX(a, b) \
  ({                          \
    __typeof__ (a) _a = (a);\
    __typeof__ (b) _b = (b);\
    _a > _b ? _a : _b; })

/*
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
*/

#define NPC_SMART 0x00000001
#define NPC_TELEPATH 0x00000002
#define NPC_TUNNEL 0x00000004
#define NPC_ERRATIC 0x00000008
#define NPC_PASS_WALL 0x00000010
#define NPC_DESTROY_OBJ 0x00000020
#define NPC_PICKUP_OBJ 0x00000040
#define NPC_UNIQ 0x00000080
#define NPC_BOSS 0x00000100
#define NPC_BIT09 0x00000200
#define NPC_BIT10 0x00000400
#define NPC_BIT11 0x00000800
#define NPC_BIT12 0x00001000
#define NPC_BIT13 0x00002000
#define NPC_BIT14 0x00004000
#define NPC_BIT15 0x00008000
#define NPC_BIT16 0x00010000
#define NPC_BIT17 0x00020000
#define NPC_BIT18 0x00040000
#define NPC_BIT19 0x00080000
#define NPC_BIT20 0x00100000
#define NPC_BIT21 0x00200000
#define NPC_BIT22 0x00400000
#define NPC_BIT23 0x00800000
#define NPC_BIT24 0x01000000
#define NPC_BIT25 0x02000000
#define NPC_BIT26 0x04000000
#define NPC_BIT27 0x08000000
#define NPC_BIT28 0x10000000
#define NPC_BIT29 0x20000000
#define NPC_BIT30 0x40000000
#define NPC_BIT31 0x80000000

using namespace std;

enum Colors
{
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE
};

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

class Monster : public Character
{
public:
  string name;
  string description;
  char symbol;
  //vector<int> color;
  vector<int> *color;
  int color_int;
  int ability;
  int rrty;

  dice damage;
  dice hitpoints;
  dice speed_dice;

  string ability_string;
  string color_string;
};

class Item
{
  public:
  int row;
  int col;
  int seen;
};

class Object : public Item
{
public:
  string name;
  string description;
  //vector<int> color;
  vector<int> *color;
  int color_int;
  dice hit;
  dice damage;
  dice dodge;
  dice defence;
  dice weight;
  dice speed;
  dice attribute;
  dice value;
  bool artifact;
  int rarity;

  string type;
  char symbol;

  string color_string;
};

/*
class NPC : public Character
{
    public:
        string name;
        string description;
        char symbol;
        uint32_t color;
        uint32_t abilities;
        dice damage;
        uint32_t hitpoints;
        unsigned int characteristics;
        int row;
        int col;
        int birth;
        int32_t speed;
        int pc_row;
        int pc_col;
        int dead;
        int dist[ROW * COL];
        int vision[ROW][COL];
};
*/

class Dungeon
{
public:
  int seed;
  int num_mon;
  int num_item;
  int num_room;
  int version;
  Room *rooms;
  Character *monster;
  Item *item;
  Character pc;
  int teleport_row;
  int teleport_col;
  Terrain map[ROW][COL];
  Queue pq_tunel;
  Queue pq_nontunel;

  vector<Monster> mon;
  vector<Object> obj;
  //vector<Monster> *mon;
  //vector<Object> *obj;
};

extern Dungeon dungeon;

void init_dungeon();
void generate_dungeon();
void generate_dungeon_desc();
void print_dungeon();
void load_dungeon(FILE *f);
void save_dungeon(FILE *f);
void delete_dungeon();
void move_dungeon();

int get_random(int modulus, int min);
int is_visible_terrain(int i, int j);
int is_room_corridor_stair(int row, int col);
void remember_map_PC();
int is_monster(int row, int col);
int is_inside(int row, int col);

#endif
