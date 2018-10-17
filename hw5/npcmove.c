#include "npcmove.h"



void npc_next_pos_00(Character *npc, int index);
void npc_next_pos_01(Character *npc, int index);
void npc_next_pos_02(Character *npc, int index);
void npc_next_pos_03(Character *npc, int index);
void npc_next_pos_04(Character *npc, int index);

void npc_next_pos_05(Character *npc, int index)
{
	memset(npc->dist, 0, sizeof (npc->dist));

	dijkstra_nontunneling(npc->dist);

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

	npc->row = next_row;
	npc->col = next_col;

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
		else if (dungeon.PC.row == next_row && dungeon.PC.col == next_col)
		{
			dungeon.PC.dead = true;
			dungeon.PC.row = -1;
			dungeon.PC.col = -1;
		}
	}
}

void npc_next_pos_06(Character *npc, int index){}

void npc_next_pos_07(Character *npc, int index)
{
	memset(npc->dist, 0, sizeof (npc->dist));

	dijkstra_tunneling(npc->dist);

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
				dungeon.map[npc->row][npc->col].space == ROCK)
		{
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
		else if (dungeon.PC.row == next_row && dungeon.PC.col == next_col)
		{
			dungeon.PC.dead = true;
			dungeon.PC.row = -1;
			dungeon.PC.col = -1;
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

