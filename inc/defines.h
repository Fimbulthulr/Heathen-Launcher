#ifndef DEFINES_H
#define DEFINES_H

/*
 * =====================================================================================
 *
 *       Filename:  defines.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/10/20 11:48:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#define PDX_PATH ".local/share/Paradox Interactive"
#define CK3_PATH "Crusader Kings III"

#include <stddef.h>
#include <time.h>
#include <sys/types.h>

enum selected_game{gCk3};

struct game_instance
{
	char* game;
	struct timespec starttime;
	pid_t pid;
};

struct launcher_data
{
	size_t n_instance;
	struct game_instance *instance;
	int lines;
	int rows;
	enum states{Quit, Main, Instance} state;
};

#endif /* DEFINES_H */
