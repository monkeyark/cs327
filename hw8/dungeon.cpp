#include <math.h>
#include <string.h>

#include "dungeon.h"
#include "move.h"
#include "path.h"
#include "parser.h"


void init_dungeon()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			dungeon.map[i][j].terrain = ROCK;
            dungeon.map[i][j].space = ROCK;
            dungeon.map[i][j].fog = ROCK;
			dungeon.map[i][j].hardness = ROCK_H;
			dungeon.pc.vision[i][j] = 0;
		}
	}
}

int get_random(int modulus, int min)
{
	return rand() % modulus + min;
}

int is_monster(int row, int col)
{
	for (int i = 0; i < dungeon.num_mon; i++)
	{
		if (row == dungeon.monster[i].row &&
			col == dungeon.monster[i].col &&
			!dungeon.monster[i].dead)
		{
			return i;
		}
	}
	return -1;
}

int is_item(int row, int col)
{
	for (int i = 0; i < dungeon.num_item; i++)
	{
		if (row == dungeon.item[i].row &&
			col == dungeon.item[i].col)
		{
			return i;
		}
	}
	return -1;
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

int is_inside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW - 1 && col < COL - 1;
}

int is_room(int row, int col)
{
	return dungeon.map[row][col].terrain == ROOM;
}

int is_room_corridor_stair(int row, int col)
{
	return dungeon.map[row][col].terrain == ROOM
		|| dungeon.map[row][col].terrain == CORRIDOR
		|| dungeon.map[row][col].terrain == STAIR_UP
		|| dungeon.map[row][col].terrain == STAIR_DOWN;
}

int is_valid_room(int row, int col, int width, int height)
{
	//is current terrain free
	if (dungeon.map[row][col].terrain != ROCK)
		return 0;
	if (!is_inside(row, col) || !is_inside(row + height, col + width))
		return 0;
	//touch or overlap another room
	for (int i = row - 1; i < row + height + 2; i++)
	{
		if (dungeon.map[i][col - 1].terrain != ROCK)
			return 0; //left touch
		if (dungeon.map[i][col].terrain != ROCK)
			return 0; //left overlap
		if (dungeon.map[i][col + width + 1].terrain != ROCK)
			return 0; //right touch
		if (dungeon.map[i][col + width].terrain != ROCK)
			return 0; //right overlap
	}
	for (int j = col - 1; j < col + width + 2; j++)
	{
		if (dungeon.map[row - 1][j].terrain != ROCK)
			return 0; //top touch
		if (dungeon.map[row][j].terrain != ROCK)
			return 0; //top overlap
		if (dungeon.map[row + height + 1][j].terrain != ROCK)
			return 0; //bottom touch
		if (dungeon.map[row + height][j].terrain != ROCK)
			return 0; //bottom overlap
	}

	return 1;
}

Room new_room_random()
{
	Room r;
	r.row = get_random(ROW, 0);
	r.col = get_random(COL, 0);
	r.width = get_random(7, 3);
	r.height = get_random(6, 2);

	int validRoom = is_valid_room(r.row, r.col, r.width, r.height);

	if (validRoom)
	{
		for (int i = r.row; i < r.row + r.height; i++)
		{
			for (int j = r.col; j < r.col + r.width; j++)
			{
				dungeon.map[i][j].terrain = ROOM;
                dungeon.map[i][j].space = ROOM;
				dungeon.map[i][j].hardness = ROOM_H;
			}
		}
	}
	else
	{
		return new_room_random();
	}

	return r;
}

