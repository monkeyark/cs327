#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdio.h>

#include "dungeon.h"
#include "dice.h"


void load_monster_desc(char *path);
void load_object_desc(char *path);
void print_monster_desc();
void print_monster_desc_with_type();
void print_object_desc();
void print_object_desc_with_type();

#endif