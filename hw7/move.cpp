#include <cstring>

#include "dungeon.h"
#include "move.h"
#include "path.h"
#include "io.h"


/*
npc_move_func[c->npc->characteristics & 0x0000000f](d, c, next);

void (*npc_move_func[])(NPC *c) =
{
	npc_next_pos_00,
	npc_next_pos_01,
	npc_next_pos_02,
	npc_next_pos_03,
	npc_next_pos_04,
	npc_next_pos_05,
	npc_next_pos_06,
	npc_next_pos_07,
	npc_next_pos_08,
	npc_next_pos_09,
	npc_next_pos_0a,
	npc_next_pos_0b,
	npc_next_pos_0c,
	npc_next_pos_0d,
	npc_next_pos_0e,
	npc_next_pos_0f,
};
*/

void npc_next_pos_00(Character *npc, int index);
void npc_next_pos_01(Character *npc, int index);
void npc_next_pos_02(Character *npc, int index);
void npc_next_pos_03(Character *npc, int index);
void npc_next_pos_04(Character *npc, int index);

void npc_next_pos_05(Character *npc, int index)
{
	memset(npc->dist, 0, sizeof (npc->dist));

	dijkstra_nontunneling(npc);

	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};

	int current_v = npc->col + npc->row * COL;
	int min_v = current_v;
	int min_dist = npc->dist[min_v];

	int next_row = npc->row; int next_col = npc->col;
	for (int i = 0; i < 8; i++)
	{
		int next_v = current_v + rowMove[i] * COL + colMove[i];
		if (next_v < 0 || next_v > ROW * COL) continue;
		if (next_v / 80 == 0 || next_v % 80 == 0 ) continue;
		int next_dist = npc->dist[next_v];

		if (next_dist == -1) continue;

		if (next_dist < min_dist)
		{
			min_v = next_v;
			min_dist = npc->dist[min_v];
			next_row = npc->row + rowMove[i];
			next_col = npc->col + colMove[i];
		}
	}

    dungeon.map[npc->row][npc->col].space = dungeon.map[npc->row][npc->col].terrain;
	npc->row = next_row;
	npc->col = next_col;
    sprintf(&dungeon.map[npc->row][npc->col].space, "%x", npc->characteristics);
	dungeon.map[npc->row][npc->col].hardness = 0;

	//check is npc in next terrain will be dead
	for (int i = 0; i < dungeon.num_mon; i++)
	{
		if (i == index) continue;
		Character *next_npc = &dungeon.monster[i];
		if (next_npc->row == next_row && next_npc->col == next_col)
		{
			next_npc->dead = true;
			next_npc->row = -1;
			next_npc->col = -1;
		}
		else if (dungeon.pc.row == next_row && dungeon.pc.col == next_col)
		{
			dungeon.pc.dead = true;
			dungeon.pc.row = -1;
			dungeon.pc.col = -1;
		}
	}
}

void npc_next_pos_06(Character *npc, int index){}

void npc_next_pos_07(Character *npc, int index)
{
	memset(npc->dist, 0, sizeof (npc->dist));

	dijkstra_tunneling(npc);

	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};

	int current_v = npc->col + npc->row * COL;
	int min_v = current_v;
	int min_dist = npc->dist[min_v];

	int next_row = npc->row; int next_col = npc->col;
	for (int i = 0; i < 8; i++)
	{
		int next_v = current_v + rowMove[i] * COL + colMove[i];
		if (next_v < 0 || next_v > ROW * COL) continue;
		if (next_v / 80 == 0 || next_v % 80 == 0 ) continue;
		int next_dist = npc->dist[next_v];

		if (next_dist == -1) continue;

		if (next_dist < min_dist)
		{
			min_v = next_v;
			min_dist = npc->dist[min_v];
			next_row = npc->row + rowMove[i];
			next_col = npc->col + colMove[i];
		}
	}

	if (dungeon.map[next_row][next_col].hardness < 85)
	{
		if (dungeon.map[next_row][next_col].hardness != 0)
		{
			dungeon.map[next_row][next_col].hardness = 0;
		}
		npc->row = next_row;
		npc->col = next_col;
		if (dungeon.map[npc->row][npc->col].hardness == 0 &&
				dungeon.map[npc->row][npc->col].terrain == ROCK)
		{
			dungeon.map[npc->row][npc->col].terrain = CORRIDOR;
            dungeon.map[npc->row][npc->col].space = CORRIDOR;
		}
	}
	else
	{
		dungeon.map[next_row][next_col].hardness -= 85;
	}

	//check is npc in next terrain will be dead
	for (int i = 0; i < dungeon.num_mon; i++)
	{
		if (i == index) continue;
		Character *next_npc = &dungeon.monster[i];
		if (next_npc->row == next_row && next_npc->col == next_col)
		{
			next_npc->dead = true;
			next_npc->row = -1;
			next_npc->col = -1;
		}
		else if (dungeon.pc.row == next_row && dungeon.pc.col == next_col)
		{
			dungeon.pc.dead = true;
			dungeon.pc.row = -1;
			dungeon.pc.col = -1;
		}
	}
}

void npc_next_pos_08(Character *npc, int index);
void npc_next_pos_09(Character *npc, int index);
void npc_next_pos_0a(Character *npc, int index);
void npc_next_pos_0b(Character *npc, int index);
void npc_next_pos_0c(Character *npc, int index);
void npc_next_pos_0d(Character *npc, int index);
void npc_next_pos_0e(Character *npc, int index);
void npc_next_pos_0f(Character *npc, int index);

void move_npc()
{
	if (!dungeon.pc.dead)
	{
		int i;
		for (i = 0; i < dungeon.num_mon; i++)
		{
			Character *npc = &dungeon.monster[i];
			if (!npc->dead)
			{
				npc_next_pos_05(npc, i);
			}
		}
	}
}

const char *move_pc(int row_move, int col_move)
{
	const char *message;
	if (row_move == 0 && col_move == 0)
	{
		move_npc();
		message = "PC is resting!";
	}
	else if (is_inside(dungeon.pc.row + row_move, dungeon.pc.col + col_move) &&
		    is_room_corridor_stair(dungeon.pc.row + row_move, dungeon.pc.col + col_move))
	{
        dungeon.map[dungeon.pc.row][dungeon.pc.col].space = dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain;
		dungeon.pc.row += row_move;
		dungeon.pc.col += col_move;
        dungeon.map[dungeon.pc.row][dungeon.pc.col].space = PLAYER;
        dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = 0;
		if (!(is_monster(dungeon.pc.row, dungeon.pc.col) < 0))
		{
			int i = is_monster(dungeon.pc.row, dungeon.pc.col);
			dungeon.monster[i].dead = true;
			dungeon.monster[i].row = -1;
			dungeon.monster[i].col = -1;
		}
		move_npc();
        remember_map_PC();
		message = "";
	}
	else
	{
		message = "There's wall in the way!";
	}

	return message;
}

void move_character()
{
	dungeon_ncurses();
	//move_npc();
}

