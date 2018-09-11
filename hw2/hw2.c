#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define ROW 21
#define COL 80
#define ROCK ' '
#define ROOM '.'
#define CORRIDOR '#'
#define ROCK_H 10
#define ROOM_H 0
#define CORRIDOR_H 0

enum action
{
	action_save,
	action_load,
	action_loadsave
};

typedef struct dungeonCell
{
	char space;
	int hardness;
} Cell;

typedef struct dungeonRoom
{
	int row;
	int col;
	int width;
	int height;
} Room;

Cell dungeon[ROW][COL];

void initDungeon()
{
	for(int i=0; i<ROW; i++)
	{
		for(int j=0; j<COL; j++)
		{
			dungeon[i][j].space = ROCK;
			dungeon[i][j].hardness = ROCK_H;
		}
	}
}


int getRandom(int modulus, int min)
{
	return rand() % modulus + min;
}

void printDungeon()
{
	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");

	for (int i=0; i<ROW; i++)
	{
		printf("|");
		for (int j=0; j<COL; j++)
		{
			printf("%c", dungeon[i][j].space);
		}
		printf("|\n");
	}

	for (int i=0; i<COL+2; i++)
	{
		printf("-");
	}
	printf("\n");
}

bool isInside (int row, int col)
{
	//is room not on edge or outside of dungeon
	return row > 0 && col > 0 && row < ROW && col < COL;
}

bool isValidRoom(int row, int col, int width, int height)
{
	//is current space free
	if (dungeon[row][col].space != ROCK) return false;
	if (!isInside(row, col) || !isInside(row+height, col+width)) return false;
	//is touching another room
	for (int i=row-1; i<row+height+2; i++)
	{
		if (dungeon[i][col-1].space != ROCK) return false; //left
		if (dungeon[i][col+width+1].space != ROCK) return false;//right
	}
	for (int j=col-1; j<col+width+2; j++)
	{
		if (dungeon[row-1][j].space != ROCK) return false;//top
		if (dungeon[row+height+1][j].space != ROCK) return false;//bottom
	}
	return true;
}

Room newRoom()
{
	Room r;
	r.row = getRandom(ROW, 0);
	r.col = getRandom(COL, 0);
	r.width = getRandom(7, 3);
	r.height = getRandom(6, 2);

	bool validRoom = isValidRoom(r.row, r.col, r.width, r.height);
	if (validRoom)
	{
		for (int i=r.row; i<r.row+r.height; i++)
		{
			for (int j=r.col; j<r.col+r.width; j++)
			{
				dungeon[i][j].space = ROOM;
				dungeon[i][j].hardness = ROOM_H;
			}
		}
	}
	else
	{
		return newRoom();
	}

	return r;
}

int distance(int aRow, int aCol, int bRow, int bCol)
{
	int row = abs(aRow - bRow);
	int col = abs(aCol - bCol);

	return row*row + col*col;
}

int minimum(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}

void newCorridor(int aRow, int aCol, int bRow, int bCol)
{
	if (aRow <= bRow) 	//now at dungeon[aRow][aCol]
	{
		for (int i=aRow; i<=bRow; i++)
		{
			if (dungeon[i][aCol].space == ROCK)
			{
				dungeon[i][aCol].space = CORRIDOR;
				dungeon[i][aCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[bRow][aCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[bRow][i].space == ROCK)
				{
					dungeon[bRow][i].space = CORRIDOR;
					dungeon[bRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}
	else
	{
		for (int i=bRow; i<=aRow; i++)
		{
			if (dungeon[i][bCol].space == ROCK)
			{
				dungeon[i][bCol].space = CORRIDOR;
				dungeon[i][bCol].hardness = CORRIDOR_H;
			}
		}
		if (aCol <= bCol) 	//now at dungeon[aRow][bCol]
		{
			for (int i=aCol; i<=bCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
		else
		{
			for (int i=bCol; i<=aCol; i++)
			{
				if (dungeon[aRow][i].space == ROCK)
				{
					dungeon[aRow][i].space = CORRIDOR;
					dungeon[aRow][i].hardness = CORRIDOR_H;
				}
			}
		}
	}

}

void generateDungeon(int n)
{
	Room rooms[n];

	for (int i=0; i<n; i++)
	{
		rooms[i] = newRoom();
	}

	for (int i=0; i<n-1; i++)
	{
		newCorridor(rooms[i].row, rooms[i].col, rooms[i+1].row, rooms[i+1].col);
	}
}

int main(int argc, char *argv[])
{
	enum action act;
	FILE *f;


	//initial dungeon
	initDungeon();

	//set up random
	int seed = time(NULL);
	printf("seed = %d\n", seed);
	srand(seed);

	//generate random number of rooms
	int n = getRandom(7, 5);
	generateDungeon(n);

	printDungeon();


	if (argv[1][0] != '-' || argv[1][1] != '-')
	{
		fprintf(stderr, "Bad format\n");
		return -1;
	}
/*	
	if (argc == 2)
	{
		if (argc[1] == '--save')
		{
			action = action_save;
		}
		else if (argc[1] == '--load')
		{
			action = action_load;
		}
	}
	else if (argc == 3)
	{
		if (argc[1] == '--save' && argc[2] == '--load' || argc[1] == '--load' && argc[2] == '--save')
		{
			action = action_loadsave;
		}
	}
	
	f = fopen(argv[2], "r");
	if (!f)
	{
		fprintf(stderr, "Failed to open %s\n", argv[2]);
		return -1;
	}


	switch (action)
	{
	case action_save:
	fread(&s, sizeof (s), 1, f);
	printf("Read %d %d as binary\n", s.i, s.j);
	break;

	case action_load:
	fscanf(f, "%hhd %hhd", &s.i, &s.j);
	printf("Read %d %d as text\n", s.i, s.j);
	break;

	case action_loadsave::
	fwrite(&s, sizeof (s), 1, f);
	printf("Wrote %d %d binary\n", s.i, s.j);
	break;
	}

	return 0;

*/



	return 0;
}


/*
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

  // Valid switches are -wb -rb -wt and -rt
  if (argv[1][0] != '-') {
    fprintf(stderr, "Bad argument format.\n");

    return -1;
  }

  // From this point on, assume arguments are good
  if (argv[1][1] == 'w') {
    // Writing
    if (argv[1][2] == 'b') {
      action = action_write_binary;
    } else {
      action = action_write_text;
    }
    f = fopen(argv[2], "w");
  } else {
    // Reading
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

*/


