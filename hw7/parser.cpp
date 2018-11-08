#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/stat.h>

#include "parser.h"
#include "dice.h"
#include "limits.h"

#define NUM_MONSTER_FIELDS 9
#define NUM_ITEM_FIELDS 14

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

int parse_dice(ifstream &f, string *lookahead, dice *d)
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

    return 0;
}

int parse_nextline(ifstream &f, string *lookahead)
{
    getline(f, *lookahead);
    return 0;
}
int parse_name(ifstream &f, string *lookahead, string *name)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *name);

    f >> *lookahead;

    return 0;
}

int parse_monster_begin(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}

int parse_monster_end(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}

int parse_monster_name(ifstream &f, string *lookahead, string *name)
{
    return parse_name(f, lookahead, name);
}

int parse_monster_symb(ifstream &f, string *lookahead, char *symb)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    *symb = f.get();

    eat_blankspace(f);
    if (f.peek() != '\n')
    {
        return 1;
    }

    f >> *lookahead;

    return 0;
}

int parse_color(ifstream &f, string *lookahead, vector<uint32_t> *integer, string *integer_string)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *integer_string = *lookahead;

    f >> *lookahead;

    return 0;
}

int parse_monster_color(ifstream &f, string *lookahead, vector<uint32_t> *color, string *color_string)
{
    return parse_color(f, lookahead, color, color_string);
}

int parse_desc(ifstream &f, string *lookahead, string *desc)
{
    if (f.peek() != '\n')
    {
        return 1;
    }

    f.get();

    while (f.peek() != EOF)
    {
        getline(f, *lookahead);
        if (lookahead->length() > 77)
        {
            return 1;
        }

        lookahead->push_back('\n');

        if (*lookahead == ".\n")
        {
            break;
        }

        *desc += *lookahead;
    }

    desc->erase(desc->length() - 1);

    if (*lookahead != ".\n")
    {
        return 1;
    }

    f >> *lookahead;

    return 0;
}

int parse_monster_desc(ifstream &f, string *lookahead, string *desc)
{
    return parse_desc(f, lookahead, desc);
}

int parse_monster_desc_end(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}

int parse_monster_speed(ifstream &f, string *lookahead, dice *d)
{
    return parse_dice(f, lookahead, d);
}

int parse_monster_dam(ifstream &f, string *lookahead, dice *d)
{
    return parse_dice(f, lookahead, d);
}

int parse_monster_hp(ifstream &f, string *lookahead, dice *d)
{
    return parse_dice(f, lookahead, d);
}

int parse_monster_abil(ifstream &f, string *lookahead, uint32_t *abil, string *abil_string)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *abil_string = *lookahead;

    f >> *lookahead;

    return 0;
}

int parse_rarity(ifstream &f, string *lookahead, int *rarity)
{
    getline(f, *lookahead);
    *rarity = atoi((*lookahead).c_str());

    return 0;
}

int parse_monster_rarity(ifstream &f, string *lookahead, int *rarity)
{
    return parse_rarity(f, lookahead, rarity);
}

