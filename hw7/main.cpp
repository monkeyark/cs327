#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <string>

#include "dungeon.h"
#include "queue.h"
#include "io.h"
#include "parser.h"

using namespace std;

Dungeon dungeon;

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	const char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);
/*
	char *path_dungeon = strcat(path, "/dungeon");
    char *path_monster = strcat(path, "/monster_desc.txt");
    char *path_dungeon = " ";
    char *path_monster = " ";
    path_dungeon = strcat(path_dungeon, path);
    path_monster = strcat(path_monster, path);
*/

    char path_dungeon[sizeof(path)*5];
    char path_monster[sizeof(path)*5];
    strcpy(path_dungeon, path);
    strcpy(path_monster, path);
    strcat(path_dungeon, "/dungeon");
    //strcat(path_monster, "/monster_desc.txt");
    strcat(path_monster, "/monster_desc_copy.txt");
    //printf("path_dungeon: %s\n", path_dungeon);
    //printf("path_monster: %s\n", path_monster);

    dungeon.num_mon = 0;
	//set up random seed
	dungeon.seed = time(NULL);
    dungeon.seed = 1540452211;
    
	//TODO
    dungeon.seed = 1540445615;
    //dungeon.seed = 1540445616;
	srand(dungeon.seed);

	bool load = false;
	bool save = false;

	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "--save") != 0 && strcmp(argv[i], "--load") != 0 &&
                strcmp(argv[i], "--nummon") != 0 && !atoi(argv[i]))
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
			if (strcmp(argv[i], "--nummon") == 0)
			{
				dungeon.num_mon = atoi(argv[i+1]);
			}
		}
	}

	if (load)
	{
		FILE *file_dungeon = fopen(path_dungeon, "r");
		load_dungeon(file_dungeon);
	}
	else
	{
		generate_dungeon();
	}

	if (save)
	{
		FILE *file_dungeon = fopen(path_dungeon, "w");
		save_dungeon(file_dungeon);
	}
/*
	move_dungeon();
    //print_dungeon();
    delete_dungeon();
*/

    load_monster_desc(path_monster);

	return 0;
}
