#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include <curses.h>

#include "parser.h"
#include "dice.h"
#include "limits.h"

#define NUM_MONSTER_FIELDS 11
#define NUM_OBJECT_FIELDS 16

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

int parse_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string, int *color_display)
{
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *color_string = *lookahead;

    char *c_color_string = new char[(*color_string).length() + 1];
    strcpy(c_color_string, (*color_string).c_str());

    *color = 0;

    char *token = strtok(c_color_string, " ");
//TODO >>>>>>>>>>>>>>>>>>>>>>
//use the first read color as display color
    if (strcmp(token, "RED") == 0)
    {
        *color_display = COLOR_RED;
    }
    else if (strcmp(token, "GREEN") == 0)
    {
        *color_display = COLOR_GREEN;
    }
    else if (strcmp(token, "YELLOW") == 0)
    {
        *color_display = COLOR_YELLOW;
    }
    else if (strcmp(token, "BLUE") == 0)
    {
        *color_display = COLOR_BLUE;
    }
    else if (strcmp(token, "MAGENTA") == 0)
    {
        *color_display = COLOR_MAGENTA;
    }
    else if (strcmp(token, "CYAN") == 0)
    {
        *color_display = COLOR_CYAN;
    }
    else if (strcmp(token, "WHITE") == 0)
    {
        *color_display = COLOR_WHITE;
    }
    else if (strcmp(token, "BLACK") == 0)
    {
        *color_display = COLOR_BLACK;
    }
    else
    {
        cout << "reading illegal color" << endl;
        delete [] c_color_string;
        return 1;
    }
//TODO <<<<<<<<<<<<<<<<<<<<<

    while (token != 0)
    {
        if (strcmp(token, "RED") == 0)
        {
            *color |= COLORS_RED;
        }
        else if (strcmp(token, "GREEN") == 0)
        {
            *color |= COLORS_GREEN;
        }
        else if (strcmp(token, "YELLOW") == 0)
        {
            *color |= COLORS_YELLOW;
        }
        else if (strcmp(token, "BLUE") == 0)
        {
            *color |= COLORS_BLUE;
        }
        else if (strcmp(token, "MAGENTA") == 0)
        {
            *color |= COLORS_MAGENTA;
        }
        else if (strcmp(token, "CYAN") == 0)
        {
            *color |= COLORS_CYAN;
        }
        else if (strcmp(token, "WHITE") == 0)
        {
            *color |= COLORS_WHITE;
        }
        else if (strcmp(token, "BLACK") == 0)
        {
            *color |= COLORS_BLACK;
        }
        else
        {
            cout << "reading illegal color" << endl;
            delete [] c_color_string;
            return 1;
        }
        token = strtok(NULL, " ");
    }

    f >> *lookahead;

    delete [] c_color_string;
    return 0;
}

int parse_monster_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string, int *color_display)
{
    return parse_color(f, lookahead, color, color_string, color_display);
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
    eat_blankspace(f);

    if (f.peek() == '\n')
    {
        return 1;
    }

    getline(f, *lookahead);
    *ability_string = *lookahead;


    char *c_ability_string = new char[(*ability_string).length() + 1];
    strcpy(c_ability_string, (*ability_string).c_str());

    *ability = 0;

    char *token = strtok(c_ability_string, " ");
    while (token != 0)
    {
        if (strcmp(token, "SMART") == 0)
        {
            *ability |= NPC_SMART;
        }
        else if (strcmp(token, "TELE") == 0)
        {
            *ability |= NPC_TELEPATH;
        }
        else if (strcmp(token, "TUNNEL") == 0)
        {
            *ability |= NPC_TUNNEL;
        }
        else if (strcmp(token, "ERRATIC") == 0)
        {
            *ability |= NPC_ERRATIC;
        }
        else if (strcmp(token, "PASS") == 0)
        {
            *ability |= NPC_PASS_WALL;
        }
        else if (strcmp(token, "DESTROY") == 0)
        {
            *ability |= NPC_DESTROY_OBJ;
        }
        else if (strcmp(token, "PICKUP") == 0)
        {
            *ability |= NPC_PICKUP_OBJ;
        }
        else if (strcmp(token, "UNIQ") == 0)
        {
            *ability |= NPC_UNIQ;
        }
        else if (strcmp(token, "BOSS") == 0)
        {
            *ability |= NPC_BOSS;
        }
        else
        {
            cout << "reading illegal ability" << endl;
            delete [] c_ability_string;
            return 1;
        }
        token = strtok(NULL, " ");
    }

    f >> *lookahead;

    delete [] c_ability_string;
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
    unsigned int color, ability; //TODO

    dice speed, dam, hp;
    int rarity;

    int count;
    int color_display;

    m->seen = false;
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
            if (parse_monster_color(f, lookahead, &color, &color_string, &color_display))
            {
                cout << "COLOR reading fail" << endl;
                return 0;
            }
            m->color = color;
            m->color_string = color_string;
            m->color_display = color_display;
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
            m->ability = ability;
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
    delete m;
}