int parse_monster_description(ifstream &f, string *lookahead, Monster *m)
{
    string abil_s, color_s;

    string name, desc;
    char symb;
    uint32_t abil;
    vector<uint32_t> color;
    dice speed, dam, hp;
    int rarity;
    int count;
    for (count = 0; count < NUM_MONSTER_FIELDS; count++)
    {
        if (!(*lookahead).compare("BEGIN"))
        {
            if (parse_monster_begin(f, lookahead))
            {
                cout << "BEGIN------------------------------" << endl;
                return 0;
            }
        }
        else if (!(*lookahead).compare("NAME"))
        {
            if (parse_monster_name(f, lookahead, &name))
            {
                cout << "NAME------------------------------" << endl;
                return 0;
            }
            m->name = name;
        }
        else if (!(*lookahead).compare("SYMB"))
        {
            if (parse_monster_symb(f, lookahead, &symb))
            {
                cout << "SYMB------------------------------" << endl;
                return 0;
            }
            m->symbol = symb;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_monster_color(f, lookahead, &color, &color_s))
            {
                cout << "COLOR------------------------------" << endl;
                return 0;
            }
            m->color_string = color_s;
        }
        else if (!(*lookahead).compare("DESC"))
        {
            if (parse_monster_desc(f, lookahead, &desc))
            {
                cout << "DESC------------------------------" << endl;
                return 0;
            }
            m->description = desc;
        }
        else if (!(*lookahead).compare("."))
        {
            if (parse_monster_desc_end(f, lookahead))
            {
                cout << ".------------------------------" << endl;
                return 0;
            }
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            if (parse_dice(f, lookahead, &speed))
            {
                cout << "SPEED------------------------------" << endl;
                return 0;
            }

            m->speed_dice = speed;
        }
        else if (!(*lookahead).compare("DAM"))
        {
            if (parse_dice(f, lookahead, &dam))
            {
                cout << "DAM------------------------------" << endl;
                return 0;
            }
            m->damage = dam;
        }
        else if (!(*lookahead).compare("HP"))
        {
            if (parse_dice(f, lookahead, &hp))
            {
                cout << "HP------------------------------" << endl;
                return 0;
            }
            m->hitpoints = hp;
        }
        else if (!(*lookahead).compare("ABIL"))
        {
            if (parse_monster_abil(f, lookahead, &abil, &abil_s))
            {
                cout << "ABIL------------------------------" << endl;
                return 0;
            }
            m->abil_string = abil_s;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_monster_rarity(f, lookahead, &rarity))
            {
                cout << "RRTY------------------------------" << endl;
                return 0;
            }
            m->rrty = rarity;
        }
        // else if (*lookahead == "END")
        // {
        //     if (parse_monster_begin(f, lookahead))
        //     {
        //         cout << "END------------------------------" << endl;
        //         return 0;
        //     }
        // }
        // else
        // {
        //     return 0;
        // }
    }

    return 1;
}

void load_monster(ifstream &f)
{
    std::string s;
    Monster *m = (Monster *)malloc(sizeof(Monster));
    do
    {
        f >> s;
        eat_whitespace(f);
        // if (f.peek() == '\n')
        // {
        //     free(m);
        //     return;
        // }
        if (!parse_monster_description(f, &s, m))
        {
            cout << "discard------------------------------" << endl;
            free(m);
            return;
        }
        eat_blankspace(f);
    } //while (s != "\n");
    while (s.compare("END"));

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

    getline(f, s); //get file header line
    string str = s.substr(0, 26);
    if (str.compare("RLG327 MONSTER DESCRIPTION"))
    {
        fprintf(stderr, "Incorrect format of monster desc\n");
        return;
    }
    while (f.peek() != EOF)
    {
        load_monster(f);
    }
}




int parse_item_begin(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}
int parse_item_end(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}
int parse_item_name(ifstream &f, string *lookahead, string *name)
{
    return parse_name(f, lookahead, name);
}
int parse_item_color(ifstream &f, string *lookahead, vector<uint32_t> *color, string *color_string)
{
    return parse_color(f, lookahead, color, color_string);
}
int parse_item_weight(ifstream &f, string *lookahead, dice *weight)
{
    return parse_dice(f, lookahead, weight);
}
int parse_item_hit(ifstream &f, string *lookahead, dice *hit)
{
    return parse_dice(f, lookahead, hit);
}
int parse_item_damage(ifstream &f, string *lookahead, dice *damage)
{
    return parse_dice(f, lookahead, damage);
}
int parse_item_attribute(ifstream &f, string *lookahead, dice *attribute)
{
    return parse_dice(f, lookahead, attribute);
}
int parse_item_value(ifstream &f, string *lookahead, dice *value)
{
    return parse_dice(f, lookahead, value);
}
int parse_item_dodge(ifstream &f, string *lookahead, dice *dodge)
{
    return parse_dice(f, lookahead, dodge);
}
int parse_item_defence(ifstream &f, string *lookahead, dice *defence)
{
    return parse_dice(f, lookahead, defence);
}
int parse_item_speed(ifstream &f, string *lookahead, dice *speed)
{
    return parse_dice(f, lookahead, speed);
}
int parse_item_desc(ifstream &f, string *lookahead, string *desc)
{
    return parse_desc(f, lookahead, desc);
}
int parse_item_desc_end(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}
int parse_item_rarity(ifstream &f, string *lookahead, int *rarity)
{
    return parse_rarity(f, lookahead, rarity);
}
int parse_item_artifact(ifstream &f, string *lookahead, bool *artifact)
{
    getline(f, *lookahead);
    //*artifact = atoi((*lookahead).c_str()); //TODO

    return 0;
}

