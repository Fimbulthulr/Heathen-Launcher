#ifndef PLAYSET_MANAGEMENT_H
#define PLAYSET_MANAGEMENT_H

/*
 * =====================================================================================
 *
 *       Filename:  playset_management.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/12/20 05:01:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), gx231@stud.uni-heidelberg.de
 *   Organization:  Universität Heidelberg
 *
 * =====================================================================================
 */

#include "mod_defines.h"

struct playset *
load_playset
	(char *path,
	 struct mods_structure *available_mods);

void
write_playset
	(struct playset *playset,
	 char *path);

#endif /* PLAYSET_MANAGEMENT_H */
