#include "queue.h"
#include "dungeon.h"
#include "npcmove.h"

void initDungeon()
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

int getRandom(int modulus, int min)
{
	return rand() % modulus + min;
}

int isMonster(int row, int col)
{
	for (int i = 0; i < dungeon.num_mon; i++)
	{
		if (row == dungeon.monster[i].row && col == dungeon.monster[i].col)
		{
			return i;
		}
	}
	return -1;
}

void printDungeon()
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
			else if (!(isMonster(i, j) < 0))
			{
				printf("%x", dungeon.monster[isMonster(i, j)].characteristics);
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

bool isInside(int row, int col)
{
	//is room not on edge or outside of dungeon or cross dungeon
	return row > 0 && col > 0 && row < ROW - 1 && col < COL - 1;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon.map[row][col].space != ROCK)
		return false;
	if (!isInside(row, col) || !isInside(row + height, col + width))
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

Room newRoom()
{
	Room r;
	r.row = getRandom(ROW, 0);
	r.col = getRandom(COL, 0);
	r.width = getRandom(7, 3);
	r.height = getRandom(6, 2);

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);

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
		return newRoom();
	}

	return r;
}

void addRoom(int row, int col, int width, int height)
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

bool isConnected(int row, int col)
{
	return dungeon.map[row - 1][col].space == CORRIDOR || dungeon.map[row + 1][col].space == CORRIDOR || dungeon.map[row][col - 1].space == CORRIDOR || dungeon.map[row][col + 1].space == CORRIDOR;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (distance(aRow, aCol, bRow, bCol) == 0)
		return;
	if (dungeon.map[aRow][aCol].space == CORRIDOR && isConnected(bRow, bCol))
		return;
	if (dungeon.map[aRow][aCol].space == ROCK)
		dungeon.map[aRow][aCol].space = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (isInside(aRow - 1, aCol))
	{
		top = distance(aRow - 1, aCol, bRow, bCol);
		min = MIN(min, top);
	}
	if (isInside(aRow + 1, aCol))
	{
		down = distance(aRow + 1, aCol, bRow, bCol);
		min = MIN(min, down);
	}
	if (isInside(aRow, aCol - 1))
	{
		left = distance(aRow, aCol - 1, bRow, bCol);
		min = MIN(min, left);
	}
	if (isInside(aRow, aCol + 1))
	{
		right = distance(aRow, aCol + 1, bRow, bCol);
		min = MIN(min, right);
	}

	if (min == top)
	{
		newCorridor(aRow - 1, aCol, bRow, bCol);
	}
	else if (min == down)
	{
		newCorridor(aRow + 1, aCol, bRow, bCol);
	}
	else if (min == left)
	{
		newCorridor(aRow, aCol - 1, bRow, bCol);
	}
	else if (min == right)
	{
		newCorridor(aRow, aCol + 1, bRow, bCol);
	}
}

Character new_NPC(int birth)
{
	Character npc;
	int row = getRandom(ROW, 0);
	int col = getRandom(COL, 0);
	//add monster into map
	if (dungeon.map[row][col].space == ROOM)
	//if (dungeon.map[npc.row][npc.col].hardness == 0)
	{
		npc.row = row;
		npc.col = col;
		npc.birth = birth;
		//creating NPC with all four characteristics having 1/2 probability, clean unused bits
		npc.characteristics = rand() & 0xf;
		npc.speed = getRandom(20, 5);
		npc.dead = false;
		if (npc.characteristics & NPC_TELEPATH) //monster is telepath
		{
			npc.pc_row = dungeon.PC.row;
			npc.pc_col = dungeon.PC.col;
		}

		//printf("npc.characteristics = %d in hex: %x\n", npc.characteristics, npc.characteristics);

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

void generateDungeon()
{
	//initialize dungeon
	initDungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = malloc(dungeon.num_mon * sizeof(Character));
	int i = 0;
	for (i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i] = newRoom();
	}

	for (i = 0; i < dungeon.num_room - 1; i++)
	{
		newCorridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
	}

	for (int i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = new_NPC(i);
	}
	
	new_PC();
}

void loadFile(FILE *f)
{
	if (!f)
	{
		fprintf(stderr, "Failed to open file\n");
		return;
	}

	initDungeon();

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

		addRoom(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i].width, dungeon.rooms[i].height);
	}

	//add PC
	dungeon.map[dungeon.PC.row][dungeon.PC.col].space = PLAYER;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = PC_H;

	fclose(f);
}

