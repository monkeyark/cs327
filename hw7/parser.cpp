#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/stat.h>

#include "parser.h"
#include "dice.h"
#include "limits.h"

using namespace std;

static inline void eat_whitespace(ifstream &f)
{
    while (isspace(f.peek()))
    {
        f.get();
    }
}

static inline void eat_blankspace(ifstream &f)
{
    while (isblank(f.peek()))
    {
        f.get();
    }
}

static uint32_t parse_dice(ifstream &f, string *lookahead, dice *d)
{
    int32_t base;
    uint32_t number, sides;

    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    f >> *lookahead;

    if (sscanf(lookahead->c_str(), "%d+%ud%u", &base, &number, &sides) != 3)
    {
        return 1;
    }

    d->set(base, number, sides);

    //f >> *lookahead;

    return 0;
}

void parse_monster_name(ifstream &f, string *lookahead, string *name)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return;
    }

    getline(f, *name);
    //f >> *lookahead;
}

void parse_monster_symb(ifstream &f, string *lookahead, char *symb)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return;
    }

    *symb = f.get();

    eat_blankspace(f);
    if (f.peek() != '\n')
    {
        return;
    }

    //f >> *lookahead;
}

void parse_monster_color(ifstream &f, string *lookahead, vector<uint32_t> *color, string *color_s)
{
    eat_blankspace(f);
    getline(f, *lookahead);
    *color_s = *lookahead;
}

void parse_monster_desc(ifstream &f, string *lookahead, string *desc)
{
    eat_blankspace(f);
    while (f.peek() != '.')
    {
        getline(f, *lookahead);
        *desc += *lookahead;
    }
    //f >> *lookahead;
}

void parse_monster_speed(ifstream &f, string *lookahead, dice *d)
{
    parse_dice(f, lookahead, d);
}

void parse_monster_dam(ifstream &f, string *lookahead, dice *d)
{
    parse_dice(f, lookahead, d);
}

void parse_monster_hp(ifstream &f, string *lookahead, dice *d)
{
    parse_dice(f, lookahead, d);
}

void parse_monster_abil(ifstream &f, string *lookahead, uint32_t *abil, string *abil_s)
{
    eat_blankspace(f);
    getline(f, *lookahead);
    *abil_s = *lookahead;
    //f >> *lookahead;
}

void parse_monster_rrty(ifstream &f, string *lookahead, int *rrty)
{
    getline(f, *lookahead);
    *rrty = atoi((*lookahead).c_str());
    //f >> *lookahead;
}

void parse_monster_description(ifstream &f, string *lookahead)
{
    string abil_s, color_s; //TODO
    
    string name, desc;
    char symb;
    uint32_t abil;
    vector<uint32_t> color;
    dice speed, dam, hp;
    int rrty;
    monster_description m;

    if ((*lookahead).compare("BEGIN"))
    {
        getline(f, *lookahead);
    }
    else if ((*lookahead).compare("NAME"))
    {
        parse_monster_name(f, lookahead, &name);
    }
    else if ((*lookahead).compare("SYMB"))
    {
        parse_monster_symb(f, lookahead, &symb);
    }
    else if ((*lookahead).compare("COLOR"))
    {
        parse_monster_color(f, lookahead, &color, &color_s);
    }
    else if ((*lookahead).compare("DESC"))
    {
        getline(f, *lookahead);
        parse_monster_desc(f, lookahead, &desc);
    }
    else if ((*lookahead).compare("."))
    {
        getline(f, *lookahead);
    }
    else if ((*lookahead).compare("SPEED"))
    {
        parse_dice(f, lookahead, &speed);
    }
    else if ((*lookahead).compare("DAM"))
    {
        parse_dice(f, lookahead, &dam);
    }
    else if ((*lookahead).compare("HP"))
    {
        parse_dice(f, lookahead, &hp);
    }
    else if ((*lookahead).compare("ABIL"))
    {
        parse_monster_abil(f, lookahead, &abil, &abil_s);
    }
    else if ((*lookahead).compare("RRTY"))
    {
        parse_monster_rrty(f, lookahead, &rrty);
    }
    else if ((*lookahead).compare("END"))
    {
        getline(f, *lookahead);
    }
}

void load_monster(ifstream &f)
{
    std::string s;
    do
    {
        f >> s;
        eat_whitespace(f);
        parse_monster_description(f, &s);
        //cout << s << endl; //TODO
        eat_blankspace(f);
        //getline(f, s);
        //cout << s << endl; //TODO
    } while (s.compare("END"));
}

void load_monster_desc(char *path)
{
    if (!fopen(path, "r"))
    {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    ifstream f(path);
    std::string s;

    getline(f, s); //consume file header line

    while (f.peek() != EOF)
    {
        load_monster(f);
    }
}

void load_item_desc(char *path);

void print_monster_desc()
{

}
