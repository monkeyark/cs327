#include <ncurses.h>
#include <curses.h>
#include <string.h>

#include "move.h"
#include "io.h"


void print_dungeon_fog_ncurses(WINDOW *game, const char *message)
{
	int i, j;
	//clean previous message
	for (i = 0, j = 0; j < TERMINAL_COL; j++)
	{
		mvwprintw(game, i, j, " ");
	}

	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		mvwprintw(game, i, j, m);
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
            if (is_visible_terrain(i, j))
            {
                mvwprintw(game, i, j, "%c", dungeon.map[i][j].space);
            }
            else
            {
                if (dungeon.pc.vision[i][j])
                {
                    mvwprintw(game, i, j, "%c", dungeon.map[i][j].terrain);
                }
                else
                {
                    mvwprintw(game, i, j, " ");
                }
            }
		}
	}
}

void print_dungeon_ncurses(WINDOW *game, const char *message)
{
	int i, j;
	//clean previous message
	for (i = 0, j = 0; j < TERMINAL_COL; j++)
	{
		mvwprintw(game, i, j, " ");
	}

	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		mvwprintw(game, i, j, m);
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
            mvwprintw(game, i, j, "%c", dungeon.map[i][j].space);
		}
	}
}

void print_monster_list_ncurses(WINDOW *list, int start)
{
	int i, j;
	char str[COL];
	for (i = 0, j = start; i < ROW && j < dungeon.num_mon - start; i++, j++)
	{

		Character npc = dungeon.monster[j];
		int row_dis = npc.row - dungeon.pc.row;
		int col_dis = npc.col - dungeon.pc.col;
		const char *row_pos, *col_pos;

		if (row_dis > 0)
		{
			row_pos = "South";
		}
		else if (row_dis < 0)
		{
			row_pos = "North";
		}
		else
		{
			row_pos = "     ";
		}
		if (col_dis > 0)
		{
			col_pos = " East";
		}
		else if (col_dis < 0)
		{
			col_pos = " West";
		}
		else
		{
			col_pos = "     ";
		}

		sprintf(str, "%x:  %2d %s and %2d %s", npc.characteristics, abs(row_dis), row_pos, abs(col_dis), col_pos);
		char *m = str;

		for (int n = 0; *m; m++, n++)
		{
			mvwprintw(list, i, n, m);
		}
	}

}

void print_dungeon_teleport_ncurses(WINDOW *game, const char *message)
{
	int i, j;
	//clean previous message
	for (i = 0, j = 0; j < TERMINAL_COL; j++)
	{
		mvwprintw(game, i, j, " ");
	}

	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		mvwprintw(game, i, j, m);
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
            if (i == dungeon.teleport_row && j == dungeon.teleport_col)
            {
                mvwprintw(game, i, j, "*");
            }
			else if (i == dungeon.pc.row && j == dungeon.pc.col)
			{
				mvwprintw(game, i, j, "@");
			}
			else if (!(is_monster(i, j) < 0))
			{
				mvwprintw(game, i, j, "%x", dungeon.monster[is_monster(i, j)].characteristics);
			}
			else
			{
				mvwprintw(game, i, j, "%c", dungeon.map[i][j].terrain);
			}
		}
	}
}

const char *move_tp_pointer(int row_move, int col_move)
{
	const char *message = "teleporting";
    if (is_inside(dungeon.teleport_row + row_move, dungeon.teleport_col + col_move))
	{
        dungeon.teleport_row += row_move;
        dungeon.teleport_col += col_move;
	}
	else
	{
		message = "Your are out of the dungeon!";
	}

	return message;
}

const char *move_pc_teleport(int row_move, int col_move)
{
	const char *message;
	if (row_move == 0 && col_move == 0)
	{
		move_npc();
		message = "PC is resting!";
	}
	else
	{
        dungeon.map[dungeon.pc.row][dungeon.pc.col].space = dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain;
		dungeon.pc.row += row_move;
		dungeon.pc.col += col_move;
        dungeon.map[dungeon.pc.row][dungeon.pc.col].space = PLAYER;
        dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = 0;

		if (!(is_monster(dungeon.pc.row, dungeon.pc.col) < 0))
		{
			int i = is_monster(dungeon.pc.row, dungeon.pc.col);
			dungeon.monster[i].dead = true;
			dungeon.monster[i].row = -1;
			dungeon.monster[i].col = -1;
		}
        if (dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain == ROCK)
        {
            dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain = CORRIDOR;
            dungeon.map[dungeon.pc.row][dungeon.pc.col].hardness = CORRIDOR_H;
        }
		move_npc();
        remember_map_PC();
		message = "";
	}

	return message;
}

