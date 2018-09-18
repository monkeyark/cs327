#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define PC '@'
#define ROCK_H 255
#define ROOM_H 0
#define CORRIDOR_H 0
#define PC_H 0

typedef struct dungeon
{
	int num_room;
	int pc_row;
	int pc_col;
	int version;
} Dungeon;

typedef struct dungeonCell
{
	char space;
	int hardness;
} Cell;

typedef struct dungeonRoom
{
	int row;
	int col;
	int width;
	int height;
} Room;


Cell dungeon[ROW][COL];
Room *dungeonRoom;
Dungeon map;

void initDungeon()
{
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			dungeon[i][j].space = ROCK;
			dungeon[i][j].hardness = ROCK_H;
		}
	}
}

int getRandom(int modulus, int min)
{
	return rand() % modulus + min;
}


/*
void printDungeon()
{
	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i=0; i<ROW; i++)
	{
		printf("|");
		for (int j=0; j<COL; j++)
		{
			printf("%c", dungeon[i][j].space);
		}
		printf("|\n");
	}

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");
}
*/



void printDungeon()
{
	printf("   ");
	for (int i=0; i<COL; i++)
	{
		printf("%d", i % 10);
	}
	printf("\n");

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i=0; i<ROW; i++)
	{
		printf("%2d|", i);
		for (int j=0; j<COL; j++)
		{
			printf("%c", dungeon[i][j].space);
			//int x = dungeon[i][j].hardness / 85 + 1;
			//printf("%d", x);
		}
		printf("|\n");
	}

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");
}


