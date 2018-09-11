#include <stdio.h>

void times_table(int table[10][10]);

int main(int argc, char *argv[])
{
  int table[10][10];
  int i, j;

  times_table(table);

  for (j = 0; j< 10; j++) {
    for (i = 0; i< 10; i++) {
      printf("%3d ", table[j][i]);
    }
    printf("\n");
  }

  return 0;
}

void times_table(int table[10][10])
{
  int i, j;

  for (j = 0; j < 10; j++) {
    for (i = 0; i < 10; i++) {
      table[j][i] = (j + 1) * (i + 1);
    }
  }
}