int parse_item_description(ifstream &f, string *lookahead, Item *item)
{
        string name;
        string description;
        vector<uint32_t> color;
        dice hit;
        dice damage;
        dice dodge;
        dice defence;
        dice weight;
        dice speed;
        dice attribute;
        dice value;
        bool artifact;
        int rarity;

        string type;
        string color_string;

        int count = 0;
    for (count = 0; count < NUM_ITEM_FIELDS; count++)
    {
        if (!(*lookahead).compare("BEGIN"))
        {
            if (parse_item_begin(f, lookahead))
            {
                cout << "BEGIN------------------------------" << endl;
                return 0;
            }
        }
        else if (!(*lookahead).compare("NAME"))
        {
            if (parse_item_name(f, lookahead, &name))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_item_color(f, lookahead, &color, &color_string))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("WEIGHT"))
        {
            if (parse_item_weight(f, lookahead, &weight))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("HIT"))
        {
            if (parse_item_hit(f, lookahead, &hit))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("DAM"))
        {
            if (parse_item_damage(f, lookahead, &damage))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("ATTR"))
        {
            if (parse_item_attribute(f, lookahead, &attribute))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("VAL"))
        {
            if (parse_item_value(f, lookahead, &value))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("DODGE"))
        {
            if (parse_item_dodge(f, lookahead, &dodge))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("DEF"))
        {
            if (parse_item_defence(f, lookahead, &defence))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            if (parse_item_speed(f, lookahead, &speed))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("DESC"))
        {
            if (parse_item_desc(f, lookahead, &description))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("."))
        {
            if (parse_item_desc_end(f, lookahead))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_item_rarity(f, lookahead, &rarity))
            {
                return 0;
            }
        }
        else if (!(*lookahead).compare("ART"))
        {
            if (parse_item_artifact(f, lookahead, &artifact))
            {
                return 0;
            }
        }
    }

    return 1;
}

void load_item(ifstream &f)
{
    std::string s;
    Item *item = (Item *)malloc(sizeof(Item));
    do
    {
        f >> s;
        eat_whitespace(f);
        // if (f.peek() == '\n')
        // {
        //     free(item);
        //     return;
        // }
        if (!parse_item_description(f, &s, item))
        {
            cout << "discard------------------------------" << endl;
            free(item);
            return;
        }
        eat_blankspace(f);
    } //while (s != "\n");
    while (s.compare("END"));

    dungeon.item.push_back(*item);
    free(item);
}

void load_item_desc(char *path)
{
    if (!fopen(path, "r"))
    {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    ifstream f(path);
    std::string s;

    getline(f, s); //get file header line
    string str = s.substr(0, 25);
    if (str.compare("RLG327 OBJECT DESCRIPTION"))
    {
        fprintf(stderr, "Incorrect format of item desc\n");
        return;
    }
    while (f.peek() != EOF)
    {
        load_item(f);
    }
}

void print_monster_desc()
{
    for (unsigned int i = 0; i < dungeon.mon.size(); i++)
    {
        Monster m = dungeon.mon.at(i);
        cout << m.name << endl;
        cout << m.description << endl;
        cout << m.color_string << endl;
        cout << m.speed_dice.print_string() << endl;
        cout << m.abil_string << endl;
        cout << m.hitpoints.print_string() << endl;
        cout << m.damage.print_string() << endl;
        cout << m.symbol << endl;
        cout << m.rrty << endl;
    }
}

void print_monster_desc_with_type()
{
    for (unsigned int i = 0; i < dungeon.mon.size(); i++)
    {
        Monster m = dungeon.mon.at(i);
        cout << "NAME: " << m.name << endl;
        cout << "DESCRIPTION: " << m.description << endl;
        cout << "COLOR: " << m.color_string << endl;
        cout << "SPEED: " << m.speed_dice.print_string() << endl;
        cout << "ABILITY: " << m.abil_string << endl;
        cout << "HITPOINTS: " << m.hitpoints.print_string() << endl;
        cout << "DAMAGE: " << m.damage.print_string() << endl;
        cout << "SYMBOL: " << m.symbol << endl;
        cout << "RRTY: " << m.rrty << endl;
        cout << endl;
    }
}

void print_item_desc();
void print_item_desc_with_type();