#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/stat.h>

using namespace std;

void load_monster(char *path)
{
    if (!fopen(path, "r"))
	{
        //printf("%s\n", path);
		fprintf(stderr, "Failed to open file\n");
		return;
	}

    ifstream file(path);

    cout << (char) file.get() << endl;

}
