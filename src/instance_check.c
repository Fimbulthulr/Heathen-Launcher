/*
 * =====================================================================================
 *
 *       Filename:  instance_check.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/10/20 17:45:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include "instance_check.h"

#include <stdlib.h>
#include <curses.h> 
#include <time.h>
#include <sys/wait.h>

void 
instance_update
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected);

void
halted_instance
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected);

void
running_instance
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected);

void
instance_check
	(struct launcher_data *data)
{
	WINDOW *w = newwin(data->n_instance*2+2, 66, 3, 0);
	for(int i = 0; i < data->n_instance; ++i)
	{
		mvwprintw(w, i*2,   0, "╠════════════════════════════════════════════════════════════════╣");	//printw due to unicode
		mvwprintw(w, i*2+1, 0, "║                                                                ║");
	}
	mvwprintw(w, 0, 0, "╔════════════════════════════════════════════════════════════════╗");
	mvwprintw(w, data->n_instance*2, 0, "╚════════════════════════════════════════════════════════════════╝");
	int selected = 0;
	instance_update(data, w, selected);

	keypad( w, TRUE );
	halfdelay(5);
	int c;
	int q = 0;
	while(!q)
	{
		c = wgetch(w);
		if(c != ERR)
		{
			switch(c)
			{
				case KEY_UP		: selected ? --selected :0; break;
				case KEY_DOWN	: (data->n_instance - selected) ? ++selected:0;break;
				case KEY_LEFT	: selected = data->n_instance;
				case KEY_RIGHT	:	
				case '\n'		: //main enter key
				case KEY_ENTER	: q = 1; break;//enter on numeric block
			}
		}
		
		instance_update(data, w, selected);
		if(q == 1)
		{
			if(selected < data->n_instance)
			{
				if(data->instance[selected].status == Running)
				{
					running_instance(data, w, selected);
				}
				else
				{
					halted_instance(data, w, selected);
				}
				q = 0;

			}
		}

	}
	werase(w);
	wrefresh(w);
	delwin(w);
	cbreak();	
	data->state = Instance;
}


void 
instance_update
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected)
{

	struct timespec curr_time;
	clock_gettime(CLOCK_REALTIME, &curr_time);
	for(int i = 0; i < data->n_instance; ++i)
	{
		struct game_instance *curr = data->instance + i;
		int hours, seconds, minutes, status;
		pid_t r_pid;
		char s_status[14] = {};
		if(curr->status == Running)
		{
			seconds = curr_time.tv_sec - curr->starttime.tv_sec;
			minutes = seconds/60;
			seconds %= 60;
			hours = minutes / 60;
			minutes %= 60;
			r_pid = waitpid(curr->pid, &status, WNOHANG);
			if(r_pid == 0)
			{
				snprintf(s_status, 14, "Running"); 
			}
			else if(WIFEXITED(status))
			{
				int statuscode = WEXITSTATUS(status); 
				snprintf(s_status, 14, "Exited(%d)", statuscode);
				curr->starttime.tv_sec = curr_time.tv_sec - curr->starttime.tv_sec;
				curr->status = Exited;
				curr->exitcode = statuscode;
			}
			else if(WIFSIGNALED(status))
			{
				int statuscode = WTERMSIG(status); 
				snprintf(s_status, 14, "Terminated(%d)", statuscode);
				curr->starttime.tv_sec = curr_time.tv_sec - curr->starttime.tv_sec;
				curr->status = Terminated;
				curr->exitcode = statuscode;
			}
			else
			{
				snprintf(s_status, 14, "ERROR");
			}
		}
		else
		{
			seconds = curr->starttime.tv_sec;
			minutes = seconds/60;
			seconds %= 60;
			hours = minutes / 60;
			minutes %= 60;
			if(curr->status == Exited)
			{
				snprintf(s_status, 14, "Exited(%d)", curr->exitcode);
			}
			else
			{
				snprintf(s_status, 14, "Terminated(%d)", curr->exitcode);
			}
		}

		if(i == selected)
		{
			wattron(w, A_STANDOUT);
		}
		mvwprintw(w, 2*i+1, 1, "%-13s%5d:%02d:%02d   Mode: %-10sStatus: %-13s",curr->game, hours, minutes, seconds, curr->mode, s_status );
		if(i == selected)
		{
			wattroff(w, A_STANDOUT);
		}
	}
	if(selected == data->n_instance)
	{
		wattron(w, A_STANDOUT);
		mvwprintw(w, data->n_instance*2+1, 0, "Back");
		wattroff(w, A_STANDOUT);
	}
	else
	{
		mvwprintw(w, data->n_instance*2+1, 0, "Back");
	}
	
}

void
halted_instance
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected)
{
	char s_menu[2][7] = {"Untrack", "back"};
	enum e_menu{Untrack, Back} selected2 = Untrack;
	WINDOW *w2 = newwin(4, 9, 3+2*selected, 67);
	box(w2, 0, 0);
	for(int i = 0; i < 2; ++i)
	{
		if(i == selected2)
		{
			wattron(w2, A_STANDOUT);
			mvwprintw(w2, i+1, 1, "%.7s", s_menu[i]);
			wattroff(w2, A_STANDOUT);
		}
		else
		{
			mvwprintw(w2, i+1, 1, "%.7s", s_menu[i]);
		}
	}
	keypad( w2, TRUE );
	int c;
	int q = 0;
	while(!q)
	{
		c = wgetch(w2);
		if(c != ERR)
		{
			switch(c)
			{
				case KEY_UP		: selected2 ? --selected2 :0; break;
				case KEY_DOWN	: (2 - selected2 - 1) ? ++selected2:0;break;
				case KEY_LEFT	: selected2 = Back;
				case KEY_RIGHT	:	
				case '\n'		: //main enter key
				case KEY_ENTER	: q = 1; break;//enter on numeric block
			}
		}
		if((q == 1) && selected2 == Untrack)
		{
			for(int i = selected + 1; i < data->n_instance; ++i)
			{
				data->instance[i-1] = data->instance[i];
			}
			data->instance = reallocarray(data->instance, sizeof(*data->instance), data->n_instance - 1);
			data->n_instance -= 1;
			wmove(w, data->n_instance*2+1, 0);
			wclrtoeol(w);
			wmove(w, data->n_instance*2+2, 0);
			wclrtoeol(w);
			wmove(w, data->n_instance*2+3, 0);
			wclrtoeol(w);
			mvwprintw(w, data->n_instance*2, 0, "╚════════════════════════════════════════════════════════════════╝");
		}
		instance_update(data, w, selected);
		for(int i = 0; i < 2; ++i)
		{
			if(i == selected2)
			{
				wattron(w2, A_STANDOUT);
				mvwprintw(w2, i+1, 1, "%.7s", s_menu[i]);
				wattroff(w2, A_STANDOUT);
			}
			else
			{
				mvwprintw(w2, i+1, 1, "%.7s", s_menu[i]);
			}
		}
		
	}
	werase(w2);
	wrefresh(w2);
	delwin(w2);
}

void
running_instance
	(struct launcher_data *data,
	 WINDOW *w,
	 int selected)
{
	char s_menu[3][9] = {"Terminate", "Kill", "back"};
	enum e_menu{Terminate, Kill, Back} selected2 = Terminate;
	WINDOW *w2 = newwin(5, 11, 3+2*selected, 67);
	box(w2, 0, 0);
	for(int i = 0; i < 3; ++i)
	{
		if(i == selected2)
		{
			wattron(w2, A_STANDOUT);
			mvwprintw(w2, i+1, 1, "%.9s", s_menu[i]);
			wattroff(w2, A_STANDOUT);
		}
		else
		{
			mvwprintw(w2, i+1, 1, "%.9s", s_menu[i]);
		}
	}
	keypad( w2, TRUE );
	int c;
	int q = 0;
	while(!q)
	{
		c = wgetch(w2);
		if(c != ERR)
		{
			switch(c)
			{
				case KEY_UP		: selected2 ? --selected2 :0; break;
				case KEY_DOWN	: (3 - selected2 - 1) ? ++selected2:0;break;
				case KEY_LEFT	: selected2 = Back;
				case KEY_RIGHT	:	
				case '\n'		: //main enter key
				case KEY_ENTER	: q = 1; break;//enter on numeric block
			}
		}
		if((q == 1) && selected2 != Back)
		{
			switch(selected2)
			{
				case Terminate	: kill(data->instance[selected].pid, SIGTERM); break;
				case Kill		: kill(data->instance[selected].pid, SIGKILL); break;
				case Back		: break;
			}
		}
		instance_update(data, w, selected);
		for(int i = 0; i < 3; ++i)
		{
			if(i == selected2)
			{
				wattron(w2, A_STANDOUT);
				mvwprintw(w2, i+1, 1, "%.9s", s_menu[i]);
				wattroff(w2, A_STANDOUT);
			}
			else
			{
				mvwprintw(w2, i+1, 1, "%.9s", s_menu[i]);
			}
		}
		
	}
	werase(w2);
	wrefresh(w2);
	delwin(w2);
}

/*
╔════════════════════════════════════════════════════════════════╗
║Imperator		102:42:10	Mode: Default	Status: Running		 ║
╠════════════════════════════════════════════════════════════════╣
║Stellaris		 10:32:23	Mode: Debug		Status: Exit(2)		 ║
╠════════════════════════════════════════════════════════════════╣
║Ck2			  1:00:42	Mode: Develop	Status: Terminated(9)║
╠════════════════════════════════════════════════════════════════╣
║																 ║
╚════════════════════════════════════════════════════════════════╝
*/
