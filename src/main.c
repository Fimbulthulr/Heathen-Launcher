/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  19/10/20 20:44:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (),
 *   Organization:
 *
 * =====================================================================================
 */

#include "defines.h"
#include "launch_game.h"
#include "state_switcher.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <curses.h>

extern char **environ;

int
main
    (int argc,
    char **argv)
{
	//switching to game folder
	char *home = getenv("HOME");
	int err = chdir(home);



	if(err == -1)
	{
		perror("could not switch to home\n");
		exit(EXIT_FAILURE);
	}
	err = chdir(PDX_PATH);
	if(err == -1)
	{
		perror("could not switch to paradox user directory\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(HEATHEN_PATH "/gamepaths", O_RDONLY);
	struct stat statbuf;
	fstat(fd, &statbuf);
	ssize_t filesize = statbuf.st_size;
	char *file = calloc(sizeof(*file), filesize + 1);
	ssize_t n_read = 0;
	while(n_read < filesize)
	{
		ssize_t tmp = read(fd, file + n_read, filesize - n_read);
		if(tmp == -1)
		{
			perror("Gamepath read");
			exit(1);
		}
		n_read += tmp;
	}
	close(fd);
	
	char *start, *end;
	size_t length;

	struct launcher_data data = {};

#ifdef HAS_CK3
	start = strstr(file, "ck3");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	length = end - start;
	data.gamepaths.ck3 = calloc(sizeof(*data.gamepaths.ck3), length);
	memcpy(data.gamepaths.ck3, start + 1, length - 1);
#endif
#ifdef HAS_EU4
	start = strstr(file, "eu4");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	length = end - start;
	data.gamepaths.eu4 = calloc(sizeof(*data.gamepaths.eu4), length);
	memcpy(data.gamepaths.eu4, start + 1, length - 1);
#endif
#ifdef HAS_HOI4
	start = strstr(file, "hoi4");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	length = end - start;
	data.gamepaths.hoi4 = calloc(sizeof(*data.gamepaths.hoi4), length);
	memcpy(data.gamepaths.hoi4, start + 1, length - 1);
#endif
#ifdef HAS_IMPERATOR
	start = strstr(file, "imperator");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	length = end - start;
	data.gamepaths.imperator = calloc(sizeof(*data.gamepaths.imperator), length);
	memcpy(data.gamepaths.imperator, start + 1, length - 1);
#endif
#ifdef HAS_STELLARIS
	start = strstr(file, "stellaris");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	length = end - start;
	data.gamepaths.stellaris = calloc(sizeof(*data.gamepaths.stellaris), length);
	memcpy(data.gamepaths.stellaris, start + 1, length - 1);
#endif
	free(file);
	

	setlocale(LC_CTYPE, ""); // unicode support
	initscr();          /* Start curses mode		*/
	start_color();          /* Start the color functionality */
	cbreak();           /* Line buffering disabled, Pass on
	                     * everty thing to me		*/
	keypad(stdscr, TRUE);       /* I need that nifty F1		*/
	noecho();
	curs_set(0);




	//create new window for title

	WINDOW *title = newwin(3, 0, 0, 0);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(title, COLOR_PAIR(1) );
	wborder(title, '#', '#', '#', '#', '#', '#', '#', '#');
	mvwprintw(title, 1, (COLS - 16) / 2, "Heathen Launcher");
	wattroff(title, COLOR_PAIR(1) );
	refresh();
	wrefresh(title);



	data.lines = LINES - 3;
	data.rows = COLS;
	data.state = Main;
	state_switcher(&data);

	endwin();
}



