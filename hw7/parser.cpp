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

static uint32_t parse_dice(ifstream &f, string *lookahead, dice *d, string *d_s)
{
    int32_t base;
    uint32_t number, sides;

    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 0;
    }

    //f >> *lookahead;

    if (sscanf(lookahead->c_str(), "%d+%ud%u", &base, &number, &sides) != 3)
    {
        return 0;
    }

    d->set(base, number, sides);

    *d_s = *lookahead;
    //f >> *lookahead;

    return 1;
}

int parse_monster_name(ifstream &f, string *lookahead, string *name)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 0;
    }

    getline(f, *name);

    return 1;
    //f >> *lookahead;
}

int parse_monster_symb(ifstream &f, string *lookahead, char *symb)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 0;
    }

    *symb = f.get();

    eat_blankspace(f);
    if (f.peek() != '\n')
    {
        return 0;
    }

    //f >> *lookahead;
    return 1;
}

int parse_monster_color(ifstream &f, string *lookahead, vector<uint32_t> *color, string *color_s)
{
    eat_blankspace(f);
    getline(f, *lookahead);
    *color_s = *lookahead;
    //f >> *lookahead;
    return 1;
}

int parse_monster_desc(ifstream &f, string *lookahead, string *desc)
{
    eat_blankspace(f);
    while (f.peek() != '.')
    {
        *desc += *lookahead;
        getline(f, *lookahead);
        *desc += *lookahead + "\n";
    }
    //f >> *lookahead;
    return 1;
}

int parse_monster_speed(ifstream &f, string *lookahead, dice *d, string *d_s)
{
    return parse_dice(f, lookahead, d, d_s);
}

int parse_monster_dam(ifstream &f, string *lookahead, dice *d, string *d_s)
{
    return parse_dice(f, lookahead, d, d_s);
}

int parse_monster_hp(ifstream &f, string *lookahead, dice *d, string *d_s)
{
    return parse_dice(f, lookahead, d, d_s);
}

int parse_monster_abil(ifstream &f, string *lookahead, uint32_t *abil, string *abil_s)
{
    eat_blankspace(f);
    getline(f, *lookahead);
    *abil_s = *lookahead;
    //f >> *lookahead;
    return 1;
}

int parse_monster_rrty(ifstream &f, string *lookahead, int *rrty)
{
    getline(f, *lookahead);
    *rrty = atoi((*lookahead).c_str());
    //f >> *lookahead;
    return 1;
}

void parse_monster_description(ifstream &f, string *lookahead, Monster *m)
{
    string abil_s, color_s, speed_s, hp_s, dam_s; //TODO
    
    string name, desc;
    char symb;
    uint32_t abil;
    vector<uint32_t> color;
    dice speed, dam, hp;
    int rrty;
    //monster_description m;

    int name_d, desc_d, symb_d, abil_d, color_d, dam_d, hp_d, speed_d, rrty_d;
    // name_d = desc_d = symb_d = abil_d = color_d = dam_d = hp_d = speed_d = rrty_d = 0;
    //int flag = name_d & desc_d & symb_d & abil_d & color_d & dam_d & hp_d & speed_d & rrty_d;

    // for (int i = 0; flag; i++)
    // {
        if (!(*lookahead).compare("BEGIN"))
        {
            getline(f, *lookahead);
        }
        else if (!(*lookahead).compare("NAME"))
        {
            name_d = parse_monster_name(f, lookahead, &name);
        }
        else if (!(*lookahead).compare("SYMB"))
        {
            symb_d = parse_monster_symb(f, lookahead, &symb);
            m->symbol = symb;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            color_d = parse_monster_color(f, lookahead, &color, &color_s);
            m->color_string = color_s;
        }
        else if (!(*lookahead).compare("DESC"))
        {
            desc_d = parse_monster_desc(f, lookahead, &desc);
            m->description = desc;
        }
        else if (!(*lookahead).compare("."))
        {
            getline(f, *lookahead);
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            speed_d = parse_dice(f, lookahead, &speed, &speed_s);
            m->speed_string = speed_s;
        }
        else if (!(*lookahead).compare("DAM"))
        {
            dam_d = parse_dice(f, lookahead, &dam, &dam_s);
            m->damage_string = dam_s;
        }
        else if (!(*lookahead).compare("HP"))
        {
            hp_d = parse_dice(f, lookahead, &hp, &hp_s);
            m->hitpoints_string = hp_s;
        }
        else if (!(*lookahead).compare("ABIL"))
        {
            abil_d = parse_monster_abil(f, lookahead, &abil, &abil_s);
            m->abil_string = abil_s;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            rrty_d = parse_monster_rrty(f, lookahead, &rrty);
            m->rrty = rrty;
        }
        else if (!(*lookahead).compare("END"))
        {
            //getline(f, *lookahead);
        }
        if(name_d & desc_d & symb_d & abil_d & color_d & dam_d & hp_d & speed_d & rrty_d);
    //}
    
    // Monster *m = new Monster;
    // m->name = name;
    // m->description = desc;
    // m->symbol = symb;
    // m->abil_string = abil_s;
    // m->color_string = color_s;
    // m->damage_string = dam_s;
    // m->hitpoints_string = hp_s;
    // m->speed_string = speed_s;
    // m->rrty = rrty;

    // dungeon.mon.push_back(*m);
}

void load_monster(ifstream &f)
{
    Monster *m = (Monster *) malloc(sizeof(Monster));
    std::string s;
    do
    {
        f >> s;
        eat_whitespace(f);
        parse_monster_description(f, &s, m);
        //cout << s << endl; //TODO
        eat_blankspace(f);
        //getline(f, s);
        //cout << s << endl; //TODO
    } while (!s.compare("END"));
    dungeon.mon.push_back(*m);
    free(m);
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
    int i = 0;
    while (f.peek() != EOF)
    {
        cout << "monster index: " << i++ << endl;
        load_monster(f);
    }
}

void load_item_desc(char *path);

void print_monster_desc()
{
    for (unsigned int i = 0; i < dungeon.mon.size(); i++)
    {
        Monster m = dungeon.mon.at(i);
        cout << "" << m.name << endl;
        cout << "" << m.symbol << endl;
        cout << "" << m.color_string << endl;
        cout << "" << m.description << endl;
        cout << "" << m.speed_string << endl;
        cout << "" << m.damage_string << endl;
        cout << "" << m.hitpoints_string << endl;
        cout << "" << m.abil_string << endl;
        cout << "" << m.rrty << endl;
    }
    
}
