/*
 * =====================================================================================
 *
 *       Filename:  instance_menu.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  20/10/20 13:56:59
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (),
 *   Organization:
 *
 * =====================================================================================
 */

#include "instance_menu.h"
#include "launch_game.h"

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define ENTRIES 3
#define ENTRY_LENGTH 13
#define GAMES (1 + N_GAMES)
#define GAME_LENGTH 10
#define LAUNCHTYPES 4
#define LAUNCH_LENGTH 7

int
submenu_instance_creation
    (struct launcher_data *data,
    enum selected_game);


void
submenu_game_selection
    (struct launcher_data *data);


void
instance_menu
    (struct launcher_data *data)
{
	char s_menu[ENTRIES][ENTRY_LENGTH] = {"New Instance", "Check Running", "Main Menu"};
	enum e_menu {iNew = 0, iCheck, iMain} selected = iNew;

	WINDOW *w = newwin(ENTRIES + 2, ENTRY_LENGTH + 2, 3, 0);



	box(w, 0, 0);

	for(int i = 0; i < ENTRIES; ++i)
	{
		if(i == selected)
		{
			wattron(w, A_STANDOUT);
			mvwprintw(w, i + 1, 1, "%.*s", ENTRY_LENGTH, s_menu[i]);
			wattroff(w, A_STANDOUT);
		}
		else
		{
			mvwprintw(w, i + 1, 1, "%.*s", ENTRY_LENGTH, s_menu[i]);
		}
	}
	keypad(w, TRUE);

	int c;
	int q = 0;

	while(!q)
	{
		c = wgetch(w);
		mvwprintw(w, selected + 1, 1, "%.*s", ENTRY_LENGTH, s_menu[selected]);
		switch(c)
		{
		case KEY_UP:
		{
			selected ? --selected : 0;
		}
		break;

		case KEY_DOWN:
		{
			(ENTRIES - selected - 1) ? ++selected : 0;
		}
		break;

		case KEY_LEFT:
		{
			selected = iMain;
		}

		case KEY_RIGHT:
		case '\n':            //main enter key
		case KEY_ENTER:
		{
			q = 1;
		}
		break;                             //enter on numeric block
		}
		wattron(w, A_STANDOUT);
		mvwprintw(w, selected + 1, 1, "%.*s", ENTRY_LENGTH, s_menu[selected]);
		wattroff(w, A_STANDOUT);
		if(q == 1 && selected == iNew)
		{
			q = 0;
			submenu_game_selection(data);
		}
	}
	werase(w);
	wrefresh(w);
	delwin(w);
	switch(selected)
	{
	case iCheck:
	{
		data->state = Check;
	}
	break;

	case iMain:
	{
		data->state = Main;
	}
	break;

	case iNew:
	{
		data->state = Quit;
	}
	break;                                            //should not be reached, quit on error
	}
}




void
submenu_game_selection
    (struct launcher_data *data)
{
	char g_menu[GAMES][GAME_LENGTH] = {
#ifdef HAS_CK3
		"CK III",
#endif
#ifdef HAS_EU4
		"EU IV",
#endif
#ifdef HAS_HOI4
		"HoI IV",
#endif
#ifdef HAS_IMPERATOR
		"Imperator",
#endif
#ifdef HAS_STELLARIS
		"Stellaris",
#endif
		"back"
	};
	enum games
	{
#ifdef HAS_CK3
		Ck3,
#endif
#ifdef HAS_EU4
		Eu4,
#endif
#ifdef HAS_HOI4
		Hoi4,
#endif
#ifdef HAS_IMPERATOR
		Imperator,
#endif
#ifdef HAS_STELLARIS
		Stellaris,
#endif
		Back
	} selected = 0;

	WINDOW *w = newwin(GAMES + 2, GAME_LENGTH + 2, 3, ENTRY_LENGTH + 2);



	box(w, 0, 0);

	for(int i = 0; i < GAMES; ++i)
	{
		if(i == selected)
		{
			wattron(w, A_STANDOUT);
			mvwprintw(w, i + 1, 1, "%.*s", GAME_LENGTH, g_menu[i]);
			wattroff(w, A_STANDOUT);
		}
		else
		{
			mvwprintw(w, i + 1, 1, "%.*s", GAME_LENGTH, g_menu[i]);
		}
	}
	keypad(w, TRUE);

	int c;
	int q = 0;

	while(!q)
	{
		c = wgetch(w);
		mvwprintw(w, selected + 1, 1, "%.*s", GAME_LENGTH, g_menu[selected]);
		switch(c)
		{
		case KEY_UP:
		{
			selected ? --selected : 0;
		}
		break;

		case KEY_DOWN:
		{
			(GAMES - selected - 1) ? ++selected : 0;
		}
		break;

		case KEY_LEFT:
		{
			selected = Back;                   //go back on left arow
		}

		case KEY_RIGHT:
		case '\n':            //main enter key
		case KEY_ENTER:
		{
			q = 1;
		}
		break;                             //enter on numeric block
		}
		wattron(w, A_STANDOUT);
		mvwprintw(w, selected + 1, 1, "%.*s", GAME_LENGTH, g_menu[selected]);
		wattroff(w, A_STANDOUT);
		if(q == 1 && selected != Back)
		{
			enum selected_game game;

			switch(selected)
			{
#ifdef HAS_CK3
			case Ck3:
			{
				game = gCk3;
			}
			break;
#endif
#ifdef HAS_EU4
			case Eu4:
			{
				game = gEu4;
			}
			break;
#endif
#ifdef HAS_HOI4
			case Hoi4:
			{
				game = gHoi4;
			}
			break;
#endif
#ifdef HAS_IMPERATOR
			case Imperator:
			{
				game = gImperator;
			}
			break;
#endif
#ifdef HAS_STELLARIS
			case Stellaris:
			{
				game = gStellaris;
			}
			break;
#endif
			case Back:
			{
				game = 0;
			}
			break;                                              //should never be reached
			}
			q = submenu_instance_creation(data, game);
		}
	}
	werase(w);
	wrefresh(w);
	delwin(w);
}




