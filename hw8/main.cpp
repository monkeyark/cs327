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


Dungeon dungeon;

int main(int argc, char *argv[])
{
	char *home = getenv("HOME");
	const char *path = strcat(home, "/.rlg327");
	mkdir(path, 0777);

    char path_dungeon[sizeof(path)*5];
    char path_monster[sizeof(path)*5];
	char path_object[sizeof(path)*5];
    strcpy(path_dungeon, path);
    strcpy(path_monster, path);
	strcpy(path_object, path);
    strcat(path_dungeon, "/dungeon");
    strcat(path_monster, "/monster_desc.txt");
	strcat(path_object, "/object_desc.txt");


    dungeon.num_mon = 0;
	//set up random seed
	dungeon.seed = time(NULL);
    //dungeon.seed = 1540445615;
	srand(dungeon.seed);

	bool load = false;
	bool save = false;

	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (strcmp(argv[i], "--save") != 0 &&
				strcmp(argv[i], "--load") != 0 &&
                strcmp(argv[i], "--nummon") != 0 &&
				!atoi(argv[i]))
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

/*
	load_monster_desc(path_monster);
	load_object_desc(path_object);
    print_monster_desc();
	print_object_desc();
*/

	if (load)
	{
		FILE *file_dungeon = fopen(path_dungeon, "r");
		load_dungeon(file_dungeon);
	}
	else
	{
		//generate_dungeon();
		generate_dungeon_desc();
	}
    
	if (save)
	{
		FILE *file_dungeon = fopen(path_dungeon, "w");
		save_dungeon(file_dungeon);
	}


	//move_dungeon();
    //print_dungeon();
    delete_dungeon();


	return 0;
}
