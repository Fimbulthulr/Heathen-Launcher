/*
 * =====================================================================================
 *
 *       Filename:  main_menu.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/10/20 12:29:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "main_menu.h"

#include <curses.h>

#define MENUENTRIES 2

void
main_menu 
	(struct launcher_data *data)
{
	WINDOW* mainmenu = newwin(0,0, 3, 0);
	
	char *menu[MENUENTRIES] = {"Instance management", "Quit"};

	enum menu{mInstance = 0, mQuit} selected = mInstance;
	for(int i = 0; i < MENUENTRIES; ++i)
	{
		if(i == selected)
		{
			wattron(mainmenu, A_STANDOUT);
			mvwprintw(mainmenu, i, 0,"%s",  menu[i]);
			wattroff(mainmenu, A_STANDOUT);
		}
		else
		{
			mvwprintw(mainmenu, i, 0,"%s",  menu[i]);
		}
	}
	keypad( mainmenu, TRUE );
	int c;
	int q;
	q = 0;
	while(!q)
	{
		c = wgetch(mainmenu);
		mvwprintw(mainmenu, selected, 0, "%s", menu[selected]);
		switch(c)
		{
			case KEY_UP		: selected ? --selected :0; break;
			case KEY_DOWN	: (MENUENTRIES - selected - 1) ? ++selected:0;break;
			case KEY_RIGHT	:	
			case '\n'		: //main enter key
			case KEY_ENTER	: q = 1; break;//enter on numeric block
		}
		wattron(mainmenu, A_STANDOUT);
		mvwprintw(mainmenu, selected, 0, "%s", menu[selected]);
		wattroff(mainmenu, A_STANDOUT);
		

	}
	werase(mainmenu);
	wrefresh(mainmenu);
	delwin(mainmenu);
	switch(selected)
	{
		case mInstance	: data->state = Instance; break;
		case mQuit		: data->state = Quit; break;
	}
	
}