enum selected_mode {mDefault, mDebug, mDevel};


int
submenu_instance_creation
    (struct launcher_data *data,
    enum selected_game game)
{
	char l_menu[LAUNCHTYPES][LAUNCH_LENGTH] = {"Default", "Debug", "Develop",  "back"};
	enum launchtype {Default = 0, Debug, Devel,  Back} selected = 0;

	WINDOW *w = newwin(LAUNCHTYPES + 2, LAUNCH_LENGTH + 2, 3, ENTRY_LENGTH + GAME_LENGTH + 4);



	box(w, 0, 0);

	for(int i = 0; i < LAUNCHTYPES; ++i)
	{
		if(i == selected)
		{
			wattron(w, A_STANDOUT);
			mvwprintw(w, i + 1, 1, "%.*s", LAUNCH_LENGTH, l_menu[i]);
			wattroff(w, A_STANDOUT);
		}
		else
		{
			mvwprintw(w, i + 1, 1, "%.*s", LAUNCH_LENGTH, l_menu[i]);
		}
	}
	keypad(w, TRUE);

	int c;
	int q = 0;

	while(!q)
	{
		c = wgetch(w);
		mvwprintw(w, selected + 1, 1, "%.*s", LAUNCH_LENGTH, l_menu[selected]);
		switch(c)
		{
		case KEY_UP:
		{
			selected ? --selected : 0;
		}
		break;

		case KEY_DOWN:
		{
			(LAUNCHTYPES - selected - 1) ? ++selected : 0;
		}
		break;

		case KEY_LEFT:
		{
			selected = Back;                   //go back on left arow
		}

		case KEY_RIGHT:
		case '\n':            //main enter key
		case KEY_ENTER:
		{
			q = 1;
		}
		break;                             //enter on numeric block
		}
		wattron(w, A_STANDOUT);
		mvwprintw(w, selected + 1, 1, "%.*s", LAUNCH_LENGTH, l_menu[selected]);
		wattroff(w, A_STANDOUT);
	}
	werase(w);
	wrefresh(w);
	delwin(w);

	int ret;
	enum selected_mode mode;

	int
	add_instance
	    (struct launcher_data*,
	    enum selected_game,
	    enum selected_mode);
	switch(selected)
	{
	case Back:
	{
		ret = 0;
	}
	break;

	case Default:
	{
		mode = mDefault;
		ret = add_instance(data, game, mode);
	}
	break;

	case Debug:
	{
		mode = mDebug;
		ret = add_instance(data, game, mode);
	}
	break;

	case Devel:
	{
		mode = mDevel;
		ret = add_instance(data, game, mode);
	}
	break;
	}

	return ret;
}




int
add_instance
    (struct launcher_data *data,
    enum selected_game game,
    enum selected_mode mode)
{
	char *launchpath;


	switch(game)
	{
		case gCk3		: launchpath = data->gamepaths.ck3;			break;
		case gEu4		: launchpath = data->gamepaths.eu4;			break;
		case gHoi4		: launchpath = data->gamepaths.hoi4;		break;
		case gImperator : launchpath = data->gamepaths.imperator;	break;
		case gStellaris : launchpath = data->gamepaths.stellaris;	break;
	}

	char **argv;

	switch(mode)
	{
		case mDefault :	argv = calloc(sizeof(char*), 2); break;
		case mDebug	  :	argv = calloc(sizeof(char*), 3); break;
		case mDevel   :	argv = calloc(sizeof(char*), 4); break;
	}
	
	switch(mode)
	{
		case mDevel   : argv[2] = "-develop";
		case mDebug   : argv[1] = "-debug_mode";
		case mDefault :	argv[0] = launchpath;
	}

	pid_t pid = launch_game(argv);
	free(argv);

	if(pid == 0)
	{
		return 0;
	}
	data->instance = reallocarray(data->instance, sizeof(*data->instance), data->n_instance + 1);
	data->n_instance += 1;

	char *s_game;

	switch(game)
	{
	case gCk3:
	{
		s_game = "CK III";
	}
	break;

	case gEu4:
	{
		s_game = "EU IV";
	}
	break;

	case gHoi4:
	{
		s_game = "HoI IV";
	}
	break;

	case gImperator:
	{
		s_game = "Imperator";
	}
	break;

	case gStellaris:
	{
		s_game = "Stellaris";
	}
	break;
	}
	;
	data->instance[data->n_instance - 1].game = s_game;
	data->instance[data->n_instance - 1].pid = pid;

	char *s_mode;

	switch(mode)
	{
	case mDefault:
	{
		s_mode = "Default";
	}
	break;

	case mDebug:
	{
		s_mode = "Debug";
	}
	break;

	case mDevel:
	{
		s_mode = "Develop";
	}
	break;
	}
	data->instance[data->n_instance - 1].mode = s_mode;

	struct stat statbuf;
	char *proc = NULL;
	size_t size = snprintf(proc, 0, "/proc/%d", pid);

	proc = calloc(sizeof(char), size + 1);
	snprintf(proc, size + 1, "/proc/%d", pid);
	stat(proc, &statbuf);
	free(proc);
	data->instance[data->n_instance - 1].starttime = statbuf.st_atim;

	return 1;
}