void teleport()
{
    WINDOW *teleport = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(teleport, true);
	bool run = true;
    dungeon.teleport_row = dungeon.pc.row;
    dungeon.teleport_col = dungeon.pc.col;

    char random_seed[10];
    sprintf(random_seed, "%d", dungeon.seed);
    char seed_message[20] = "seed = ";
    strcat(seed_message, random_seed);
    char seed_message_suffix[2] = ";";
    strcat(seed_message, seed_message_suffix);
    const char *message = seed_message;

	while(run)
	{
        print_dungeon_teleport_ncurses(teleport, message);
        int row_move = dungeon.teleport_row - dungeon.pc.row;
        int col_move = dungeon.teleport_col - dungeon.pc.col;
		int key = wgetch(teleport);
		switch(key)
		{
            case KEY_HOME:
				message = move_tp_pointer(-1, -1);//move up-left
				break;
			case KEY_UP:
				message = move_tp_pointer(-1, 0);//move up
				break;
			case KEY_PPAGE:
				message = move_tp_pointer(-1, 1);//move up-right
				break;
			case KEY_RIGHT:
				message = move_tp_pointer(0, 1);//move right
				break;
			case KEY_NPAGE:
				message = move_tp_pointer(1, 1);//move down-right
				break;
			case KEY_DOWN:
				message = move_tp_pointer(1, 0);//move down
				break;
			case KEY_END:
				message = move_tp_pointer(1, -1);//move down-left
				break;
			case KEY_LEFT:
				message = move_tp_pointer(0, -1);//move left
				break;
            case '1':
				message = move_tp_pointer(1, -1);//move down-left
				break;
			case '2':
				message = move_tp_pointer(1, 0);//move down
				break;
			case '3':
				message = move_tp_pointer(1, 1);//move down-right
				break;
			case '4':
				message = move_tp_pointer(0, -1);//move left
				break;
			case '6':
				message = move_tp_pointer(0, 1);//move right
				break;
			case '7':
				message = move_tp_pointer(-1, -1);//move up-left
				break;
			case '8':
				message = move_tp_pointer(-1, 0);//move up
				break;
			case '9':
				message = move_tp_pointer(-1, 1);//move up-right
				break;
			case 'b':
				message = move_tp_pointer(1, -1);//move down-left
				break;
			case 'g':
                message = move_pc_teleport(row_move, col_move);
				run = false;
				break;
			case 'h':
				message = move_tp_pointer(0, -1);//move left
				break;
			case 'j':
				message = move_tp_pointer(1, 0);//move down
				break;
			case 'k':
				message = move_tp_pointer(-1, 0);//move up
				break;
			case 'l':
				message = move_tp_pointer(0, 1);//move right
				break;
			case 'n':
				message = move_tp_pointer(1, 1);//move down-right
				break;
			case 'r':
                do
                {
                    dungeon.teleport_row = get_random(ROW, 0);
                    dungeon.teleport_col = get_random(COL, 0);
                }
                while (!is_inside(dungeon.teleport_row, dungeon.teleport_col));

                row_move = dungeon.teleport_row - dungeon.pc.row;
                col_move = dungeon.teleport_col - dungeon.pc.col;
                message = move_pc_teleport(row_move, col_move);
                run = false;
				break;
			case 'u':
				message = move_tp_pointer(-1, 1);//move up-right
				break;
			case 'y':
				message = move_tp_pointer(-1, -1);//move up-left
				break;
		}
	}

	wclrtoeol(teleport);
	wrefresh(teleport);
	delwin(teleport);
}

