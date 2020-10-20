/*
 * =====================================================================================
 *
 *       Filename:  state_switcher.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  20/10/20 13:44:54
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "state_switcher.h"
#include "main_menu.h"
#include "instance_menu.h"
#include "instance_check.h"

void
state_switcher
	(struct launcher_data *data)
{
	while(data->state != Quit)
	{
		switch(data->state)
		{
			case Main		: main_menu(data);		break;
			case Instance	: instance_menu(data);	break;
			case Check		: instance_check(data); break;
			case Quit		: data->state = Quit;	break;
		}
	}
}
