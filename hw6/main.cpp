#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "dungeon.h"
#include "queue.h"

Dungeon dungeon;

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	dungeon.seed = time(NULL);
	//TODO
    //dungeon.seed = 1540445615;
    //dungeon.seed = 1540445616;
	srand(dungeon.seed);

	bool load = false;
	bool save = false;

	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
		{
//			if (strcmp(argv[i], "--save") != 0 && strcmp(argv[i], "--load") != 0 &&
//					strcmp(argv[i], "--nummon") != 0)
//			{
//				fprintf(stderr, "Bad argument\n");
//				return -1;
//			}
			if (strcmp(argv[i], "--save") == 0)
			{
				save = true;
			}
			if (strcmp(argv[i], "--load") == 0)
			{
				load = true;
			}
			if (strcmp(argv[i], "--nummon") == 0)
			{
				dungeon.num_mon = atoi(argv[i+1]);
			}
		}
	}

	if (load)
	{
		FILE *f = fopen(path, "r");
		load_file(f);
	}
	else
	{
		generate_dungeon();
	}
	//print_dungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		save_file(f);
	}

/*
	int dist_nontunneling[ROW * COL];
	memset(dist_nontunneling, 0, sizeof (dist_nontunneling));
	int dist_tunneling[ROW * COL];
	memset(dist_tunneling, 0, sizeof (dist_tunneling));
	//dijkstra_nontunneling(dist_nontunneling);
	//dijkstra_tunneling(dist_tunneling);
*/

	move_dungeon();
    delete_dungeon();

	return 0;
}
