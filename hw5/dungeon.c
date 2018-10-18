#include "queue.h"
#include "dungeon.h"
#include "npcmove.h"

void init_dungeon()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			dungeon.map[i][j].space = ROCK;
			dungeon.map[i][j].hardness = ROCK_H;
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

void print_dungeon()
{
	//printf("\nnpc_row = %d; npc_col = %d\n", dungeon.monster[0].row, dungeon.monster[0].col);//TODO
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
			if (i == dungeon.PC.row && j == dungeon.PC.col)
			{
				printf("@");
			}
			else if (!(is_monster(i, j) < 0))
			{
				printf("%x", dungeon.monster[is_monster(i, j)].characteristics);
			}
			else
			{
				printf("%c", dungeon.map[i][j].space);
			}
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

bool is_inside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW - 1 && col < COL - 1;
}

bool is_room(int row, int col)
{
	return dungeon.map[row][col].space == ROOM;
}

bool is_room_corridor_stair(int row, int col)
{
	return dungeon.map[row][col].space == ROOM
		|| dungeon.map[row][col].space == CORRIDOR
		|| dungeon.map[row][col].space == STAIR_UP
		|| dungeon.map[row][col].space == STAIR_DOWN;
}

bool is_valid_room(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon.map[row][col].space != ROCK)
		return false;
	if (!is_inside(row, col) || !is_inside(row + height, col + width))
		return false;
	//touch or overlap another room
	for (int i = row - 1; i < row + height + 2; i++)
	{
		if (dungeon.map[i][col - 1].space != ROCK)
			return false; //left touch
		if (dungeon.map[i][col].space != ROCK)
			return false; //left overlap
		if (dungeon.map[i][col + width + 1].space != ROCK)
			return false; //right touch
		if (dungeon.map[i][col + width].space != ROCK)
			return false; //right overlap
	}
	for (int j = col - 1; j < col + width + 2; j++)
	{
		if (dungeon.map[row - 1][j].space != ROCK)
			return false; //top touch
		if (dungeon.map[row][j].space != ROCK)
			return false; //top overlap
		if (dungeon.map[row + height + 1][j].space != ROCK)
			return false; //bottom touch
		if (dungeon.map[row + height][j].space != ROCK)
			return false; //bottom overlap
	}

	return true;
}

Room new_room_random()
{
	Room r;
	r.row = get_random(ROW, 0);
	r.col = get_random(COL, 0);
	r.width = get_random(7, 3);
	r.height = get_random(6, 2);

	bool validRoom = is_valid_room(r.row, r.col, r.width, r.height);

	if (validRoom)
	{
		for (int i = r.row; i < r.row + r.height; i++)
		{
			for (int j = r.col; j < r.col + r.width; j++)
			{
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

bool is_connected(int row, int col)
{
	return dungeon.map[row - 1][col].space == CORRIDOR
		|| dungeon.map[row + 1][col].space == CORRIDOR
		|| dungeon.map[row][col - 1].space == CORRIDOR
		|| dungeon.map[row][col + 1].space == CORRIDOR;
}

void new_corridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0)
		return;
	if (dungeon.map[aRow][aCol].space == CORRIDOR && is_connected(bRow, bCol))
		return;
	if (dungeon.map[aRow][aCol].space == ROCK)
		dungeon.map[aRow][aCol].space = CORRIDOR;

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
		dungeon.map[stair_up_row][stair_up_col].space = STAIR_UP;
		dungeon.map[stair_up_row][stair_up_col].hardness = 0;
		dungeon.map[stair_down_row][stair_down_col].space = STAIR_DOWN;
		dungeon.map[stair_down_row][stair_down_col].hardness = 0;
	}
	else
	{
		new_stair();
	}
	
}

Character new_NPC(int birth)
{
	Character npc;
	int row = get_random(ROW, 0);
	int col = get_random(COL, 0);
	//add monster into map
	if (dungeon.map[row][col].space == ROOM ||
		dungeon.map[row][col].space == CORRIDOR)
	{
		npc.row = row;
		npc.col = col;
		npc.birth = birth;
		//creating NPC with all four characteristics having 1/2 probability, clean unused bits
		npc.characteristics = rand() & 0xf;
		npc.speed = get_random(20, 5);
		npc.dead = false;
		if (npc.characteristics & NPC_TELEPATH) //monster is telepath
		{
			npc.pc_row = dungeon.PC.row;
			npc.pc_col = dungeon.PC.col;
		}

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
	dungeon.PC.birth = -1;
	dungeon.PC.speed = 10;
	dungeon.PC.dead = false;
	dungeon.PC.row = dungeon.rooms[0].row;
	dungeon.PC.col = dungeon.rooms[0].col;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = 0;
}

void generate_dungeon()
{
	//initialize dungeon
	init_dungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = malloc(dungeon.num_mon * sizeof(Character));
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

	//add npc
	for (i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = new_NPC(i);
	}
	
	//add pc
	new_PC();
}

void load_file(FILE *f)
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
	dungeon.PC.col = pc_col;
	uint8_t pc_row;
	fread(&pc_row, 1, 1, f);
	dungeon.PC.row = pc_row;

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
				dungeon.map[row][col].space = CORRIDOR;
			}
			else
			{
				dungeon.map[row][col].space = ROCK;
			}
		}
	}

	dungeon.num_room = (filesize - 1702) / 4;
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));

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
	dungeon.map[dungeon.PC.row][dungeon.PC.col].space = PLAYER;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = PC_H;

	fclose(f);
}

