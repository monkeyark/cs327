#include <stdio.h>

#include "cf.h"

int main(int argc, char *argv[])
{
  printf("Water boils at %f C\n", f2c(212.0));
  printf("Water Freezes at %f F\n", c2f(f2c(32)));

  return 0;
}
