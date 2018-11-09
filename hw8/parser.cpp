#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <string.h>
#include <sys/stat.h>

#include "parser.h"
#include "dice.h"
#include "limits.h"

#define NUM_MONSTER_FIELDS 11
#define NUM_ITEM_FIELDS 16

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
    f >> *lookahead;
    return 0;
}

int parse_nextline(ifstream &f, string *lookahead)
{
    eat_whitespace(f);
    getline(f, *lookahead);
    eat_whitespace(f);
    eat_blankspace(f);
    eat_whitespace(f);
    f >> *lookahead;
    return 0;
}
int parse_name(ifstream &f, string *lookahead, string *name)
{
    eat_whitespace(f);
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

int parse_color(ifstream &f, string *lookahead, vector<int> *color, string *color_string)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *color_string = *lookahead;

    f >> *lookahead;

    return 0;
}

int parse_monster_color(ifstream &f, string *lookahead, vector<int> *color, string *color_string)
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

int parse_monster_abil(ifstream &f, string *lookahead, int *abil, string *abil_string)
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
    f >> *lookahead;
    return 0;
}

int parse_monster_rarity(ifstream &f, string *lookahead, int *rarity)
{
    return parse_rarity(f, lookahead, rarity);
}

int parse_monster_description(ifstream &f, string *lookahead, Monster *m)
{
    //TODO
    //this parser can only read description with no repeative filed
    //the later repeative filed will overried the previous one
    string abil_s, color_s;

    string name, desc;
    char symb;
    int abil;
    vector<int> *color = nullptr;
    dice speed, dam, hp;
    int rarity;
    int count;
    for (f >> *lookahead, count = 0; count < NUM_MONSTER_FIELDS; count++)
    {
        if (!(*lookahead).compare("BEGIN"))
        {
            if (parse_monster_begin(f, lookahead))
            {
                cout << "BEGIN reading fail" << endl;
                return 0;
            }
            continue;
        }
        else if (!(*lookahead).compare("NAME"))
        {
            if (parse_monster_name(f, lookahead, &name))
            {
                cout << "NAME reading fail" << endl;
                return 0;
            }
            m->name = name;
            continue;
        }
        else if (!(*lookahead).compare("SYMB"))
        {
            if (parse_monster_symb(f, lookahead, &symb))
            {
                cout << "SYMB reading fail" << endl;
                return 0;
            }
            m->symbol = symb;
            continue;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_monster_color(f, lookahead, color, &color_s))
            {
                cout << "COLOR reading fail" << endl;
                return 0;
            }
            m->color_string = color_s;
            continue;
        }
        else if (!(*lookahead).compare("DESC"))
        {
            if (parse_monster_desc(f, lookahead, &desc))
            {
                cout << "DESC reading fail" << endl;
                return 0;
            }
            m->description = desc;
            continue;
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            if (parse_dice(f, lookahead, &speed))
            {
                cout << "SPEED reading fail" << endl;
                return 0;
            }
            m->speed_dice = speed;
            continue;
        }
        else if (!(*lookahead).compare("DAM"))
        {
            if (parse_dice(f, lookahead, &dam))
            {
                cout << "DAM reading fail" << endl;
                return 0;
            }
            m->damage = dam;
            continue;
        }
        else if (!(*lookahead).compare("HP"))
        {
            if (parse_dice(f, lookahead, &hp))
            {
                cout << "HP reading fail" << endl;
                return 0;
            }
            m->hitpoints = hp;
            continue;
        }
        else if (!(*lookahead).compare("ABIL"))
        {
            if (parse_monster_abil(f, lookahead, &abil, &abil_s))
            {
                cout << "ABIL reading fail" << endl;
                return 0;
            }
            m->abil_string = abil_s;
            continue;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_monster_rarity(f, lookahead, &rarity))
            {
                cout << "RRTY reading fail" << endl;
                return 0;
            }
            m->rrty = rarity;

            continue;
        }
        else if (*lookahead == "END")
        {
            getline(f, *lookahead);
            return 1;
        }
        else
        {
            cout << *lookahead << " not in filed" << endl;
            return 0;
        }
    }
    return 1;
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

    Monster *m = new Monster;
    while (f.peek() != EOF)
    {
        if (!parse_monster_description(f, &s, m))
        {
            cout << "discard monster" << endl;
            return;
        }
        //TODO
        dungeon.mon.push_back(*m);
        //(dungeon.mon)->push_back(*m);
        //(*dungeon.mon).push_back(*m);
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
int parse_item_type(ifstream &f, string *lookahead, string *type)
{
    return parse_name(f, lookahead, type);
}
int parse_item_color(ifstream &f, string *lookahead, vector<int> *color, string *color_string)
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
int parse_item_rarity(ifstream &f, string *lookahead, int *rarity)
{
    return parse_rarity(f, lookahead, rarity);
}
int parse_item_artifact(ifstream &f, string *lookahead, bool *artifact)
{
    eat_whitespace(f);
    getline(f, *lookahead);
    if (!(*lookahead).compare("TRUE"))
    {
        *artifact = true;
    }
    else if (!(*lookahead).compare("FALSE"))
    {
        *artifact = false;
    }
    else
    {
        return 1;
    }
    f >> *lookahead;
    return 0;
}

int parse_item_description(ifstream &f, string *lookahead, Item *item)
{
    //TODO
    //this parser can only read description with no repeative filed
    //the later repeative filed will overried the previous one
    string name;
    string description;
    vector<int> *color = nullptr;
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

    int count;
    for (f >> *lookahead, count = 0; count < NUM_ITEM_FIELDS; count++)
    {
        if (!(*lookahead).compare("BEGIN"))
        {
            if (parse_item_begin(f, lookahead))
            {
                return 0;
            }
            continue;
        }
        else if (!(*lookahead).compare("NAME"))
        {
            if (parse_item_name(f, lookahead, &name))
            {
                return 0;
            }
            item->name = name;
            continue;
        }
        else if (!(*lookahead).compare("TYPE"))
        {
            if (parse_item_type(f, lookahead, &type))
            {
                return 0;
            }
            item->type = type;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_item_color(f, lookahead, color, &color_string))
            {
                return 0;
            }
            item->color = *color;
            item->color_string = color_string;
            continue;
        }
        else if (!(*lookahead).compare("WEIGHT"))
        {
            if (parse_item_weight(f, lookahead, &weight))
            {
                return 0;
            }
            item->weight = weight;
            continue;
        }
        else if (!(*lookahead).compare("HIT"))
        {
            if (parse_item_hit(f, lookahead, &hit))
            {
                return 0;
            }
            item->hit = hit;
            continue;
        }
        else if (!(*lookahead).compare("DAM"))
        {
            if (parse_item_damage(f, lookahead, &damage))
            {
                return 0;
            }
            item->damage = damage;
            continue;
        }
        else if (!(*lookahead).compare("ATTR"))
        {
            if (parse_item_attribute(f, lookahead, &attribute))
            {
                return 0;
            }
            item->attribute = attribute;
            continue;
        }
        else if (!(*lookahead).compare("VAL"))
        {
            if (parse_item_value(f, lookahead, &value))
            {
                return 0;
            }
            item->value = value;
            continue;
        }
        else if (!(*lookahead).compare("DODGE"))
        {
            if (parse_item_dodge(f, lookahead, &dodge))
            {
                return 0;
            }
            item->dodge = dodge;
            continue;
        }
        else if (!(*lookahead).compare("DEF"))
        {
            if (parse_item_defence(f, lookahead, &defence))
            {
                return 0;
            }
            item->defence = defence;
            continue;
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            if (parse_item_speed(f, lookahead, &speed))
            {
                return 0;
            }
            item->speed = speed;
            continue;
        }
        else if (!(*lookahead).compare("DESC"))
        {
            if (parse_item_desc(f, lookahead, &description))
            {
                return 0;
            }
            item->description = description;
            continue;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_item_rarity(f, lookahead, &rarity))
            {
                return 0;
            }
            item->rarity = rarity;
            continue;
        }
        else if (!(*lookahead).compare("ART"))
        {
            if (parse_item_artifact(f, lookahead, &artifact))
            {
                return 0;
            }
            item->artifact = artifact;
            continue;
        }
        else if (*lookahead == "END")
        {
            getline(f, *lookahead);
            return 1;
        }
        else
        {
            cout << *lookahead << " is not in filed" << endl;
            return 0;
        }
    }

    return 1;
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
    Item *item = new Item;
    while (f.peek() != EOF)
    {
        if (!parse_item_description(f, &s, item))
        {
            cout << "discard item" << endl;
            return;
        }
        //TODO
        dungeon.it.push_back(*item);
        //(dungeon.it)->push_back(*item);
        //(*(dungeon.it)).push_back(*item);
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
void print_item_desc_with_type()
{
    cout << dungeon.it.size() << endl;
    for (unsigned int i = 0; i < dungeon.it.size(); i++)
    {
        Item item = dungeon.it.at(i);
        cout << "NAME: " << item.name << endl;
        cout << "DESCRIPTION: " << item.description << endl;
        cout << "TYPE: " << item.type << endl;
        cout << "COLOR: " << item.color_string << endl;
        cout << "WEIGHT: " << item.weight.print_string() << endl;
        cout << "HITPOINTS: " << item.hit.print_string() << endl;
        cout << "DAMAGE: " << item.damage.print_string() << endl;
        cout << "ATTRIBUTE: " << item.attribute.print_string() << endl;
        cout << "VALUE: " << item.value.print_string() << endl;
        cout << "DODGE: " << item.dodge.print_string() << endl;
        cout << "DEF: " << item.defence.print_string() << endl;
        cout << "SPEED: " << item.speed.print_string() << endl;
        cout << "ARTIFACT: " << uppercase << boolalpha << item.artifact << endl;
        cout << "RRTY: " << item.rarity << endl;
        cout << endl;
    }
}