void save_file(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	char *marker = "RLG327-F2018";
	fwrite(marker, 1, 12, f);

	int ver = htobe32(dungeon.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * dungeon.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = dungeon.PC.col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = dungeon.PC.row;
	fwrite(&pc_y, 1, 1, f);

	char *hard = malloc(1680);
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
	unsigned char *loc = malloc(4 * dungeon.num_room);
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

int get_hardness_cost(int hardness)
{
	if (hardness == 255)
		return 3;
	if (hardness == 0)
		return 1;
	return 1 + (hardness / 85);
}

void print_dijkstra_path(int dist[ROW * COL])
{
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
	for (int i = 0; i < COL; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");
	//TODO above
	//	putchar('\n');
	int i, j;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.PC.row == i && dungeon.PC.col == j)
			{
				printf("%c", PLAYER);
			}
			else if (dist[i * COL + j] != -1)
			{
				int n = dist[i * COL + j] % 10;
				printf("%d", n);
			}
			else
			{
				printf("%c", ROCK);
			}
		}
		printf("\n");
	}
	putchar('\n');
}

void dijkstra_tunneling(int dist[ROW * COL])
{
	int rowMove[8] = {-1, -1, -1, 0, +1, +1, +1, 0};
	int colMove[8] = {-1, 0, +1, +1, +1, 0, -1, -1};
	int i, j;
	//Queue pq_tunel;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (!is_inside(i, j))
			{
				dist[i * COL + j] = -1;
			}
			else if (dungeon.map[i][j].space != PLAYER)
			{
				dist[i * COL + j] = ROW * COL + 1;
				pq_insert(dungeon.pq_tunel, &node, i * COL + j, dist);
			}
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;

	while (!pq_isEmpty(dungeon.pq_tunel, &node))
	{
		int u = pq_pop(dungeon.pq_tunel, &node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i] * COL;
			if (0 > v || v > ROW * COL || dist[v] == -1)
				continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + get_hardness_cost(dungeon.map[u / COL][u % COL].hardness);
				if (alt < dist[v])
				{
					dist[v] = alt;
					pq_insert(dungeon.pq_tunel, &node, v, dist);
				}
			}
		}
	}
	//print_dijkstra_path(dist);
}

void dijkstra_nontunneling(int dist[ROW * COL])
{
	int rowMove[8] = {-1, -1, -1, 0, +1, +1, +1, 0};
	int colMove[8] = {-1, 0, +1, +1, +1, 0, -1, -1};
	int i, j;
	//Queue pq_nontunel;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.map[i][j].space == ROOM ||
				dungeon.map[i][j].space == CORRIDOR ||
				dungeon.map[i][j].space == STAIR_UP ||
				dungeon.map[i][j].space == STAIR_DOWN)
			{
				dist[i * COL + j] = ROW * COL + 1;
				pq_insert(dungeon.pq_nontunel, &node, i * COL + j, dist);
			}
			else if (dungeon.map[i][j].space == ROCK)
			{
				dist[i * COL + j] = -1;
			}
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;

	while (!pq_isEmpty(dungeon.pq_nontunel, &node))
	{
		int u = pq_pop(dungeon.pq_nontunel, &node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i] * COL;
			if (0 > v || v > ROW * COL)
				continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + 1;
				if (alt < dist[v])
				{
					dist[v] = alt;
					pq_insert(dungeon.pq_nontunel, &node, v, dist);
				}
			}
		}
	}
	//print_dijkstra_path(dist);
}

/*
void move_character_turn()
{
	Queue_npc pq;
	Character pc;
	int pc_turn = 0;
	int npc_turn = 0;
	Node_t *character = node_new_NPC(pc_turn, pc);

	for (int i = 0; i < dungeon.num_mon; i++)
	{
		Node_t *new = node_new_NPC(npc_turn, dungeon.monster[i]);
		pq_insert_NPC(pq, &character, &new);
	}

	while (!dungeon.PC.dead)
	{
		Node_t *n = pq_pop_NPC(pq, &character);
		//TODO
		if ((*n).character.row == dungeon.PC.row && (*n).character.col == dungeon.PC.col)
		{
			usleep(250000);
			print_dungeon();
		}
		else if ((*n).character.characteristics & NPC_TELEPATH)
			pq_insert_NPC(pq, &character, &n);
	}
}
*/
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