bool isInside(int row, int col)
{
	//is room not on edge or outside of dungeon
	return row > 0 && col > 0 && row < ROW && col < COL;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col].space != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//is touching another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1].space != ROCK) return false; //left
		if (dungeon[i][col+width+1].space != ROCK) return false;//right
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j].space != ROCK) return false;//top
		if (dungeon[row+height+1][j].space != ROCK) return false;//bottom
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
		for (int i=r.row; i<r.row+r.height; i++)
		{
			for (int j=r.col; j<r.col+r.width; j++)
			{
				dungeon[i][j].space = ROOM;
				dungeon[i][j].hardness = ROOM_H;
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
	for (int i=row; i<row+height; i++)
	{
		for (int j=col; j<col+width; j++)
		{
			dungeon[i][j].space = ROOM;
			dungeon[i][j].hardness = ROOM_H;
		}
	}
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (aRow <= bRow) 	//now at dungeon[aRow][aCol]
	{
		for (int i=aRow; i<=bRow; i++)
		{
			if (dungeon[i][aCol].space == ROCK)
			{
				dungeon[i][aCol].space = CORRIDOR;
				dungeon[i][aCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[bRow][aCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}
	else
	{
		for (int i=bRow; i<=aRow; i++)
		{
			if (dungeon[i][bCol].space == ROCK)
			{
				dungeon[i][bCol].space = CORRIDOR;
				dungeon[i][bCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[aRow][bCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}

}

void generateDungeon(int n)
{
	//initial dungeon
	initDungeon();
	dungeonRoom = malloc(map.num_room * sizeof(Room));

	for (int i=0; i<n; i++)
	{
		dungeonRoom[i] = newRoom();
		printf("  ROOM%2d    row=%2d   col=%2d   width=%2d   height=%2d\n", i, dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i].width, dungeonRoom[i].height);//TODO
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i+1].row, dungeonRoom[i+1].col);
	}
	
	//add initial player loaction
	map.pc_row = dungeonRoom[0].row;
	map.pc_col = dungeonRoom[0].col;
	dungeon[map.pc_row][map.pc_col].space = '@';
	dungeon[map.pc_row][map.pc_col].hardness = 0;
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
	
	u_int32_t ver;
	fread(&ver, 4, 1, f);
	map.version = be32toh(ver);

	u_int32_t file_size;
	fread(&file_size, 4, 1, f);
	int filesize = be32toh(file_size);

	u_int8_t pc_col;
	fread(&pc_col, 1, 1, f);
	map.pc_col = pc_col;
	u_int8_t pc_row;
	fread(&pc_row, 1, 1, f);
	map.pc_row = pc_row;


	u_int8_t hard[1680];
	fread(hard, 1, 1680, f);

	for (int row=0; row<ROW; row++)
	{
		for (int col=0; col<COL; col++)
		{
			int index = COL*row + col;
			int h = hard[index];
			dungeon[row][col].hardness = h;
			if (h == 0)
			{
				dungeon[row][col].space = CORRIDOR;
			}
			else
			{
				dungeon[row][col].space = ROCK;
			}
		}
	}

	map.num_room = (filesize - 1702) / 4;
	dungeonRoom = malloc(map.num_room * sizeof(Room));

	u_int8_t roomRead[filesize - 1702];
	fread(roomRead, 1, filesize - 1702, f);

	int n = 0;
	for (int i=0; i<map.num_room; i++)
	{
		dungeonRoom[i].col = roomRead[n++];
		dungeonRoom[i].row = roomRead[n++];
		dungeonRoom[i].width = roomRead[n++];
		dungeonRoom[i].height = roomRead[n++];

		addRoom(dungeonRoom[i].row, dungeonRoom[i].col, dungeonRoom[i].width, dungeonRoom[i].height);
	}
	
	//add PC
	dungeon[map.pc_row][map.pc_col].space = PC;
	dungeon[map.pc_row][map.pc_col].hardness = PC_H;

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

	int ver = htobe32(map.version);
	fwrite(&ver, 4, 1, f);

	int filesize = 1702 + 4 * map.num_room;
	filesize = htobe32(filesize);
	fwrite(&filesize, 4, 1, f);

	int pc_x = map.pc_col;
	fwrite(&pc_x, 1, 1, f);
	int pc_y = map.pc_row;
	fwrite(&pc_y, 1, 1, f);

	char *hard = malloc(1680);
	//write hardness
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			hard[COL*i + j] = (char) dungeon[i][j].hardness;
		}
	}
	fwrite(hard, 1, 1680, f);

	//write room
	unsigned char *loc = malloc(4 *map.num_room);
	int n = 0;
	for (int i=0; i<map.num_room; i++)
	{
		loc[n++] = (unsigned char) dungeonRoom[i].col;
		loc[n++] = (unsigned char) dungeonRoom[i].row;
		loc[n++] = (unsigned char) dungeonRoom[i].width;
		loc[n++] = (unsigned char) dungeonRoom[i].height;
	}
	fwrite(loc, 1, 4*map.num_room, f);
	
	free(hard);
	free(loc);
	fclose(f);
}

static void dijkstra(int pcRow, int pcCol)
{
	static corridor_path_t path[DUNGEON_Y][DUNGEON_X], *p;
	static uint32_t initialized = 0;
	heap_t h;
	uint32_t x, y;

	if (!initialized)
	{
		for (y = 0; y < DUNGEON_Y; y++)
		{
			for (x = 0; x < DUNGEON_X; x++)
			{
				path[y][x].pos[dim_y] = y;
				path[y][x].pos[dim_x] = x;
			}
		}
		initialized = 1;
	}

	for (y = 0; y < DUNGEON_Y; y++)
	{
		for (x = 0; x < DUNGEON_X; x++)
		{
			path[y][x].cost = INT_MAX;
		}
	}

	path[from[dim_y]][from[dim_x]].cost = 0;

	heap_init(&h, corridor_path_cmp, NULL);

	for (y = 0; y < DUNGEON_Y; y++)
	{
		for (x = 0; x < DUNGEON_X; x++)
		{
			if (mapxy(x, y) != ter_wall_immutable)
			{
				path[y][x].hn = heap_insert(&h, &path[y][x]);
			} else
			{
				path[y][x].hn = NULL;
			}
		}
	}

	while ((p = heap_remove_min(&h)))
	{
		p->hn = NULL;

		if ((p->pos[dim_y] == to[dim_y]) && p->pos[dim_x] == to[dim_x])
		{
			for (x = to[dim_x], y = to[dim_y];
					(x != from[dim_x]) || (y != from[dim_y]);
					p = &path[y][x], x = p->from[dim_x], y = p->from[dim_y])
			{
				if (mapxy(x, y) != ter_floor_room)
				{
					mapxy(x, y) = ter_floor_hall;
					hardnessxy(x, y) = 0;
				}
			}
			heap_delete(&h);
			return;
		}

		if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn)
				&& (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost
						> p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_y] - 1][p->pos[dim_x]].cost = p->cost
					+ hardnesspair(p->pos);
			path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
			path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
			heap_decrease_key_no_replace(&h,
					path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
		}
		if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn)
				&& (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost
						> p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_y]][p->pos[dim_x] - 1].cost = p->cost
					+ hardnesspair(p->pos);
			path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
			path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
			heap_decrease_key_no_replace(&h,
					path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
		}
		if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn)
				&& (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost
						> p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_y]][p->pos[dim_x] + 1].cost = p->cost
					+ hardnesspair(p->pos);
			path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
			path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
			heap_decrease_key_no_replace(&h,
					path[p->pos[dim_y]][p->pos[dim_x] + 1].hn);
		}
		if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn)
				&& (path[p->pos[dim_y] + 1][p->pos[dim_x]].cost
						> p->cost + hardnesspair(p->pos)))
		{
			path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost
					+ hardnesspair(p->pos);
			path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
			path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
			heap_decrease_key_no_replace(&h,
					path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
		}
	}
}

