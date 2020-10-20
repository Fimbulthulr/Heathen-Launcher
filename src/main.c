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

#include "launch_game.h"


#include	<stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>

#define PDX_PATH ".local/share/Paradox Interactive"
#define CK3_PATH "Crusader Kings III"

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
	err = chdir(CK3_PATH);
	if(err == -1)
	{
		perror("could not switch to CKIII directory\n");
		exit(EXIT_FAILURE);
	}




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

	}
	//*/
}
