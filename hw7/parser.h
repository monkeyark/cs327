#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdio.h>

#include "dungeon.h"
#include "dice.h"

/*
typedef enum object_type
{
    objtype_no_type,
    objtype_WEAPON,
    objtype_OFFHAND,
    objtype_RANGED,
    objtype_LIGHT,
    objtype_ARMOR,
    objtype_HELMET,
    objtype_CLOAK,
    objtype_GLOVES,
    objtype_BOOTS,
    objtype_AMULET,
    objtype_RING,
    objtype_SCROLL,
    objtype_BOOK,
    objtype_FLASK,
    objtype_GOLD,
    objtype_AMMUNITION,
    objtype_FOOD,
    objtype_WAND,
    objtype_CONTAINER
} object_type_t;

class object_description
{
  private:
    std::string name, description;
    object_type_t type;
    uint32_t color;
    dice hit, damage, dodge, defence, weight, speed, attribute, value;

  public:
    object_description() : name(), description(), type(objtype_no_type),
                           color(0), hit(), damage(),
                           dodge(), defence(), weight(),
                           speed(), attribute(), value()
    {
    }
    void set(const std::string &name,
             const std::string &description,
             const object_type_t type,
             const uint32_t color,
             const dice &hit,
             const dice &damage,
             const dice &dodge,
             const dice &defence,
             const dice &weight,
             const dice &speed,
             const dice &attrubute,
             const dice &value);
    std::ostream &print(std::ostream &o);
    //Need all these accessors because otherwise there is a
   //circular dependancy that is difficult to get around.
    inline const std::string &get_name() const { return name; }
    inline const std::string &get_description() const { return description; }
    inline const object_type_t get_type() const { return type; }
    inline const uint32_t get_color() const { return color; }
    inline const dice &get_hit() const { return hit; }
    inline const dice &get_damage() const { return damage; }
    inline const dice &get_dodge() const { return dodge; }
    inline const dice &get_defence() const { return defence; }
    inline const dice &get_weight() const { return weight; }
    inline const dice &get_speed() const { return speed; }
    inline const dice &get_attribute() const { return attribute; }
    inline const dice &get_value() const { return value; }
};

class monster_description
{
  private:
    std::string name, description;
    char symbol;
    std::vector<uint32_t> color;
    uint32_t abilities;
    dice speed, hitpoints, damage;

  public:
    monster_description() : name(), description(), symbol(0), color(0),
                            abilities(0), speed(), hitpoints(), damage()
    {
    }
    void set(const std::string &name,
             const std::string &description,
             const char symbol,
             const std::vector<uint32_t> &color,
             const dice &speed,
             const uint32_t abilities,
             const dice &hitpoints,
             const dice &damage);
    std::ostream &print(std::ostream &o);
    inline const std::string get_name() const { return name; }
    inline const std::string get_description() const { return description; }
    inline const dice &get_speed() const { return speed; }
    inline const uint32_t get_color() const { return color[0]; }
    inline const dice &get_hitpoints() const { return hitpoints; }
    inline const dice &get_damage() const { return damage; }
    inline const uint32_t get_abilities() const { return abilities; }
    char get_symbol() { return symbol; }
    NPC *genMonster()
    {
        NPC *n = new NPC;
        n->name = this->name;               //Valgrind says leak is happening here
        n->description = this->description; //Valgrind says leak is happening here
        n->symbol = this->symbol;
        n->color = this->color[0];
        n->abilities = this->abilities;
        n->speed = this->speed.roll();
        n->hitpoints = this->hitpoints.roll();
        n->damage = this->damage;
        return n;
    }
};

class monster_description
{
  private:
    std::string name, description;
    char symbol;
    std::vector<uint32_t> color;
    uint32_t abilities;
    dice speed, hitpoints, damage;

  public:
    monster_description() : name(), description(), symbol(0), color(0),
                            abilities(0), speed(), hitpoints(), damage()
    {
    }
    void set(const std::string &name,
             const std::string &description,
             const char symbol,
             const std::vector<uint32_t> &color,
             const dice &speed,
             const uint32_t abilities,
             const dice &hitpoints,
             const dice &damage);
    std::ostream &print(std::ostream &o);
    inline const std::string get_name() const { return name; }
    inline const std::string get_description() const { return description; }
    inline const dice &get_speed() const { return speed; }
    inline const uint32_t get_color() const { return color[0]; }
    inline const dice &get_hitpoints() const { return hitpoints; }
    inline const dice &get_damage() const { return damage; }
    inline const uint32_t get_abilities() const { return abilities; }
    char get_symbol() { return symbol; }
    Monster *genMonster()
    {
        Monster *n = new Monster;
        n->name = this->name;               //Valgrind says leak is happening here
        n->description = this->description; //Valgrind says leak is happening here
        n->symbol = this->symbol;
        n->color = this->color[0];
        n->abilities = this->abilities;
        n->speed_dice = this->speed;
        n->hitpoints = this->hitpoints;
        n->damage = this->damage;
        return n;
    }
};
*/

void load_monster_desc(char *path);
void load_item_desc(char *path);
void print_monster_desc();
void print_monster_desc_with_type();
void print_item_desc();
void print_item_desc_with_type();

#endif