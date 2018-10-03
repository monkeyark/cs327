#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

#include "dungeon.h"

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
	path = strcat(path, "/dungeon");

	//set up random seed
	int seed = time(NULL);
	//seed = 1538543175;
	seed = 1538556186;

	printf("\nseed = %d;\n\n", seed);
	srand(seed);

	//generate random number of rooms
	dungeon.num_room = getRandom(7, 5);
	//generate random number of monster
	//dungeon.num_mon = getRandom(5, 8);
	dungeon.num_mon = 8;

	bool load = false;
	bool save = false;
	//bool nummon = false;

	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
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
		generateDungeon();
	}
	printDungeon();

	if (save)
	{
		FILE *f = fopen(path, "w");
		saveFile(f);
	}
	dijkstra_nontunneling();
	dijkstra_tunneling();

	free(dungeon.rooms);
	free(dungeon.monster);

	return 0;
}