int parse_object_begin(ifstream &f, string *lookahead)
{
    return parse_nextline(f, lookahead);
}

int parse_object_name(ifstream &f, string *lookahead, string *name)
{
    return parse_name(f, lookahead, name);
}

int parse_object_type(ifstream &f, string *lookahead, int *type, string *type_string, char *symbol)
{
    if (parse_name(f, lookahead, type_string))
    {
        return 1;
    }

    if (*type_string == "WEAPON")
    {
        *symbol = '|';
        *type = WEAPON;
    }
    else if (*type_string == "OFFHAND")
    {
        *symbol = ')';
        *type = OFFHAND;
    }
    else if (*type_string == "RANGED")
    {
        *symbol = '}';
        *type = RANGED;
    }
    else if (*type_string == "ARMOR")
    {
        *symbol = '[';
        *type = ARMOR;
    }
    else if (*type_string == "HELMET")
    {
        *symbol = ']';
        *type = HELMET;
    }
    else if (*type_string == "CLOAK")
    {
        *symbol = '(';
        *type = CLOAK;
    }
    else if (*type_string == "GLOVES")
    {
        *symbol = '{';
        *type = GLOVES;
    }
    else if (*type_string == "BOOTS")
    {
        *symbol = '\\';
        *type = BOOTS;
    }
    else if (*type_string == "AMULET")
    {
        *symbol = '"';
        *type = AMULET;
    }
    else if (*type_string == "LIGHT")
    {
        *symbol = '_';
        *type = LIGHT;
    }
    else if (*type_string == "RING")
    {
        *symbol = '=';
        *type = RING;
    }
    else if (*type_string == "SCROLL")
    {
        *symbol = '~';
    }
    else if (*type_string == "BOOK")
    {
        *symbol = '?';
    }
    else if (*type_string == "FLASK")
    {
        *symbol = '!';
    }
    else if (*type_string == "GOLD")
    {
        *symbol = '$';
    }
    else if (*type_string == "AMMUNITION")
    {
        *symbol = '/';
    }
    else if (*type_string == "FOOD")
    {
        *symbol = ',';
    }
    else if (*type_string == "WAND")
    {
        *symbol = '-';
    }
    else if (*type_string == "CONTAINER")
    {
        *symbol = '%';
    }
    else if (*type_string == "STACK")
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

int parse_object_color(ifstream &f, string *lookahead, unsigned int *color, string *color_string, int *color_display)
{
    return parse_color(f, lookahead, color, color_string, color_display);
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
    int type;
    //int vision_bonus;//TODO
    string type_string;

    unsigned int color;
    string color_string;
    int color_display;

    object->seen = false;
    int count;
    for (f >> *lookahead, count = 0; count < NUM_OBJECT_FIELDS; count++)
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
            if (parse_object_type(f, lookahead, &type, &type_string, &symbol))
            {
                return 0;
            }
            object->type = type;
            object->type_string = type_string;
            object->symbol = symbol;
        }
        else if (!(*lookahead).compare("COLOR"))
        {
            if (parse_object_color(f, lookahead, &color, &color_string, &color_display))
            {
                return 0;
            }
            object->color = color;
            object->color_string = color_string;
            object->color_display = color_display;
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
    delete object;
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
        cout << object.type_string << endl;
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
        cout << "TYPE: " << object.type_string << endl;
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