/*
void print_dungeon_ncurses_debug(WINDOW *game)
{
	//printf("\nnpc_row = %d; npc_col = %d\n", dungeon.monster[0].row, dungeon.monster[0].col);//TODO
	wprintw(game, "   ");
	for (int i = 0; i < COL; i++)
	{
		int row = (i - i % 10) / 10;
		if (i % 10 == 0)
		{
			wprintw(game, "%d", row);
		}
		else
		{
			wprintw(game, " ");
		}
	}
	wprintw(game, "\n");

	wprintw(game, "   ");
	for (int i = 0; i < COL; i++)
	{
		wprintw(game, "%d", i % 10);
	}
	wprintw(game, "\n ");

	for (int i = 0; i < COL + 2; i++)
	{
		wprintw(game, "-");
	}
	wprintw(game, "\n");

	for (int i = 0; i < ROW; i++)
	{
		wprintw(game, "%2d|", i);
		for (int j = 0; j < COL; j++)
		{
			if (is_monster(i, j))
			{
				wprintw(game, "%x", dungeon.monster[is_monster(i, j) - 1].characteristics);
			}
			else
			{
				wprintw(game, "%c", dungeon.map[i][j].space);
			}
		}
		wprintw(game, "|\n");
	}
	wprintw(game, "  ");
	for (int i = 0; i < COL + 2; i++)
	{
		wprintw(game, "-");
	}
	wprintw(game, "\n");
}
*/
void print_dungeon_ncurses(WINDOW *game, char *message)
{
	int i, j;
	//clean previous message
	for (i = 0, j = 0; j < TERMINAL_COL; j++)
	{
		mvwprintw(game, i, j, " ");
	}

	//print current message
	char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		mvwprintw(game, i, j, m);
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (i == dungeon.PC.row && j == dungeon.PC.col)
			{
				mvwprintw(game, i, j, "@");
			}
			else if (!(is_monster(i, j) < 0))
			{
				mvwprintw(game, i, j, "%x", dungeon.monster[is_monster(i, j)].characteristics);
			}
			else
			{
				mvwprintw(game, i, j, "%c", dungeon.map[i][j].space);
			}
		}
		//mvwprintw(game, i, COL, "\n");
	}
}

void print_monster_list_ncurses(WINDOW *list, int start)
{
	int i, j, n;
	char *m;
	char str[COL];
	for (i = 0, j = start; i < ROW && j < dungeon.num_mon - start; i++, j++)
	{

		Character npc = dungeon.monster[j];
		int row_dis = npc.row - dungeon.PC.row;
		int col_dis = npc.col - dungeon.PC.col;
		char *row_pos, *col_pos;

		if (row_dis > 0)
		{
			row_pos = "South";
		}
		else if (row_dis < 0)
		{
			row_pos = "North";
		}
		else
		{
			row_pos = "     ";
		}

		if (col_dis > 0)
		{
			col_pos = " East";
		}
		else if (col_dis < 0)
		{
			col_pos = " West";
		}
		else
		{
			col_pos = "     ";
		}

		sprintf(str, "%x:  %2d %s and %2d %s", npc.characteristics, abs(row_dis), row_pos, abs(col_dis), col_pos);
		m = str;

		for (n = 0; *m; m++, n++)
		{
			mvwprintw(list, i, n, m);
		}
	}

}

void monster_list()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	bool run = true;
	int index = 0;
	while(run)
	{
		print_monster_list_ncurses(list, index);
		int key = wgetch(list);
		switch(key)
		{
			case KEY_UP:
				index++;
				index = MIN(index, dungeon.num_mon);
				break;
			case KEY_DOWN:
				index--;
				index = MAX(index, 0);
				break;
			case 27:
				run = false;
				break;

		}
	}

	clrtoeol();
	refresh();
	endwin();
}

