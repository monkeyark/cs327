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
#define NUM_object_FIELDS 16

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

int parse_monster_name(ifstream &f, string *lookahead, string *name)
{
    return parse_name(f, lookahead, name);
}

int parse_monster_symb(ifstream &f, string *lookahead, char *symbol)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    *symbol = f.get();

    eat_blankspace(f);
    if (f.peek() != '\n')
    {
        return 1;
    }

    f >> *lookahead;

    return 0;
}

int parse_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string)
{
    *color = get_random(6, 1);//TODO
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

int parse_monster_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string)
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

int parse_monster_ability(ifstream &f, string *lookahead, unsigned int *ability, string *ability_string)
{
    //*ability = //TODO
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *ability_string = *lookahead;

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
    //this parser can only read description with no repeative field
    //the later repeative field will overried the previous one
    string ability_string, color_string;

    string name, desc;
    char symbol;
    unsigned int color, ability;//TODO

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
            if (parse_monster_symb(f, lookahead, &symbol))
            {
                cout << "SYMB reading fail" << endl;
                return 0;
            }
            m->symbol = symbol;
            continue;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_monster_color(f, lookahead, &color, &color_string))
            {
                cout << "COLOR reading fail" << endl;
                return 0;
            }
            m->color = color; //TODO
            //m->color_int = color;//TODO
            m->color_string = color_string;
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
            m->speed = speed;
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
            if (parse_monster_ability(f, lookahead, &ability, &ability_string))
            {
                cout << "ABIL reading fail" << endl;
                return 0;
            }
            m->ability_string = ability_string;
            continue;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_monster_rarity(f, lookahead, &rarity))
            {
                cout << "RRTY reading fail" << endl;
                return 0;
            }
            m->rarity = rarity;

            continue;
        }
        else if (*lookahead == "END")
        {
            getline(f, *lookahead);
            return 1;
        }
        else
        {
            cout << *lookahead << " not in field" << endl;
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


int parse_object_begin(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}

int parse_object_name(ifstream &f, string *lookahead, string *name)
{
    return parse_name(f, lookahead, name);
}

int parse_object_type(ifstream &f, string *lookahead, string *type, char *symbol)
{
    if (parse_name(f, lookahead, type))
    {
        return 1;
    }
    if (*type == "WEAPON")
    {
        *symbol = '|';
    }
    else if(*type == "OFFHAND")
    {
        *symbol = ')';
    }
    else if(*type == "RANGED")
    {
        *symbol = '}';
    }
    else if(*type == "ARMOR")
    {
        *symbol = '[';
    }
    else if(*type == "HELMET")
    {
        *symbol = ']';
    }
    else if(*type == "CLOAK")
    {
        *symbol = '(';
    }
    else if(*type == "GLOVES")
    {
        *symbol = '{';
    }
    else if(*type == "BOOTS")
    {
        *symbol = '\\';
    }
    else if(*type == "RING")
    {
        *symbol = '=';
    }
    else if(*type == "AMULET")
    {
        *symbol = '"';
    }
    else if(*type == "LIGHT")
    {
        *symbol = '_';
    }
    else if(*type == "SCROLL")
    {
        *symbol = '~';
    }
    else if(*type == "BOOK")
    {
        *symbol = '?';
    }
    else if(*type == "FLASK")
    {
        *symbol = '!';
    }
    else if(*type == "GOLD")
    {
        *symbol = '$';
    }
    else if(*type == "AMMUNITION")
    {
        *symbol = '/';
    }
    else if(*type == "FOOD")
    {
        *symbol = ',';
    }
    else if(*type == "WAND")
    {
        *symbol = '-';
    }
    else if(*type == "CONTAINER")
    {
        *symbol = '%';
    }
    else if(*type == "STACK")
    {
        *symbol = '&';
    }
    else
    {
        cout << "Fail to parse type to symbol" << endl;
        return 1;
    }
 
    return 0;
}

int parse_object_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string)
{
    return parse_color(f, lookahead, color, color_string);
}