void saveFile(FILE *f)
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

int getHardnessCost(int hardness)
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
	Queue pq_tunel;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (!isInside(i, j))
			{
				dist[i * COL + j] = -1;
			}
			else if (dungeon.map[i][j].space != PLAYER)
			{
				dist[i * COL + j] = ROW * COL + 1;
				pq_insert(pq_tunel, &node, i * COL + j, dist);
			}
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;

	while (!pq_isEmpty(pq_tunel, &node))
	{
		int u = pq_pop(pq_tunel, &node);
		for (i = 0; i < 8; i++)
		{
			int alt = 0;
			int v = u + rowMove[i] + colMove[i] * COL;
			if (0 > v || v > ROW * COL || dist[v] == -1)
				continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + getHardnessCost(dungeon.map[u / COL][u % COL].hardness);
				if (alt < dist[v])
				{
					dist[v] = alt;
					pq_insert(pq_tunel, &node, v, dist);
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
	Queue pq_nontunel;
	;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.map[i][j].space == ROOM ||
				dungeon.map[i][j].space == CORRIDOR)
			{
				dist[i * COL + j] = ROW * COL + 1;
				pq_insert(pq_nontunel, &node, i * COL + j, dist);
			}
			else if (dungeon.map[i][j].space == ROCK)
			{
				dist[i * COL + j] = -1;
			}
		}
	}
	dist[dungeon.PC.row * COL + dungeon.PC.col] = 0;

	while (!pq_isEmpty(pq_nontunel, &node))
	{
		int u = pq_pop(pq_nontunel, &node);
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
					pq_insert(pq_nontunel, &node, v, dist);
				}
			}
		}
	}
	//print_dijkstra_path(dist);
}

void move_character_turn()
{
	Queue pq;
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
			printDungeon();
		}
		else if ((*n).character.characteristics & NPC_TELEPATH)
			pq_insert_NPC(pq, &character, &n);
	}
}

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

void move_npc()
{
	// for (int i = 0; i < 13; i++)
	// {
	// 	int npc_index = 0;
	// 	Character *npc = &dungeon.monster[npc_index];
	// 	npc_next_pos_07(npc, npc_index);
	// 	printDungeon();
	// }

	while (!dungeon.PC.dead)
	{
		int i;
		for (i = 0; i < dungeon.num_mon; i++)
		{
			Character *npc = &dungeon.monster[i];
			npc_next_pos_05(npc, i);
		}
		printDungeon();
	}
}
void monsterList()
{
	WINDOW *list = newwin(ROW/4, COL/4, ROW/2, COL/2);

}

void printDungeon_ncurses_debug(WINDOW *game)
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
			if (isMonster(i, j))
			{
				wprintw(game, "%x", dungeon.monster[isMonster(i, j) - 1].characteristics);
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

void printDungeon_ncurses(WINDOW *game)
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (i == dungeon.PC.row && j == dungeon.PC.col)
			{
				mvwprintw(game, i, j, "@");
			}
			else if (!(isMonster(i, j) < 0))
			{
				mvwprintw(game, i, j, "%x", dungeon.monster[isMonster(i, j)].characteristics);
			}
			else
			{
				mvwprintw(game, i, j, "%c", dungeon.map[i][j].space);
			}
		}
		mvwprintw(game, i, COL, "\n");
	}
}

void move_pc()
{
	initscr();
	noecho();
	cbreak();
	WINDOW *game = newwin(ROW, COL, 0, 0);
	//game = newwin(ROW, COL, 0, 0);

	keypad(game, true);
	bool run = true;
	while(run)
	{
		printDungeon_ncurses(game);
		int key = wgetch(game);
		switch(key)
		{
			
			case KEY_UP:
				dungeon.PC.row--;
				printDungeon_ncurses(game);
				break;
			case KEY_DOWN:
				dungeon.PC.row++;
				printDungeon_ncurses(game);
				break;
			case KEY_LEFT:
				dungeon.PC.col--;
				printDungeon_ncurses(game);
				break;
			case KEY_RIGHT:
				dungeon.PC.col++;
				printDungeon_ncurses(game);
				break;
			case 'm':
				monsterList();
				break;
			case 'Q':
				run = false;
				break;
		}

	}


	clrtoeol();
	refresh();
	endwin();
}

void move_character()
{
	move_pc();
	//move_npc();
}

void move_dungeon()
{
	move_character();
	//move_character_turn();
}
