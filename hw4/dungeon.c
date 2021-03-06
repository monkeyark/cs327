#include "queue.h"
#include "dungeon.h"
#include "npcmove.h"

void initDungeon()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			dungeon.map[i][j].terrain = ROCK;
			dungeon.map[i][j].hardness = ROCK_H;
		}
	}
}

int get_random(int modulus, int min)
{
	return rand() % modulus + min;
}

int isMonster(int row, int col)
{
	for (int i = 0; i < dungeon.num_mon; i++)
	{
		if (row == dungeon.monster[i].row && col == dungeon.monster[i].col)
		{
			return i+1;
		}
	}
	return 0;
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
			if (isMonster(i, j))
			{
				printf("%x", dungeon.monster[isMonster(i, j)-1].characteristics);
			}
			else
			{
				printf("%c", dungeon.map[i][j].terrain);
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
	return row > 0 && col > 0 && row < ROW-1 && col < COL-1;
}

bool is_valid_room(int row, int col, int width, int height)
{
	//is current terrain free
	if (dungeon.map[row][col].terrain != ROCK)
		return false;
	if (!is_inside(row, col) || !is_inside(row + height, col + width))
		return false;
	//touch or overlap another room
	for (int i = row - 1; i < row + height + 2; i++)
	{
		if (dungeon.map[i][col - 1].terrain != ROCK)
			return false; //left touch
		if (dungeon.map[i][col].terrain != ROCK)
			return false; //left overlap
		if (dungeon.map[i][col + width + 1].terrain != ROCK)
			return false; //right touch
		if (dungeon.map[i][col + width].terrain != ROCK)
			return false; //right overlap
	}
	for (int j = col - 1; j < col + width + 2; j++)
	{
		if (dungeon.map[row - 1][j].terrain != ROCK)
			return false; //top touch
		if (dungeon.map[row][j].terrain != ROCK)
			return false; //top overlap
		if (dungeon.map[row + height + 1][j].terrain != ROCK)
			return false; //bottom touch
		if (dungeon.map[row + height][j].terrain != ROCK)
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
				dungeon.map[i][j].terrain = ROOM;
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

int minimum(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

bool is_connected(int row, int col)
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
		dungeon.map[aRow][aCol].terrain = CORRIDOR;

	int max = distance(0, 0, ROW, COL);
	int min = max;
	int top = max;
	int down = max;
	int left = max;
	int right = max;

	if (is_inside(aRow - 1, aCol))
	{
		top = distance(aRow - 1, aCol, bRow, bCol);
		min = minimum(min, top);
	}
	if (is_inside(aRow + 1, aCol))
	{
		down = distance(aRow + 1, aCol, bRow, bCol);
		min = minimum(min, down);
	}
	if (is_inside(aRow, aCol - 1))
	{
		left = distance(aRow, aCol - 1, bRow, bCol);
		min = minimum(min, left);
	}
	if (is_inside(aRow, aCol + 1))
	{
		right = distance(aRow, aCol + 1, bRow, bCol);
		min = minimum(min, right);
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


typedef struct Node_t
{
	struct Node_t *next;
	int priority;
	Character character;
} Node_t;

Node_t *node_new_NPC(int priority, Character c)
{
	Node_t *temp = malloc(sizeof(Node_t));
	temp->priority = priority;
	temp->character = c;
	temp->next = NULL;

	return temp;
}

void pq_insert_NPC(Queue pq, Node_t** head, Node_t** new)
{
	Node_t *temp = *head;

	int head_priority = (*head)->priority;
	int new_priority = (*new)->priority;
	if (head_priority == new_priority)
	{
		head_priority = (*head)->character.birth;
		new_priority = (*new)->character.birth;
	}

	if (head_priority > new_priority)
	{
		(*new)->next = *head;
		*head = *new;
	}
	else
	{
		int c = temp->next->priority;
		int d = (*new)->priority;
		while (temp->next != NULL && (c < d))
		{
			temp = temp->next;
		}

		(*new)->next = temp->next;
		temp->next = (*new);
	}
}

Node_t *pq_pop_NPC(Queue pq, Node_t **head)
{
	int priority = (*head)->priority;
	Node_t *t = *head;
	(*head) = (*head)->next;
	free(t);

	int next_turn = priority + 1000 / (*head)->character.speed;
	Character h = (*head)->character;

	return node_new_NPC(next_turn, h);
}

void move_character()
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
			print_dungeon();
		}
		else if ((*n).character.characteristics & NPC_TELEPATH)
		pq_insert_NPC(pq, &character, &n);
	}
}

Character new_NPC(int birth)
{
	Character npc;
	int row = get_random(ROW, 0);
	int col = get_random(COL, 0);
	//add monster into map
	if (dungeon.map[row][col].terrain == ROOM)
	//if (dungeon.map[npc.row][npc.col].hardness == 0)
	{
		npc.row = row;
		npc.col = col;
		npc.birth = birth;
		//creating NPC with all four characteristics having 1/2 probability, clean unused bits
		npc.characteristics = rand() & 0xf;
		npc.speed = get_random(20, 5);
		npc.dead = false;
		if (npc.characteristics & NPC_TELEPATH)//monster is telepath
		{
			npc.pc_row = dungeon.PC.row;
			npc.pc_col = dungeon.PC.col;
		}

		//printf("npc.characteristics = %d in hex: %x\n", npc.characteristics, npc.characteristics);
		//dungeon.map[npc.row][npc.col].terrain = MONSTER;

		dungeon.map[npc.row][npc.col].hardness = 0;

	}
	else
	{
		return new_NPC(birth);
	}

	return npc;
}

void generate_dungeon()
{
	//initialize dungeon
	initDungeon();
	dungeon.rooms = malloc(dungeon.num_room * sizeof(Room));
	dungeon.monster = malloc(dungeon.num_mon * sizeof(Character));
	int i = 0;
	for (i = 0; i < dungeon.num_room; i++)
	{
		dungeon.rooms[i] = new_room_random();
	}

	for (i = 0; i < dungeon.num_room - 1; i++)
	{
		new_corridor(dungeon.rooms[i].row, dungeon.rooms[i].col, dungeon.rooms[i + 1].row, dungeon.rooms[i + 1].col);
	}

	for (int i = 0; i < dungeon.num_mon; i++)
	{
		dungeon.monster[i] = new_NPC(i);
	}

	//add initial player location
	dungeon.PC.birth = -1;
	dungeon.PC.speed = 10;
	dungeon.PC.dead = false;
	dungeon.PC.row = dungeon.rooms[0].row;
	dungeon.PC.col = dungeon.rooms[0].col;
	dungeon.map[dungeon.PC.row][dungeon.PC.col].terrain = '@';
	dungeon.map[dungeon.PC.row][dungeon.PC.col].hardness = 0;

}

void load_file(FILE *f)
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
				dungeon.map[row][col].terrain = CORRIDOR;
			}
			else
			{
				dungeon.map[row][col].terrain = ROCK;
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
	dungeon.map[dungeon.PC.row][dungeon.PC.col].terrain = PLAYER;
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
	if (hardness == 255) return 3;
	if (hardness == 0) return 1;
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
	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	int i, j;
	Queue pq_tunel;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (!is_inside(i, j))
			{
				dist[i * COL + j] = -1;
			}
			else if (dungeon.map[i][j].terrain != PLAYER)
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
			if (0 > v || v > ROW * COL || dist[v] == -1) continue;

			if (dist[v] >= 0)
			{
				alt = dist[u] + get_hardness_cost(dungeon.map[u / COL][u % COL].hardness);
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
	int rowMove[8] = {-1,  -1,  -1,   0,  +1,  +1,  +1,   0};
	int colMove[8] = {-1,   0,  +1,  +1,  +1,   0,  -1,  -1};
	int i, j;
	Queue pq_nontunel;;
	Node *node = node_new(dungeon.PC.row * COL + dungeon.PC.col);

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			if (dungeon.map[i][j].terrain == ROOM ||
					dungeon.map[i][j].terrain == CORRIDOR ||
					dungeon.map[i][j].terrain == MONSTER)
			{
				dist[i * COL + j] = ROW * COL + 1;
				pq_insert(pq_nontunel, &node, i * COL + j, dist);
			}
			else if (dungeon.map[i][j].terrain == ROCK)
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
			if (0 > v || v > ROW * COL) continue;

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

void move_npc()
{
//	for (int i = 0; i < 13; i++)
//	{
//		int npc_index = 1;
//		Character *npc = &dungeon.monster[npc_index];
//		npc_next_pos_07(npc, npc_index);
//		print_dungeon();
//	}

	while (!dungeon.PC.dead)
	{
		int i;
		for (i = 0; i < dungeon.num_mon; i++)
		{
			Character *npc = &dungeon.monster[i];
			npc_next_pos_05(npc, i);
		}
		print_dungeon();
	}
}

void dungeon_ncurses()
{

}
