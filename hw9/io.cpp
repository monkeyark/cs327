#include <ncurses.h>
#include <curses.h>
#include <string.h>

#include "move.h"
#include "io.h"

void clear_message(WINDOW *win)
{
	//clean previous message
	int i, j;
	for (i = 0, j = 0; j < TERMINAL_COL; j++)
	{
		mvwprintw(win, i, j, " ");
	}
}

void print_dungeon_fog_ncurses(WINDOW *game, const char *message)
{
	//clean previous message
	clear_message(game);

	int i, j;
	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(game, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(game, i, j, m);
		wattroff(game, COLOR_PAIR(COLOR_CYAN));
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
			short color;
			int npc_index = is_monster((i - 1), j);
			int item_index = is_item((i - 1), j);
			if (is_visible_terrain((i - 1), j))
			{
				if ((i - 1) == dungeon.pc.row && j == dungeon.pc.col)
				{
					mvwprintw(game, i, j, "@");
					//mvwprintw(game, i + TERMINAL_ROW, j, "@"); //DEBUG
				}
				else if (!(npc_index < 0))
				{
					color = dungeon.monster[npc_index].color_display;
					init_pair(color, color, COLOR_BLACK);
					wattron(game, COLOR_PAIR(color));
					mvwprintw(game, i, j, "%c", dungeon.monster[npc_index].symbol);
					wattroff(game, COLOR_PAIR(color));

					//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.monster[npc_index].color_display); //DEBUG
				}
				else if (!(item_index < 0))
				{
					color = dungeon.item[item_index].color_display;
					init_pair(color, color, COLOR_BLACK);
					wattron(game, COLOR_PAIR(color));
					mvwprintw(game, i, j, "%c", dungeon.item[item_index].symbol);
					wattroff(game, COLOR_PAIR(color));

					//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.item[item_index].color_display); //DEBUG
				}
				else
				{
					mvwprintw(game, i, j, "%c", dungeon.map[i - 1][j].terrain);
					//mvwprintw(game, i + TERMINAL_ROW, j, "%c", dungeon.map[i - 1][j].terrain); //DEBUG
				}
			}
			else
			{
				if (dungeon.pc.vision[i - 1][j])
				{
					mvwprintw(game, i, j, "%c", dungeon.map[i - 1][j].terrain);
				}
				else
				{
					mvwprintw(game, i, j, " ");
				}
			}
			//TODO some items are not added to dungeon.map[i][j].space
			//mvwprintw(game, i + TERMINAL_ROW, j, "%c", dungeon.map[i - 1][j].space); //DEBUG
		}
	}
}

void print_dungeon_ncurses(WINDOW *game, const char *message)
{
	//clean previous message
	clear_message(game);

	int i, j;
	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(game, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(game, i, j, m);
		wattroff(game, COLOR_PAIR(COLOR_CYAN));
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
			short color;
			int npc_index = is_monster((i - 1), j);
			int item_index = is_item((i - 1), j);
			if ((i - 1) == dungeon.pc.row && j == dungeon.pc.col)
			{
				mvwprintw(game, i, j, "@");
				//mvwprintw(game, i + TERMINAL_ROW, j, "@"); //DEBUG
			}
			else if (!(npc_index < 0))
			{
				color = dungeon.monster[npc_index].color_display;
				init_pair(color, color, COLOR_BLACK);
				wattron(game, COLOR_PAIR(color));
				mvwprintw(game, i, j, "%c", dungeon.monster[npc_index].symbol);
				wattroff(game, COLOR_PAIR(color));

				//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.monster[npc_index].color_display); //DEBUG
			}
			else if (!(item_index < 0))
			{
				color = dungeon.item[item_index].color_display;
				init_pair(color, color, COLOR_BLACK);
				wattron(game, COLOR_PAIR(color));
				mvwprintw(game, i, j, "%c", dungeon.item[item_index].symbol);
				wattroff(game, COLOR_PAIR(color));

				//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.item[item_index].color_display); //DEBUG
			}
			else
			{
				mvwprintw(game, i, j, "%c", dungeon.map[i - 1][j].terrain);
				//mvwprintw(game, i + TERMINAL_ROW, j, "%c", dungeon.map[i - 1][j].terrain); //DEBUG
			}
		}
	}
}

