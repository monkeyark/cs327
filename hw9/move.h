#ifndef MOVE_H
# define MOVE_H

#include "dungeon.h"

void npc_next_pos_00(NPC *npc, int index);
void npc_next_pos_01(NPC *npc, int index);
void npc_next_pos_02(NPC *npc, int index);
void npc_next_pos_03(NPC *npc, int index);
void npc_next_pos_04(NPC *npc, int index);
void npc_next_pos_05(NPC *npc, int index);
void npc_next_pos_06(NPC *npc, int index);
void npc_next_pos_07(NPC *npc, int index);
void npc_next_pos_08(NPC *npc, int index);
void npc_next_pos_09(NPC *npc, int index);
void npc_next_pos_0a(NPC *npc, int index);
void npc_next_pos_0b(NPC *npc, int index);
void npc_next_pos_0c(NPC *npc, int index);
void npc_next_pos_0d(NPC *npc, int index);
void npc_next_pos_0e(NPC *npc, int index);
void npc_next_pos_0f(NPC *npc, int index);

void move_npc();
const char *move_pc(int row_move, int col_move);
void move_character();
#endif