/*
# define INF 0x3f3f3f3f
// iPair ==> Integer Pair
typedef pair<int, int> iPair;

// To add an edge
void addEdge(vector <pair<int, int> > adj[], int u, int v, int wt)
{
	adj[u].push_back(make_pair(v, wt));
	adj[v].push_back(make_pair(u, wt));
}

// Prints shortest paths from src to all other vertices
void shortestPath(vector<pair<int,int> > adj[], int V, int src)
{
	// Create a priority queue to store vertices that
	// are being preprocessed. This is weird syntax in C++.
	// Refer below link for details of this syntax
	// http://geeksquiz.com/implement-min-heap-using-stl/
	priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

	// Create a vector for distances and initialize all
	// distances as infinite (INF)
	vector<int> dist(V, INF);

	// Insert source itself in priority queue and initialize
	// its distance as 0.
	pq.push(make_pair(0, src));
	dist[src] = 0;

	//Looping till priority queue becomes empty (or all
	//   distances are not finalized)
	while (!pq.empty())
	{
		// The first vertex in pair is the minimum distance
		// vertex, extract it from priority queue.
		// vertex label is stored in second of pair (it
		// has to be done this way to keep the vertices
		// sorted distance (distance must be first item
		// in pair)
		int u = pq.top().second;
		pq.pop();

		// Get all adjacent of u. 
		for (auto x : adj[u])
		{
			// Get vertex label and weight of current adjacent
			// of u.
			int v = x.first;
			int weight = x.second;

			// If there is shorted path to v through u.
			if (dist[v] > dist[u] + weight)
			{
				// Updating distance of v
				dist[v] = dist[u] + weight;
				pq.push(make_pair(dist[v], v));
			}
		}
	}
}
*/


int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	//seed = 1536656798;
	//seed = 1536656664;
	//seed = 1536657024;
	//seed = 1536657138;
	//seed = 1536807801;
	//////////////////////////Linux above, Windows below
	//seed = 1536941561;

	printf("\nseed = %d;\n", seed);//TODD
	srand(seed);

	//generate random number of rooms
	map.num_room = getRandom(7, 5);
	bool load = false;
	bool save = false;

	if (argc != 1)
	{
		for (int i=1; i<argc; i++)
		{
			if (strcmp(argv[i], "--save") != 0 && strcmp(argv[i], "--load") != 0)
			{
				fprintf(stderr, "Bad argument\n");
				return -1;
			}
			if (strcmp(argv[i], "--save") == 0)
			{
				save = true;
			}
			if (strcmp(argv[i], "--load") == 0)
			{
				load = true;
			}
		}
	}

	if (load)
	{
		FILE *f = fopen(path, "r");
		loadFile(f);
	}
	else
	{
		generateDungeon(map.num_room);
	}
	printDungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		saveFile(f);
	}

	return 0;
}

