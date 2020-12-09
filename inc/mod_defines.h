#ifndef MOD_DEFINES_H
#define MOD_DEFINES_H

/*
 * =====================================================================================
 *
 *       Filename:  mod_defines.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/20 21:35:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), gx231@stud.uni-heidelberg.de
 *   Organization:  Universität Heidelberg
 *
 * =====================================================================================
 */

#include <stddef.h>
#include <inttypes.h>

struct mod_file
{
	size_t filename_length;
	char *filename;
	size_t name_length;
	char *name;
	size_t version_length;
	char *version;
};


struct mods_structure {
	size_t n_mods;
	size_t mods_buf;
	struct mod_file *mods;
};

struct playset
{
	size_t n_mods;
	struct mod_file **mods;
	uint8_t *enabled;
};

#endif /* MOD_DEFINES_H */
