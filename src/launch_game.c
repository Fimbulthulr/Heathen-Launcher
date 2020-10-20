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

#include <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>



pid_t
launch_game
	(char **argv)
{
	pid_t pid = fork();
	if(pid == 0) //execute game in the child proces
	{
#ifndef GAME_ADDITIONAL_LOGGING		//reroute stdout & stderr to /dev/null unless specified otherwise
		int fd = open("/dev/null", O_WRONLY | O_APPEND);
		dup2(fd, 1);
		dup2(fd, 2);
#endif
		if(execv(argv[0], argv) == -1)
		{
			perror("game launch failed");
			_exit(EXIT_FAILURE);
		}
	}
	return pid;
}