void new_room(int row, int col, int width, int height)
{
	for (int i = row; i < row + height; i++)
	{
		for (int j = col; j < col + width; j++)
		{
			dungeon.map[i][j].terrain = ROOM;
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

int is_connected(int row, int col)
{
	return dungeon.map[row - 1][col].terrain == CORRIDOR
		|| dungeon.map[row + 1][col].terrain == CORRIDOR
		|| dungeon.map[row][col - 1].terrain == CORRIDOR
		|| dungeon.map[row][col + 1].terrain == CORRIDOR;
}

void new_corridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0)
		return;
	if (dungeon.map[aRow][aCol].terrain == CORRIDOR && is_connected(bRow, bCol))
		return;
	if (dungeon.map[aRow][aCol].terrain == ROCK)
    {
		dungeon.map[aRow][aCol].terrain = CORRIDOR;
        dungeon.map[aRow][aCol].space = CORRIDOR;
    }

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (is_inside(aRow - 1, aCol))
	{
		top = distance(aRow - 1, aCol, bRow, bCol);
		min = MIN(min, top);
	}
	if (is_inside(aRow + 1, aCol))
	{
		down = distance(aRow + 1, aCol, bRow, bCol);
		min = MIN(min, down);
	}
	if (is_inside(aRow, aCol - 1))
	{
		left = distance(aRow, aCol - 1, bRow, bCol);
		min = MIN(min, left);
	}
	if (is_inside(aRow, aCol + 1))
	{
		right = distance(aRow, aCol + 1, bRow, bCol);
		min = MIN(min, right);
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

void new_stair()
{
	int stair_up_row = get_random(ROW, 0);
	int stair_up_col = get_random(COL, 0);
	int stair_down_row = get_random(ROW, 0);
	int stair_down_col = get_random(COL, 0);

	if (is_room(stair_up_row, stair_up_col) &&
		is_room(stair_down_row, stair_down_col))
	{
		dungeon.map[stair_up_row][stair_up_col].terrain = STAIR_UP;
        dungeon.map[stair_up_row][stair_up_col].space = STAIR_UP;
		dungeon.map[stair_up_row][stair_up_col].hardness = 0;
		dungeon.map[stair_down_row][stair_down_col].terrain = STAIR_DOWN;
        dungeon.map[stair_down_row][stair_down_col].space = STAIR_DOWN;
		dungeon.map[stair_down_row][stair_down_col].hardness = 0;
	}
	else
	{
		new_stair();
	}
	
}

int is_visible_terrain(int i, int j)
{
    return i >= dungeon.pc.row - PC_VISION_RADIUS &&
           i <= dungeon.pc.row + PC_VISION_RADIUS &&
           j >= dungeon.pc.col - PC_VISION_RADIUS &&
           j <= dungeon.pc.col + PC_VISION_RADIUS;
}

void remember_map_PC()
{
    int row, col;
    for (row = dungeon.pc.row - PC_VISION_RADIUS; row < dungeon.pc.row + PC_VISION_RADIUS + 1; row++)
    {
        for (col = dungeon.pc.col - PC_VISION_RADIUS; col < dungeon.pc.col + PC_VISION_RADIUS + 1; col++)
        {
            if (row >= 0 && col >= 0 && row < ROW && col < COL)
            {
                dungeon.pc.vision[row][col] = 1;
				//if (is_monster(row, col) && is_visible_terrain(row, col))
                if (is_visible_terrain(row, col))
                {
                    dungeon.map[row][col].fog = dungeon.map[row][col].space;
                }
                else
                {
                    dungeon.map[row][col].fog = dungeon.map[row][col].terrain;
                }
            }
        }
    }

}

NPC new_NPC(int birth)
{
	NPC npc;
	int row = get_random(ROW, 0);
	int col = get_random(COL, 0);
	//add monster into map
	if (dungeon.map[row][col].space == ROOM ||
		dungeon.map[row][col].space == CORRIDOR)
	{
		npc.row = row;
		npc.col = col;
		npc.birth = birth;
		//creating NPC with all four ability having 1/2 probability, clean unused bits
		npc.ability = rand() & 0xf;
		npc.speed = get_random(20, 5);
		npc.dead = 0;
		if (npc.ability & NPC_TELEPATH) //monster is telepath
		{
			npc.pc_row = dungeon.pc.row;
			npc.pc_col = dungeon.pc.col;
		}
		
		sprintf(&npc.symbol, "%x", npc.ability);
		dungeon.map[npc.row][npc.col].space = npc.symbol;
		dungeon.map[npc.row][npc.col].hardness = 0;
	}
	else
	{
		return new_NPC(birth);
	}

	return npc;
}

void new_PC()
{
	//add initial player location
	dungeon.pc.birth = -1;
	dungeon.pc.speed = 10;
	dungeon.pc.dead = 0;
	dungeon.pc.row = dungeon.rooms[0].row;
	dungeon.pc.col = dungeon.rooms[0].col;
    dungeon.map[dungeon.pc.row][dungeon.pc.col].space = PLAYER;
	dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = 0;

    remember_map_PC();
}

void generate_dungeon()
{
	//initialize dungeon
	init_dungeon();

    //generate random number of rooms
	dungeon.num_room = get_random(7, 5);
	//generate random number of monster
    if (!dungeon.num_mon)
    {
	    dungeon.num_mon = get_random(5, 8);
    }

    dungeon.rooms = (Room *) malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = (NPC *) malloc(dungeon.num_mon * sizeof(NPC));
	int i;

	//add rooms
	for (i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i] = new_room_random();
	}

	//add corridors
	for (i = 0; i < dungeon.num_room - 1; i++)
	{
		new_corridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
	}

	//add stair
	new_stair();
	
	//add pc
	new_PC();

	//add npc
	for (i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = new_NPC(i);
	}
}

void new_PC_desc()
{
	//add initial player location
	dungeon.pc.birth = -1;
	dungeon.pc.speed = 10;
	dungeon.pc.dead = 0;
	dungeon.pc.row = dungeon.rooms[0].row;
	dungeon.pc.col = dungeon.rooms[0].col;
    dungeon.map[dungeon.pc.row][dungeon.pc.col].space = PLAYER;
	dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = 0;
	dungeon.pc.item = (Item *) malloc(dungeon.num_item * sizeof(Item));
    remember_map_PC();
}

NPC new_NPC_desc(int birth)
{
	NPC npc;
	int row = get_random(ROW, 0);
	int col = get_random(COL, 0);
	//add monster into map
	if (dungeon.map[row][col].space == ROOM ||
		dungeon.map[row][col].space == CORRIDOR)
	{
		int index, rarity;
		bool is_unique = false;
		Monster mons;
		do
		{
			index = get_random(dungeon.mon.size(), 0);
			rarity = get_random(99, 0);
			mons = dungeon.mon.at(index);
			dungeon.mon.at(index).seen = true;
			// is_unique = NPC_UNIQ & dungeon.mon.at(index).ability;
			// std::cout << "rarity--------------- " << rarity << std::endl;
			// std::cout << "mons.rarity---------- " << mons.rarity << std::endl;
			// std::cout << "mons.seen------------ " << mons.seen << std::endl;
			// std::cout << "is_unique------------ " << is_unique << std::endl;
			// std::cout << std::endl;
		}
		while ((rarity >= mons.rarity) || (dungeon.mon.at(index).seen && is_unique));

		npc.row = row;
		npc.col = col;
		npc.birth = birth;
		npc.dead = 0;

		//creating NPC with all four ability having 1/2 probability, clean unused bits
		npc.ability = mons.ability;
		npc.damage = mons.damage;
		npc.symbol = mons.symbol;
		npc.color = mons.color;
		npc.color_display = mons.color_display;
		npc.speed = mons.speed.roll();

		if (npc.ability & NPC_TELEPATH) //monster is telepath
		{
			npc.pc_row = dungeon.pc.row;
			npc.pc_col = dungeon.pc.col;
		}

		dungeon.map[npc.row][npc.col].space = npc.symbol;
	}
	else
	{
		return new_NPC_desc(birth);
	}

	return npc;
}

bool is_inventory(Item item)
{
	/*
	for (int i = 0; i < sizeof(dungeon.item)/sizeof(item); i++)
	{
		if (dungeon.item[i].inventory)
		{
			return true;
		}
	}
	*/
	return false;
}

Item new_item_desc()
{
	Item item;
	int row = get_random(ROW, 0);
	int col = get_random(COL, 0);
	//add object into map

	//TODO
	if (dungeon.map[row][col].space == ROOM ||
		dungeon.map[row][col].space == CORRIDOR)
	{
		//get random index of object from description
		//bool inventory = false;//TODO generated artifact
		int index, rarity;
		Object obj;
		do
		{
			index = get_random(dungeon.obj.size(), 0);
			rarity = get_random(99, 0);
			obj = dungeon.obj.at(index);
			dungeon.obj.at(index).seen = true;
		}
		while((rarity >= obj.rarity) || (dungeon.obj.at(index).seen && dungeon.obj.at(index).artifact));

		item.row = row;
		item.col = col;

		item.name = &obj.name;
		item.description = &obj.description;
		item.damage = obj.damage;
		item.color = obj.color;
		item.color_display = obj.color_display;
		item.hit = obj.hit.roll();
		item.dodge = obj.dodge.roll();
		item.defence = obj.defence.roll();
		item.weight = obj.weight.roll();
		item.speed = obj.speed.roll();
		item.attribute = obj.attribute.roll();
		item.value = obj.value.roll();
		item.artifact = obj.artifact;
		item.rarity = obj.rarity;
		item.type = &obj.type;
		item.symbol = obj.symbol;

		dungeon.map[item.row][item.col].space = item.symbol;
	}
	else
	{
		return new_item_desc();
	}
	return item;
}

void generate_dungeon_desc()
{
	char *home = getenv("HOME");
	const char *path = strcat(home, "");
    char path_monster[sizeof(path)*5];
	char path_object[sizeof(path)*5];
    strcpy(path_monster, path);
	strcpy(path_object, path);
    strcat(path_monster, "/monster_desc.txt");
	strcat(path_object, "/object_desc.txt");
	load_monster_desc(path_monster);
	load_object_desc(path_object);

	//initialize dungeon
	init_dungeon();

    //generate random number of rooms
	dungeon.num_room = get_random(7, 5);
	//generate random number of monster
    if (!dungeon.num_mon)
    {
	    dungeon.num_mon = get_random(5, 8);
    }
	dungeon.num_item = get_random(5, 10);

    dungeon.rooms = (Room *) malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = (NPC *) malloc(dungeon.num_mon * sizeof(NPC));
	dungeon.item = (Item *) malloc(dungeon.num_item * sizeof(Item));

	int i;
	//add rooms
	for (i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i] = new_room_random();
	}

	//add corridors
	for (i = 0; i < dungeon.num_room - 1; i++)
	{
		new_corridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
	}

	//add stair
	new_stair();

	//add pc
	new_PC_desc();

	//add npc
	for (i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = new_NPC_desc(i);
	}

	//add object
	for (i = 0; i < dungeon.num_item; i++)
	{
		dungeon.item[i] = new_item_desc();
	}
}

void load_dungeon(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	init_dungeon();

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
	dungeon.pc.col = pc_col;
	uint8_t pc_row;
	fread(&pc_row, 1, 1, f);
	dungeon.pc.row = pc_row;

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
				dungeon.map[row][col].terrain = CORRIDOR;
			}
			else
			{
				dungeon.map[row][col].terrain = ROCK;
			}
		}
	}

	dungeon.num_room = (filesize - 1702) / 4;
    dungeon.rooms = (Room *) malloc(dungeon.num_room * sizeof(Room));

	uint8_t roomRead[filesize - 1702];
	fread(roomRead, 1, filesize - 1702, f);

	int n = 0;
	for (int i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i].col = roomRead[n++];
		dungeon.rooms[i].row = roomRead[n++];
		dungeon.rooms[i].width = roomRead[n++];
		dungeon.rooms[i].height = roomRead[n++];

		new_room(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i].width, dungeon.rooms[i].height);
	}

	//add PC
	dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain = PLAYER;
	dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = PC_H;

	fclose(f);
}

void save_dungeon(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	//char *marker = "RLG327-F2018";
    const char *marker = "RLG327-F2018";
	fwrite(marker, 1, 12, f);

	int ver = htobe32(dungeon.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * dungeon.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = dungeon.pc.col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = dungeon.pc.row;
	fwrite(&pc_y, 1, 1, f);

	char *hard = (char *) malloc(1680);
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
	unsigned char *loc = (unsigned char *) malloc(4 * dungeon.num_room);
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

void delete_dungeon()
{
    free(dungeon.rooms);
	free(dungeon.monster);	
}

void delete_dungeon_desc()
{
    free(dungeon.rooms);
	free(dungeon.monster);
	free(dungeon.item);
	
	//TODO memory leak 
	/*
	for (unsigned int i = 0; i < dungeon.mon.size(); i++)
	{
		Monster m = dungeon.mon.at(i);
		delete &m;
	}
	for (unsigned int i = 0; i < dungeon.obj.size(); i++)
	{
		Object object = dungeon.obj.at(i);
		delete &object;
	}
	*/
}

void move_dungeon()
{
	move_character();
	//move_character_turn();
}
