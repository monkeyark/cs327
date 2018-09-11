#include <stdio.h>

enum action {
  action_read_binary,
  action_read_text,
  action_write_binary,
  action_write_text
};

int main(int argc, char *argv[])
{
  enum action action;
  FILE *f;
  struct {
    char i;
    char j;
  } s = { 1, 2 };

  if (argc != 3) {
    fprintf(stderr, "Bad arguments.\n");

    return -1;
  }

  /* Valid switches are -wb -rb -wt and -rt */
  if (argv[1][0] != '-') {
    fprintf(stderr, "Bad argument format.\n");

    return -1;
  }

  /* From this point on, assume arguments are good */
  if (argv[1][1] == 'w') {
    /* Writing */
    if (argv[1][2] == 'b') {
      action = action_write_binary;
    } else {
      action = action_write_text;
    }
    f = fopen(argv[2], "w");
  } else {
    /* Reading */
    if (argv[1][2] == 'b') {
      action = action_read_binary;
    } else {
      action = action_read_text;
    }
    f = fopen(argv[2], "r");
  }

  if (!f) {
    fprintf(stderr, "Failed to open %s\n", argv[2]);

    return -1;
  }

  switch (action) {
  case action_read_binary:
    fread(&s, sizeof (s), 1, f);
    printf("Read %d %d as binary\n", s.i, s.j);
    break;
  case action_read_text:
    fscanf(f, "%hhd %hhd", &s.i, &s.j);
    printf("Read %d %d as text\n", s.i, s.j);
    break;
  case action_write_binary:
    fwrite(&s, sizeof (s), 1, f);
    printf("Wrote %d %d binary\n", s.i, s.j);
    break;
  case action_write_text:
    fprintf(f, "%d %d\n", s.i, s.j);
    printf("Wrote %d %d text\n", s.i, s.j);
    break;
  }

  return 0;
}