void monster_list()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	int index = 0;
	while(run)
	{
		print_monster_list_ncurses(list, index);
		int key = wgetch(list);
		switch(key)
		{
			case 27:
				run = false;
				break;
			case KEY_UP:
				index++;
				index = MIN(index, dungeon.num_mon);
				break;
			case KEY_DOWN:
				index--;
				index = MAX(index, 0);
				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void dungeon_ncurses()
{
	initscr();
	noecho();
	cbreak();
	WINDOW *game = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);

	keypad(game, true);
	bool run = true;
    bool fog = true;


    char random_seed[10];
    sprintf(random_seed, "%d", dungeon.seed);
    char seed_message[20] = "seed = ";
    strcat(seed_message, random_seed);
    char seed_message_suffix[2] = ";";
    strcat(seed_message, seed_message_suffix);

    const char *message = seed_message;
	while(run)
	{
        if (fog)
        {
            print_dungeon_fog_ncurses(game, message);
        }
        else
        {
            print_dungeon_ncurses(game, message);
        }
        
		int key = wgetch(game);
		switch(key)
		{
			case KEY_HOME:
				message = move_pc(-1, -1);//move up-left
				break;
			case KEY_UP:
				message = move_pc(-1, 0);//move up
				break;
			case KEY_PPAGE:
				message = move_pc(-1, 1);//move up-right
				break;
			case KEY_RIGHT:
				message = move_pc(0, 1);//move right
				break;
			case KEY_NPAGE:
				message = move_pc(1, 1);//move down-right
				break;
			case KEY_DOWN:
				message = move_pc(1, 0);//move down
				break;
			case KEY_END:
				message = move_pc(1, -1);//move down-left
				break;
			case KEY_LEFT:
				message = move_pc(0, -1);//move left
				break;
			case KEY_B2:
				message = move_pc(0, 0);//rest
				break;
			case ' ':
				message = move_pc(0, 0);//rest
				break;
			case '<':
				if (dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain == STAIR_UP)
				{
                    delete_dungeon();
					generate_dungeon();
					message = "You went up stair";
				}
				else
				{
					message = "You are not standing on up stair";
				}
				break;
			case '>':
				if (dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain == STAIR_DOWN)
				{
                    delete_dungeon();
					generate_dungeon();
					message = "You went down stair";
				}
				else
				{
					message = "You are not standing on down stair";
				}
				break;
			case '.':
				message = move_pc(0, 0);//rest
				break;
			case '1':
				message = move_pc(1, -1);//move down-left
				break;
			case '2':
				message = move_pc(1, 0);//move down
				break;
			case '3':
				message = move_pc(1, 1);//move down-right
				break;
			case '4':
				message = move_pc(0, -1);//move left
				break;
			case '5':
				message = move_pc(0, 0);//rest
				break;
			case '6':
				message = move_pc(0, 1);//move right
				break;
			case '7':
				message = move_pc(-1, -1);//move up-left
				break;
			case '8':
				message = move_pc(-1, 0);//move up
				break;
			case '9':
				message = move_pc(-1, 1);//move up-right
				break;
			case 'b':
				message = move_pc(1, -1);//move down-left
				break;
			case 'c':
			//TODO
				break;
			case 'd':
			//TODO
				break;
			case 'e':
			//TODO
				break;
			case 'f':
                fog = !fog;
				break;
			case 'g':
				teleport();
				refresh();
				break;
			case 'h':
				message = move_pc(0, -1);//move left
				break;
			case 'i':
			//TODO
				break;
			case 'j':
				message = move_pc(1, 0);//move down
				break;
			case 'k':
				message = move_pc(-1, 0);//move up
				break;
			case 'l':
				message = move_pc(0, 1);//move right
				break;
			case 'm':
				monster_list();
				refresh();
				break;
			case 'n':
				message = move_pc(1, 1);//move down-right
				break;
			case 's':
			//TODO
				break;
			case 't':
			//TODO
				break;
			case 'u':
				message = move_pc(-1, 1);//move up-right
				break;
			case 'w':
			//TODO
				break;
			case 'x':
			//TODO
				break;
			case 'y':
				message = move_pc(-1, -1);//move up-left
				break;
			case 'D':
			//TODO
				break;
			case 'E':
			//TODO
				break;
			case 'H':
			//TODO
				break;
			case 'I':
			//TODO
				break;
			case 'L':
			//TODO
				break;
			case 'Q':
				run = false;
				break;
			case 'T':
			//TODO
				break;
		}

		if (dungeon.pc.dead)
		{
			message = "Player is dead!";
		}
	}

	clrtoeol();
	refresh();
	endwin();
}

