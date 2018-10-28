#ifndef MOVE_H
# define MOVE_H

#include "dungeon.h"

void npc_next_pos_00(Character *npc, int index);
void npc_next_pos_01(Character *npc, int index);
void npc_next_pos_02(Character *npc, int index);
void npc_next_pos_03(Character *npc, int index);
void npc_next_pos_04(Character *npc, int index);
void npc_next_pos_05(Character *npc, int index);
void npc_next_pos_06(Character *npc, int index);
void npc_next_pos_07(Character *npc, int index);
void npc_next_pos_08(Character *npc, int index);
void npc_next_pos_09(Character *npc, int index);
void npc_next_pos_0a(Character *npc, int index);
void npc_next_pos_0b(Character *npc, int index);
void npc_next_pos_0c(Character *npc, int index);
void npc_next_pos_0d(Character *npc, int index);
void npc_next_pos_0e(Character *npc, int index);
void npc_next_pos_0f(Character *npc, int index);

void move_npc();
const char *move_pc(int row_move, int col_move);
void move_character();
#endif