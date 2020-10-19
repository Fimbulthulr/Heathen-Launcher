/*
 * =====================================================================================
 *
 *       Filename:  launch_game.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  19/10/20 20:33:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include"launch_game.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


pid_t
launch_game
	(char *argv)
{
	pid_t pid = fork();
	if(pid == 0)
	{
		if(system(argv) == -1)
		{
			perror("game launch failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			exit(EXIT_SUCCESS);
		}
	}
	return pid;
}