int parse_object_weight(ifstream &f, string *lookahead, dice *weight)
{
    return parse_dice(f, lookahead, weight);
}

int parse_object_hit(ifstream &f, string *lookahead, dice *hit)
{
    return parse_dice(f, lookahead, hit);
}

int parse_object_damage(ifstream &f, string *lookahead, dice *damage)
{
    return parse_dice(f, lookahead, damage);
}

int parse_object_attribute(ifstream &f, string *lookahead, dice *attribute)
{
    return parse_dice(f, lookahead, attribute);
}

int parse_object_value(ifstream &f, string *lookahead, dice *value)
{
    return parse_dice(f, lookahead, value);
}

int parse_object_dodge(ifstream &f, string *lookahead, dice *dodge)
{
    return parse_dice(f, lookahead, dodge);
}

int parse_object_defence(ifstream &f, string *lookahead, dice *defence)
{
    return parse_dice(f, lookahead, defence);
}

int parse_object_speed(ifstream &f, string *lookahead, dice *speed)
{
    return parse_dice(f, lookahead, speed);
}

int parse_object_desc(ifstream &f, string *lookahead, string *desc)
{
    return parse_desc(f, lookahead, desc);
}

int parse_object_rarity(ifstream &f, string *lookahead, int *rarity)
{
    return parse_rarity(f, lookahead, rarity);
}

int parse_object_artifact(ifstream &f, string *lookahead, bool *artifact)
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

int parse_object_description(ifstream &f, string *lookahead, Object *object)
{
    //TODO
    //this parser can only read description with no repeative field
    //the later repeative field will overried the previous one
    string name;
    string description;
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
    char symbol;
    string type;

    unsigned int color;
    string color_string;

    int count;
    for (f >> *lookahead, count = 0; count < NUM_object_FIELDS; count++)
    {
        if (!(*lookahead).compare("BEGIN"))
        {
            if (parse_object_begin(f, lookahead))
            {
                return 0;
            }
            continue;
        }
        else if (!(*lookahead).compare("NAME"))
        {
            if (parse_object_name(f, lookahead, &name))
            {
                return 0;
            }
            object->name = name;
            continue;
        }
        else if (!(*lookahead).compare("TYPE"))
        {
            if (parse_object_type(f, lookahead, &type, &symbol))
            {
                return 0;
            }
            object->type = type;
            object->symbol = symbol;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_object_color(f, lookahead, &color, &color_string))
            {
                return 0;
            }
            object->color = color; //TODO
            //object->color_int = color;
            object->color_string = color_string;
            continue;
        }
        else if (!(*lookahead).compare("WEIGHT"))
        {
            if (parse_object_weight(f, lookahead, &weight))
            {
                return 0;
            }
            object->weight = weight;
            continue;
        }
        else if (!(*lookahead).compare("HIT"))
        {
            if (parse_object_hit(f, lookahead, &hit))
            {
                return 0;
            }
            object->hit = hit;
            continue;
        }
        else if (!(*lookahead).compare("DAM"))
        {
            if (parse_object_damage(f, lookahead, &damage))
            {
                return 0;
            }
            object->damage = damage;
            continue;
        }
        else if (!(*lookahead).compare("ATTR"))
        {
            if (parse_object_attribute(f, lookahead, &attribute))
            {
                return 0;
            }
            object->attribute = attribute;
            continue;
        }
        else if (!(*lookahead).compare("VAL"))
        {
            if (parse_object_value(f, lookahead, &value))
            {
                return 0;
            }
            object->value = value;
            continue;
        }
        else if (!(*lookahead).compare("DODGE"))
        {
            if (parse_object_dodge(f, lookahead, &dodge))
            {
                return 0;
            }
            object->dodge = dodge;
            continue;
        }
        else if (!(*lookahead).compare("DEF"))
        {
            if (parse_object_defence(f, lookahead, &defence))
            {
                return 0;
            }
            object->defence = defence;
            continue;
        }
        else if (!(*lookahead).compare("SPEED"))
        {
            if (parse_object_speed(f, lookahead, &speed))
            {
                return 0;
            }
            object->speed = speed;
            continue;
        }
        else if (!(*lookahead).compare("DESC"))
        {
            if (parse_object_desc(f, lookahead, &description))
            {
                return 0;
            }
            object->description = description;
            continue;
        }
        else if (!(*lookahead).compare("RRTY"))
        {
            if (parse_object_rarity(f, lookahead, &rarity))
            {
                return 0;
            }
            object->rarity = rarity;
            continue;
        }
        else if (!(*lookahead).compare("ART"))
        {
            if (parse_object_artifact(f, lookahead, &artifact))
            {
                return 0;
            }
            object->artifact = artifact;
            continue;
        }
        else if (*lookahead == "END")
        {
            getline(f, *lookahead);
            return 1;
        }
        else
        {
            cout << *lookahead << " is not in field" << endl;
            return 0;
        }
    }

    return 1;
}

