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
		fprintf(stderr, "Failed to open file\n");
		return;
	}
    else
    {
        printf("found path: %s\n", path);
    }

    ifstream f(path);
    std::string s;
    getline(f, s);
    cout << s << endl;

}
