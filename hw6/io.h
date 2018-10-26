#ifndef IO_H
# define IO_H

#include "dungeon.h"

#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>



void load_file(FILE *f);
void save_file(FILE *f);

#endif