void load_object_desc(char *path)
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
        fprintf(stderr, "Incorrect format of object desc\n");
        return;
    }
    Object *object = new Object;
    while (f.peek() != EOF)
    {
        if (!parse_object_description(f, &s, object))
        {
            cout << "discard object" << endl;
            return;
        }
        //TODO
        dungeon.obj.push_back(*object);
        //(dungeon.it)->push_back(*object);
        //(*(dungeon.it)).push_back(*object);
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
        cout << m.speed.print_string() << endl;
        cout << m.ability_string << endl;
        cout << m.hitpoints.print_string() << endl;
        cout << m.damage.print_string() << endl;
        cout << m.symbol << endl;
        cout << m.rarity << endl;
        cout << endl;
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
        cout << "SPEED: " << m.speed.print_string() << endl;
        cout << "ABILITY: " << m.ability_string << endl;
        cout << "HITPOINTS: " << m.hitpoints.print_string() << endl;
        cout << "DAMAGE: " << m.damage.print_string() << endl;
        cout << "SYMBOL: " << m.symbol << endl;
        cout << "RRTY: " << m.rarity << endl;
        cout << endl;
    }
}

void print_object_desc()
{
    for (unsigned int i = 0; i < dungeon.obj.size(); i++)
    {
        Object object = dungeon.obj.at(i);
        cout << object.name << endl;
        cout << object.description << endl;
        cout << object.type << endl;
        cout << object.color_string << endl;
        cout << object.weight.print_string() << endl;
        cout << object.hit.print_string() << endl;
        cout << object.damage.print_string() << endl;
        cout << object.attribute.print_string() << endl;
        cout << object.value.print_string() << endl;
        cout << object.dodge.print_string() << endl;
        cout << object.defence.print_string() << endl;
        cout << object.speed.print_string() << endl;
        cout << uppercase << boolalpha << object.artifact << endl;
        cout << object.rarity << endl;
        cout << endl;
    }
}

void print_object_desc_with_type()
{
    for (unsigned int i = 0; i < dungeon.obj.size(); i++)
    {
        Object object = dungeon.obj.at(i);
        cout << "NAME: " << object.name << endl;
        cout << "DESCRIPTION: " << object.description << endl;
        cout << "TYPE: " << object.type << endl;
        cout << "COLOR: " << object.color_string << endl;
        cout << "WEIGHT: " << object.weight.print_string() << endl;
        cout << "HITPOINTS: " << object.hit.print_string() << endl;
        cout << "DAMAGE: " << object.damage.print_string() << endl;
        cout << "ATTRIBUTE: " << object.attribute.print_string() << endl;
        cout << "VALUE: " << object.value.print_string() << endl;
        cout << "DODGE: " << object.dodge.print_string() << endl;
        cout << "DEF: " << object.defence.print_string() << endl;
        cout << "SPEED: " << object.speed.print_string() << endl;
        cout << "ARTIFACT: " << uppercase << boolalpha << object.artifact << endl;
        cout << "RRTY: " << object.rarity << endl;
        cout << endl;
    }
}