void move_npc()
{
	if (!dungeon.PC.dead)
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

char *move_pc(int row_move, int col_move)
{
	char *message;
	if (row_move == 0 && col_move == 0)
	{
		move_npc();
		message = "PC is resting!";
	}
	else if (is_inside(dungeon.PC.row + row_move, dungeon.PC.col + col_move) &&
		is_room_corridor_stair(dungeon.PC.row + row_move, dungeon.PC.col + col_move))
	{
		dungeon.PC.row += row_move;
		dungeon.PC.col += col_move;
		if (!(is_monster(dungeon.PC.row, dungeon.PC.col) < 0))
		{
			int i = is_monster(dungeon.PC.row, dungeon.PC.col);
			dungeon.monster[i].dead = true;
			dungeon.monster[i].row = -1;
			dungeon.monster[i].col = -1;
		}
		move_npc();
		message = "";
	}
	else
	{
		message = "There's wall in the way!";
	}

	return message;
}

void dungeon_ncurses()
{
	initscr();
	noecho();
	cbreak();
	WINDOW *game = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);

	keypad(game, true);
	bool run = true;
	char *message = "Dungeon";
	while(run)
	{
		print_dungeon_ncurses(game, message);
		int key = wgetch(game);
		switch(key)
		{
			case KEY_HOME:
				message = move_pc(-1, -1);//move up-left
				break;
			case KEY_UP:
				message = move_pc(-1, 0);//move up
				break;
			case KEY_PPAGE:
				message = move_pc(-1, 1);//move up-right
				break;
			case KEY_RIGHT:
				message = move_pc(0, 1);//move right
				break;
			case KEY_NPAGE:
				message = move_pc(1, 1);//move down-right
				break;
			case KEY_DOWN:
				message = move_pc(1, 0);//move down
				break;
			case KEY_END:
				message = move_pc(1, -1);//move down-left
				break;
			case KEY_LEFT:
				message = move_pc(0, -1);//move left
				break;
			case KEY_B2:
				message = move_pc(0, 0);//rest
				break;
			case ' ':
				message = move_pc(0, 0);//rest
				break;
			case '<':
				if (dungeon.map[dungeon.PC.row][dungeon.PC.col].space == STAIR_UP)
				{
					// pq_delete(dungeon.pq_nontunel);
					// pq_delete(dungeon.pq_tunel);
					//TODO BUGFIX clean monster queue
					free(dungeon.rooms);
					free(dungeon.monster);
					generate_dungeon();
					message = "You went up stair";
				}
				else
				{
					message = "You are not standing on up stair";
				}
				break;
			case '>':
				if (dungeon.map[dungeon.PC.row][dungeon.PC.col].space == STAIR_DOWN)
				{
					// pq_delete(dungeon.pq_nontunel);
					// pq_delete(dungeon.pq_tunel);
					//TODO BUGFIX clean monster queue
					free(dungeon.rooms);
					free(dungeon.monster);
					generate_dungeon();
					message = "You went down stair";
				}
				else
				{
					message = "You are not standing on down stair";
				}
				break;
			case '.':
				message = move_pc(0, 0);//rest
				break;
			case '1':
				message = move_pc(1, -1);//move down-left
				break;
			case '2':
				message = move_pc(-1, 0);//move up
				break;
			case '3':
				message = move_pc(1, 1);//move down-right
				break;
			case '4':
				message = move_pc(0, -1);//move left
				break;
			case '5':
				message = move_pc(0, 0);//rest
				break;
			case '6':
				message = move_pc(0, 1);//move right
				break;
			case '7':
				message = move_pc(-1, -1);//move up-left
				break;
			case '8':
				message = move_pc(-1, 0);//move up
				break;
			case '9':
				message = move_pc(-1, 1);//move up-right
				break;
			case 'b':
				message = move_pc(1, -1);//move down-left
				break;
			case 'c':
			//TODO
				break;
			case 'd':
			//TODO
				break;
			case 'e':
			//TODO
				break;
			case 'f':
			//TODO
				break;
			case 'g':
			//TODO
				break;
			case 'h':
				message = move_pc(0, -1);//move left
				break;
			case 'i':
			//TODO
				break;
			case 'j':
				message = move_pc(1, 0);//move down
				break;
			case 'k':
				message = move_pc(-1, 0);//move up
				break;
			case 'l':
				message = move_pc(0, 1);//move right
				break;
			case 'm':
				monster_list();
				break;
			case 'n':
				message = move_pc(1, 1);//move down-right
				break;
			case 's':
			//TODO
				break;
			case 't':
			//TODO
				break;
			case 'u':
				message = move_pc(-1, 1);//move up-right
				break;
			case 'w':
			//TODO
				break;
			case 'x':
			//TODO
				break;
			case 'y':

				break;
			case 'D':
			//TODO
				break;
			case 'E':
			//TODO
				break;
			case 'H':
			//TODO
				break;
			case 'I':
			//TODO
				break;
			case 'L':
			//TODO
				break;
			case 'Q':
				run = false;
				break;
			case 'T':
			//TODO
				break;
		}

		if (dungeon.PC.dead)
		{
			message = "Player is dead!";
		}
	}

	clrtoeol();
	refresh();
	endwin();
}

void move_character()
{
	dungeon_ncurses();
	//move_npc();
}

void move_dungeon()
{
	move_character();
	//move_character_turn();
}
