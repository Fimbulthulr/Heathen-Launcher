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
#define EU4_PATH "Europa Universalis IV"
#define HOI4_PATH "Hearts of Iron IV"
#define STELLARIS_PATH "Stellaris"
#define IMPERATOR_PATH "Imperator"

#include <stddef.h>
#include <sys/types.h>
#include <time.h>

#define HAS_CK3
#define HAS_IMPERATOR
#define HAS_EU4
#define HAS_STELLARIS
#define HAS_HOI4

enum selected_game {gCk3, gEu4, gStellaris, gImperator, gHoi4};


struct game_instance
{
	char *game;
	struct timespec starttime;
	pid_t pid;
	enum statuscode {Running = 0, Exited, Terminated} status;
	int exitcode;
	char *mode;
};


struct launcher_data
{
	size_t n_instance;
	struct game_instance *instance;
	int lines;
	int rows;
	enum states {Quit, Main, Instance, Check, Mod} state;
};


//sum available games
#ifdef HAS_CK3
#define CK3_VAL 1
#else
#define CK3_VAL 0
#endif

#ifdef HAS_EU4
#define EU4_VAL 1
#else
#define EU4_VAL 0
#endif

#ifdef HAS_HOI4
#define HOI4_VAL 1
#else
#define HOI4_VAL 0
#endif

#ifdef HAS_IMPERATOR
#define IMP_VAL 1
#else
#define IMP_VAL 0
#endif

#ifdef HAS_STELLARIS
#define STEL_VAL 1
#else
#define STEL_VAL 0
#endif

#define N_GAMES CK3_VAL + EU4_VAL + HOI4_VAL + IMP_VAL + STEL_VAL
#endif /* DEFINES_H */