void print_dungeon_teleport_ncurses(WINDOW *game, const char *message)
{
	//clean previous message
	clear_message(game);

	int i, j;
	//print current message
	const char *m = message;
	for (i = 0, j = 0; *m; m++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(game, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(game, i, j, m);
		wattroff(game, COLOR_PAIR(COLOR_CYAN));
	}

	//print dungeon
	for (i = 1; i < ROW + 1; i++)
	{
		for (j = 0; j < COL; j++)
		{
			short color;
			int npc_index = is_monster((i - 1), j);
			int item_index = is_item((i - 1), j);
			if ((i - 1) == dungeon.teleport_row && j == dungeon.teleport_col)
			{
				mvwprintw(game, i, j, "*");
				//mvwprintw(game, i + TERMINAL_ROW, j, "*"); //DEBUG
			}
			else if ((i - 1) == dungeon.pc.row && j == dungeon.pc.col)
			{
				mvwprintw(game, i, j, "@");
				//mvwprintw(game, i + TERMINAL_ROW, j, "@"); //DEBUG
			}
			else if (!(npc_index < 0))
			{
				color = dungeon.monster[npc_index].color_display;
				init_pair(color, color, COLOR_BLACK);
				wattron(game, COLOR_PAIR(color));
				mvwprintw(game, i, j, "%c", dungeon.monster[npc_index].symbol);
				wattroff(game, COLOR_PAIR(color));

				//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.monster[npc_index].color_display); //DEBUG
			}
			else if (!(item_index < 0))
			{
				color = dungeon.item[item_index].color_display;
				init_pair(color, color, COLOR_BLACK);
				wattron(game, COLOR_PAIR(color));
				mvwprintw(game, i, j, "%c", dungeon.item[item_index].symbol);
				wattroff(game, COLOR_PAIR(color));

				//mvwprintw(game, i + TERMINAL_ROW, j, "%d", dungeon.item[item_index].color_display); //DEBUG
			}
			else
			{
				mvwprintw(game, i, j, "%c", dungeon.map[i - 1][j].terrain);
				//mvwprintw(game, i + TERMINAL_ROW, j, "%c", dungeon.map[i - 1][j].terrain); //DEBUG
			}
		}
	}
}

void print_equipment_ncurses(WINDOW *list, const char *message)
{
	int i, j;
	for (i = 0, j = 0; *message; message++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(list, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(list, i, j, message);
		wattroff(list, COLOR_PAIR(COLOR_CYAN));
	}

	char str[TERMINAL_COL];
	char *m;
	for (i = 1, j = 0; i < NUM_EQUIPMENT + 1; i++, j++)
	{
		if (dungeon.pc.equipment[j].rarity != 0)
		{
			Item item = dungeon.pc.equipment[j];
			sprintf(str, "%c) %6s --- %s", j + 'a', item.type_string, item.name);
		}
		else
		{
			sprintf(str, "%c)", j + 'a');
		}
		m = str;

		for (int n = 0; *m; m++, n++)
		{
			mvwprintw(list, i, n, m);
		}
	}
}

void print_iventory_ncurses(WINDOW *list, const char *message)
{
	int i, j;
	for (i = 0, j = 0; *message; message++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(list, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(list, i, j, message);
		wattroff(list, COLOR_PAIR(COLOR_CYAN));
	}

	char str[TERMINAL_COL];
	char *m;
	for (i = 1, j = 0; i < PC_INVENTORY + 1; i++, j++)
	{
		if ((dungeon.pc.inventory[j]).rarity)
		//if (&(dungeon.pc.inventory[j]) != NULL)//TODO
		{
			Item item = dungeon.pc.inventory[j];
			sprintf(str, "%2d) %6s --- %s", j, item.type_string, item.name);
		}
		else
		{
			sprintf(str, "%2d)", j);
		}
		m = str;

		for (int n = 0; *m; m++, n++)
		{
			mvwprintw(list, i, n, m);
		}
	}
	
/*
	for (i = 1, j = 0; i < PC_INVENTORY + 1; i++, j++)
	{
		char str[TERMINAL_COL];
		if ((dungeon.pc.inventory[j]).rarity)
		//if (&(dungeon.pc.inventory[j]) != NULL)//TODO
		{
			Item item = dungeon.pc.inventory[j];
			sprintf(str, "%d) %6s --- %s", j, item.type_string, item.name);
		}
		else
		{
			sprintf(str, "%d)", j);
		}
		for (int n = 0; str[n]; n++)
		{
			mvwprintw(list, i, n, str);
		}
	}
	*/
}

const char *equip_item(int index)
{
	std::string message;
	if ((dungeon.pc.inventory[index]).rarity)
	{
		Item *inventory_item = &(dungeon.pc.inventory[index]);
		int item_type = inventory_item->type;
		std::string item_name(inventory_item->name);
		//check if item is ring
		if (item_type == RING)
		{
			if (dungeon.pc.equipment_open[RING]) //first ring slot open
			{
				//pass item from inventory to equipment
				dungeon.pc.equipment[RING] = dungeon.pc.inventory[index];
				dungeon.pc.equipment_open[RING] = false;
				dungeon.pc.inventory_size--;
				//set inventory memory block of indexed item to 0, match calloc 0
				memset(inventory_item, 0, sizeof(Item));
				
				message = "you have equiped - " + item_name;
			}
			else if (dungeon.pc.equipment_open[RING_SEC]) //second ring slot open
			{
				dungeon.pc.equipment[RING_SEC] = dungeon.pc.inventory[index];
				dungeon.pc.equipment_open[RING_SEC] = false;
				dungeon.pc.inventory_size--;
				//set inventory memory block of indexed item to 0, match calloc 0
				memset(inventory_item, 0, sizeof(Item));

				message = "you have equiped - " + item_name;
			}
			else
			{
				message = "no place for third ring";
			}
		}
		else //item is not ring
		{
			if (dungeon.pc.equipment_open[item_type])
			{
				dungeon.pc.equipment[item_type] = dungeon.pc.inventory[index];
				dungeon.pc.equipment_open[item_type] = false;
				dungeon.pc.inventory_size--;
				//set inventory memory block of indexed item to 0, match calloc 0
				memset(inventory_item, 0, sizeof(Item));
				message = "you have equiped - " + item_name;
			}
			else
			{
				message = "no place for second item";
			}
		}
	}
	else
	{
		message = "invaild selection";
	}

	return message.c_str();
}

const char *drop_item(int index)
{
	std::string message;
	if ((dungeon.pc.inventory[index]).rarity)
	{
		Item *inventory_item = &(dungeon.pc.inventory[index]);
		std::string item_name(inventory_item->name);
		int birth = inventory_item->birth;
		if ((is_item(dungeon.pc.row, dungeon.pc.col) < 0))//check if current terrain open
		{
			dungeon.item[birth].row = dungeon.pc.row;
			dungeon.item[birth].col = dungeon.pc.col;
		}
		else //random drop item
		{
			do
			{
				dungeon.item[birth].row = get_random(ROW, 0);
				dungeon.item[birth].col = get_random(COL, 0);
			}
			while (dungeon.map[dungeon.item[birth].row][dungeon.item[birth].col].space != ROOM &&
					dungeon.map[dungeon.item[birth].row][dungeon.item[birth].col].space != CORRIDOR);
		}

		dungeon.pc.inventory_size--;
		memset(inventory_item, 0, sizeof(Item));

		message = "you have dropped - " + item_name;
	}
	else
	{
		message = "nothing to drop";
	}

	return message.c_str();
}

const char *takeoff_item(int index)
{
	std::string message;
	if (dungeon.pc.equipment[index].rarity)
	{
		Item *equip_item = &(dungeon.pc.equipment[index]);
		std::string item_name(equip_item->name);

		if (dungeon.pc.inventory_size != PC_INVENTORY)
		{
			for (int i = 0; i < PC_INVENTORY; i++)
			{
				if ((dungeon.pc.inventory[i]).rarity == 0)
				{
					dungeon.pc.inventory[index] = dungeon.pc.equipment[i];
					dungeon.pc.inventory_size++;
					dungeon.pc.equipment_open[index] = false;
					break;
				}
			}

			//set inventory memory block of indexed item to 0, match calloc 0
			memset(equip_item, 0, sizeof(Item));
			message = "you have took off - " + item_name;
		}
		else
		{
			int birth = equip_item->birth;
			if ((is_item(dungeon.pc.row, dungeon.pc.col) < 0)) //check if current terrain open
			{
				dungeon.item[birth].row = dungeon.pc.row;
				dungeon.item[birth].col = dungeon.pc.col;
			}
			else //random drop item
			{
				do
				{
					dungeon.item[birth].row = get_random(ROW, 0);
					dungeon.item[birth].col = get_random(COL, 0);
				}
				while (dungeon.map[dungeon.item[birth].row][dungeon.item[birth].col].space != ROOM &&
						dungeon.map[dungeon.item[birth].row][dungeon.item[birth].col].space != CORRIDOR);
			}

			dungeon.pc.equipment_open[index] = false;
			memset(equip_item, 0, sizeof(Item));

			message = "inventory is full, you have dropped - " + item_name;
		}
	}
	else
	{
		message = "invaild selection";
	}

	return message.c_str();
}

void item_wear()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "press number key to select item to wear, ESC to return";
	while (run)
	{
		print_iventory_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
			case 27:
				run = false;
				break;
			case '0':
				clear_message(list);
				message = equip_item(0);
				break;
			case '1':
				clear_message(list);
				message = equip_item(1);
				break;
			case '2':
				clear_message(list);
				message = equip_item(2);
				break;
			case '3':
				clear_message(list);
				message = equip_item(3);
				break;
			case '4':
				clear_message(list);
				message = equip_item(4);
				break;
			case '5':
				clear_message(list);
				message = equip_item(5);
				break;
			case '6':
				clear_message(list);
				message = equip_item(6);
				break;
			case '7':
				clear_message(list);
				message = equip_item(7);
				break;
			case '8':
				clear_message(list);
				message = equip_item(8);
				break;
			case '9':
				clear_message(list);
				message = equip_item(9);
				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void item_expunge()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "press number key to expunge item, ESC to return";
	while (run)
	{
		print_iventory_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
			case 27:
				run = false;
				break;
			case '0':

				break;
			case '1':

				break;
			case '2':

				break;
			case '3':

				break;
			case '4':

				break;
			case '5':

				break;
			case '6':

				break;
			case '7':

				break;
			case '8':

				break;
			case '9':

				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void item_drop()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "press number key to drop item, ESC to return";
	while (run)
	{
		print_iventory_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
			case 27:
				run = false;
				break;
			case '0':
				clear_message(list);
				message = drop_item(0);
				break;
			case '1':
				clear_message(list);
				message = drop_item(1);
				break;
			case '2':
				clear_message(list);
				message = drop_item(2);
				break;
			case '3':
				clear_message(list);
				message = drop_item(3);
				break;
			case '4':
				clear_message(list);
				message = drop_item(4);
				break;
			case '5':
				clear_message(list);
				message = drop_item(5);
				break;
			case '6':
				clear_message(list);
				message = drop_item(6);
				break;
			case '7':
				clear_message(list);
				message = drop_item(7);
				break;
			case '8':
				clear_message(list);
				message = drop_item(8);
				break;
			case '9':
				clear_message(list);
				message = drop_item(9);
				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void print_item_descr(WINDOW *list, int index)
{
	int i, j;
	const char *message;
	if ((dungeon.pc.inventory[index]).rarity)
	{
		message = "press number key to return";
		wprintw(list, "\n");
		const char *description = dungeon.pc.inventory[index].description;
		for (; *description;)
		{
			std::string s(1, *description);
			const char *m = s.c_str();
			wprintw(list, m);
			description++;
		}
	}
	else
	{
		message = "there is no such item";
	}

	for (i = 0, j = 0; *message; message++, j++)
	{
		init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
		wattron(list, COLOR_PAIR(COLOR_CYAN));
		mvwprintw(list, i, j, message);
		wattroff(list, COLOR_PAIR(COLOR_CYAN));
	}
}

void item_inspect()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	bool desc = false;
	int index;
	const char *message = "press number key to inspect item, i to inventory, ESC to return";
	while (run)
	{
		if (desc)
		{
			print_item_descr(list, index);
		}
		else
		{
			print_iventory_ncurses(list, message);
		}
		
		int key = wgetch(list);
		switch (key)
		{
			case 27:
				run = false;
				break;
			case '0':
				clear_message(list);
				index = 0;
				desc = !desc;
				break;
			case '1':
				clear_message(list);
				index = 1;
				desc = !desc;
				break;
			case '2':
				clear_message(list);
				index = 2;
				desc = !desc;
				break;
			case '3':
				clear_message(list);
				index = 3;
				desc = !desc;
				break;
			case '4':
				clear_message(list);
				index = 4;
				desc = !desc;
				break;
			case '5':
				clear_message(list);
				index = 5;
				desc = !desc;
				break;
			case '6':
				clear_message(list);
				index = 6;
				desc = !desc;
				break;
			case '7':
				clear_message(list);
				index = 7;
				desc = !desc;
				break;
			case '8':
				clear_message(list);
				index = 8;
				desc = !desc;
				break;
			case '9':
				clear_message(list);
				index = 9;
				desc = !desc;
				break;
			case 'i':
				clear_message(list);
				message = "choose one item";
				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void inventory_list()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "inventory list, ESC to return";
	while (run)
	{
		print_iventory_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
		case 27:
			run = false;
			break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void item_takeoff()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "press key to take of item, ESC to return";
	while (run)
	{
		print_equipment_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
			case 27:
				run = false;
				break;
			case 'a':
				clear_message(list);
				message = takeoff_item(WEAPON);
				break;
			case 'b':
				clear_message(list);
				message = takeoff_item(OFFHAND);
				break;
			case 'c':
				clear_message(list);
				message = takeoff_item(RANGED);
				break;
			case 'd':
				clear_message(list);
				message = takeoff_item(ARMOR);
				break;
			case 'e':
				clear_message(list);
				message = takeoff_item(HELMET);
				break;
			case 'f':
				clear_message(list);
				message = takeoff_item(CLOAK);
				break;
			case 'g':
				clear_message(list);
				message = takeoff_item(GLOVES);
				break;
			case 'h':
				clear_message(list);
				message = takeoff_item(BOOTS);
				break;
			case 'i':
				clear_message(list);
				message = takeoff_item(AMULET);
				break;
			case 'j':
				clear_message(list);
				message = takeoff_item(LIGHT);
				break;
			case 'k':
				clear_message(list);
				message = takeoff_item(RING);
				break;
			case 'l':
				clear_message(list);
				message = takeoff_item(RING_SEC);
				break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void equipment_list()
{
	WINDOW *list = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	keypad(list, true);
	bool run = true;
	const char *message = "equipment list, ESC to return";
	while (run)
	{
		print_equipment_ncurses(list, message);
		int key = wgetch(list);
		switch (key)
		{
		case 27:
			run = false;
			break;
		}
	}

	wclrtoeol(list);
	wrefresh(list);
	delwin(list);
}

void print_monster_list_ncurses(WINDOW *list, int start)
{
	int i, j;
	char str[COL];
	for (i = 0, j = start; i < ROW && j < dungeon.num_mon - start; i++, j++)
	{

		NPC npc = dungeon.monster[j];
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

		sprintf(str, "%c:  %2d %s and %2d %s", npc.symbol, abs(row_dis), row_pos, abs(col_dis), col_pos);
		char *m = str;

		for (int n = 0; *m; m++, n++)
		{
			mvwprintw(list, i, n, m);
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
	//WINDOW *teleport = newwin(TERMINAL_ROW * 2, TERMINAL_COL, 0, 0); //DEBUG
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

	while (run)
	{
		print_dungeon_teleport_ncurses(teleport, message);
		int row_move = dungeon.teleport_row - dungeon.pc.row;
		int col_move = dungeon.teleport_col - dungeon.pc.col;
		int key = wgetch(teleport);
		switch (key)
		{
		case KEY_HOME:
			message = move_tp_pointer(-1, -1); //move up-left
			break;
		case KEY_UP:
			message = move_tp_pointer(-1, 0); //move up
			break;
		case KEY_PPAGE:
			message = move_tp_pointer(-1, 1); //move up-right
			break;
		case KEY_RIGHT:
			message = move_tp_pointer(0, 1); //move right
			break;
		case KEY_NPAGE:
			message = move_tp_pointer(1, 1); //move down-right
			break;
		case KEY_DOWN:
			message = move_tp_pointer(1, 0); //move down
			break;
		case KEY_END:
			message = move_tp_pointer(1, -1); //move down-left
			break;
		case KEY_LEFT:
			message = move_tp_pointer(0, -1); //move left
			break;
		case '1':
			message = move_tp_pointer(1, -1); //move down-left
			break;
		case '2':
			message = move_tp_pointer(1, 0); //move down
			break;
		case '3':
			message = move_tp_pointer(1, 1); //move down-right
			break;
		case '4':
			message = move_tp_pointer(0, -1); //move left
			break;
		case '6':
			message = move_tp_pointer(0, 1); //move right
			break;
		case '7':
			message = move_tp_pointer(-1, -1); //move up-left
			break;
		case '8':
			message = move_tp_pointer(-1, 0); //move up
			break;
		case '9':
			message = move_tp_pointer(-1, 1); //move up-right
			break;
		case 'b':
			message = move_tp_pointer(1, -1); //move down-left
			break;
		case 'g':
			message = move_pc_teleport(row_move, col_move);
			run = false;
			break;
		case 'h':
			message = move_tp_pointer(0, -1); //move left
			break;
		case 'j':
			message = move_tp_pointer(1, 0); //move down
			break;
		case 'k':
			message = move_tp_pointer(-1, 0); //move up
			break;
		case 'l':
			message = move_tp_pointer(0, 1); //move right
			break;
		case 'n':
			message = move_tp_pointer(1, 1); //move down-right
			break;
		case 'r':
			do
			{
				dungeon.teleport_row = get_random(ROW, 0);
				dungeon.teleport_col = get_random(COL, 0);
			} while (!is_inside(dungeon.teleport_row, dungeon.teleport_col));

			row_move = dungeon.teleport_row - dungeon.pc.row;
			col_move = dungeon.teleport_col - dungeon.pc.col;
			message = move_pc_teleport(row_move, col_move);
			run = false;
			break;
		case 'u':
			message = move_tp_pointer(-1, 1); //move up-right
			break;
		case 'y':
			message = move_tp_pointer(-1, -1); //move up-left
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
	while (run)
	{
		print_monster_list_ncurses(list, index);
		int key = wgetch(list);
		switch (key)
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
	start_color();
	noecho();
	cbreak();
	WINDOW *game = newwin(TERMINAL_ROW, TERMINAL_COL, 0, 0);
	//WINDOW *game = newwin(TERMINAL_ROW * 2, TERMINAL_COL, 0, 0); //DEBUG

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
	while (run)
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
		switch (key)
		{
		case KEY_HOME:
			message = move_pc(-1, -1); //move up-left
			break;
		case KEY_UP:
			message = move_pc(-1, 0); //move up
			break;
		case KEY_PPAGE:
			message = move_pc(-1, 1); //move up-right
			break;
		case KEY_RIGHT:
			message = move_pc(0, 1); //move right
			break;
		case KEY_NPAGE:
			message = move_pc(1, 1); //move down-right
			break;
		case KEY_DOWN:
			message = move_pc(1, 0); //move down
			break;
		case KEY_END:
			message = move_pc(1, -1); //move down-left
			break;
		case KEY_LEFT:
			message = move_pc(0, -1); //move left
			break;
		case KEY_B2:
			message = move_pc(0, 0); //rest
			break;
		case ' ':
			message = move_pc(0, 0); //rest
			break;
		case '<':
			if (dungeon.map[dungeon.pc.row][dungeon.pc.col].terrain == STAIR_UP)
			{
				//delete_dungeon();
				//generate_dungeon();

				delete_dungeon_desc();
				generate_dungeon_desc();
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
				//delete_dungeon();
				//generate_dungeon();

				delete_dungeon_desc();
				generate_dungeon_desc();
				message = "You went down stair";
			}
			else
			{
				message = "You are not standing on down stair";
			}
			break;
		case '.':
			message = move_pc(0, 0); //rest
			break;
		case '1':
			message = move_pc(1, -1); //move down-left
			break;
		case '2':
			message = move_pc(1, 0); //move down
			break;
		case '3':
			message = move_pc(1, 1); //move down-right
			break;
		case '4':
			message = move_pc(0, -1); //move left
			break;
		case '5':
			message = move_pc(0, 0); //rest
			break;
		case '6':
			message = move_pc(0, 1); //move right
			break;
		case '7':
			message = move_pc(-1, -1); //move up-left
			break;
		case '8':
			message = move_pc(-1, 0); //move up
			break;
		case '9':
			message = move_pc(-1, 1); //move up-right
			break;
		case 'b':
			message = move_pc(1, -1); //move down-left
			break;
		case 'c':
			//TODO
			break;
		case 'd':
			item_drop();
			break;
		case 'e':
			equipment_list();
			break;
		case 'f':
			fog = !fog;
			break;
		case 'g':
			teleport();
			refresh();
			break;
		case 'h':
			message = move_pc(0, -1); //move left
			break;
		case 'i':
			inventory_list();
			break;
		case 'j':
			message = move_pc(1, 0); //move down
			break;
		case 'k':
			message = move_pc(-1, 0); //move up
			break;
		case 'l':
			message = move_pc(0, 1); //move right
			break;
		case 'm':
			monster_list();
			refresh();
			break;
		case 'n':
			message = move_pc(1, 1); //move down-right
			break;
		case 's':
			//TODO
			break;
		case 't':
			item_takeoff();
			break;
		case 'u':
			message = move_pc(-1, 1); //move up-right
			break;
		case 'w':
			item_wear();
			break;
		case 'x':
			item_expunge();
			break;
		case 'y':
			message = move_pc(-1, -1); //move up-left
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
			item_inspect();
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
