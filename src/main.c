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

#include <stdlib.h>
#include <stdio.h>
#include <locale.h>

#include <sys/types.h>
#include <sys/wait.h>

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
/*	err = chdir(CK3_PATH);
	if(err == -1)
	{
		perror("could not switch to CKIII directory\n");
		exit(EXIT_FAILURE);
	}
*/
	setlocale(LC_CTYPE, ""); // unicode support
	initscr();			/* Start curses mode		*/
	start_color();			/* Start the color functionality */
	cbreak();			/* Line buffering disabled, Pass on
															 * everty thing to me		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1		*/
	noecho();
	curs_set(0);




	//create new window for title
	
	WINDOW *title = newwin(3, 0, 0, 0);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	wattron(title,COLOR_PAIR(1));
	wborder(title, '#', '#', '#', '#', '#', '#', '#', '#');
	mvwprintw(title, 1, (COLS-16)/2, "Heathen Launcher");		
	wattroff(title, COLOR_PAIR(1));
	refresh();
	wrefresh(title);


	struct launcher_data data = {};
	data.lines = LINES - 3;
	data.rows = COLS;
	data.state = Main;
	state_switcher(&data);

	endwin();
	/*
	char *gameargs[] = {"heathen_launcher/ck3", NULL};
	pid_t pid = launch_game(gameargs);
	printf("child pid is %d\n", pid);
	int status;
	while(1)
	{
		pid_t return_pid = waitpid(pid, &status, WNOHANG);
		if(return_pid == 0)
		{
			printf("game is running\n");
		}
		else if(WIFEXITED(status))
		{
			printf("exited with code %d\n", WEXITSTATUS(status));
			break;
		}
		else if(WIFSIGNALED(status))
		{
			printf("game terminated with signal %d\n", WTERMSIG(status));
			break;
		}
		sleep(1);

	}
	//*/
}
