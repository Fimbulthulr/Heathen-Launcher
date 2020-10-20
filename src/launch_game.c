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

#include <spawn.h>


extern char **environ;

pid_t
launch_game
	(char **argv)
{
	pid_t pid;
	posix_spawn_file_actions_t actions;
	posix_spawn_file_actions_init(&actions);
#ifndef HEATHEN_GAME_ADDITIONAL_LOGGING
	posix_spawn_file_actions_addopen(&actions, 3, "/dev/null", O_WRONLY | O_APPEND, 0222);
	posix_spawn_file_actions_adddup2(&actions, 3, 1);
	posix_spawn_file_actions_adddup2(&actions, 3, 2);
#endif
	int status = posix_spawn(&pid, argv[0], &actions, NULL, argv, environ);
	
	if(status != 0)
	{
		perror("game launch failed");
	}
	return pid;
}
