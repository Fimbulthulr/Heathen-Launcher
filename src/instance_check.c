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

#include <curses.h> 
#include <time.h>
#include <sys/wait.h>

void 
instance_update
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
	/*
	struct timespec curr_time;
	clock_gettime(CLOCK_REALTIME, &curr_time);
	for(int i = 0; i < data->n_instance; ++i)
	{
		struct game_instance *curr = data->instance + i;
		int hours, seconds, minutes, status;
		pid_t r_pid;
		char s_status[14] = {};
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
			snprintf(s_status, 14, "Exited(%d)", WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			snprintf(s_status, 14, "Terminated(%d)", WTERMSIG(status));
		}
		else
		{
			snprintf(s_status, 14, "ERROR");
		}

		if(i == selected)
		{
			wattron(w, A_STANDOUT);
		}
		mvwprintw(w, 2*i+1, 1, "%-13s%5d:%02d:%02d   Mode: %-10sStatus: %s",curr->game, hours, minutes, seconds, curr->mode, s_status );
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
	*/
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
				case KEY_RIGHT	:	
				case '\n'		: //main enter key
				case KEY_ENTER	: q = 1; break;//enter on numeric block
			}
		}
		
		instance_update(data, w, selected);
		/*
		clock_gettime(CLOCK_REALTIME, &curr_time);
		for(int i = 0; i < data->n_instance; ++i)
		{
			struct game_instance *curr = data->instance + i;
			int hours, seconds, minutes, status;
			pid_t r_pid;
			char s_status[14] = {};
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
				snprintf(s_status, 14, "Exited(%d)", WEXITSTATUS(status));
			}
			else if(WIFSIGNALED(status))
			{
				snprintf(s_status, 14, "Terminated(%d)", WTERMSIG(status));
			}
			else
			{
				snprintf(s_status, 14, "ERROR");
			}

			if(i == selected)
			{
				wattron(w, A_STANDOUT);
			}
			mvwprintw(w, 2*i+1, 1, "%-13s%5d:%02d:%02d   Mode: %-10sStatus: %s",curr->game, hours, minutes, seconds, curr->mode, s_status );
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
		*/
		if(q == 1)
		{
			if(selected < data->n_instance)
			{
				//instance controll handling
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
			snprintf(s_status, 14, "Exited(%d)", WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status))
		{
			snprintf(s_status, 14, "Terminated(%d)", WTERMSIG(status));
		}
		else
		{
			snprintf(s_status, 14, "ERROR");
		}

		if(i == selected)
		{
			wattron(w, A_STANDOUT);
		}
		mvwprintw(w, 2*i+1, 1, "%-13s%5d:%02d:%02d   Mode: %-10sStatus: %s",curr->game, hours, minutes, seconds, curr->mode, s_status );
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
