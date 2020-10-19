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


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	char* gameargs = "\"/fast_access/Steam/steamapps/common/Crusader Kings III/binaries/ck3\" >> /dev/null 2>&1";

	launch_game(gameargs);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
