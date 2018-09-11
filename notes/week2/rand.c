#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
  int i;
  int seed;

  seed = time(NULL);

  printf("argv[0] is %s\n", argv[0]);

  if (argc == 2) {
    seed = atoi(argv[1]);
  }

  printf("Seed is %d\n", seed);

  srand(seed);

  printf("Maximum random number: %d\n", RAND_MAX);

  for (i = 0; i < 10; i++) {
    printf("%d\n", rand());
  }

  return 0;
}
