/*
 * =====================================================================================
 *
 *       Filename:  playset_management.c
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

#include <cjson/cJSON.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "playset_management.h"

#define MAX_FILENAME 256

struct playset *
load_playset
	(char *path,
	 struct mods_structure *available_mods)
{
	int fd = open(path, O_RDONLY);
	struct stat statbuf;
	fstat(fd, &statbuf);
	ssize_t filesize = statbuf.st_size;
	char *file = calloc(sizeof(*file), filesize);
	ssize_t n_read = 0;
	while(n_read < filesize)
	{
		ssize_t tmp = read(fd, file + n_read, filesize - n_read);
		if(tmp == -1)
		{
			fprintf(stderr, "Read on file %s failed\n", path);
			exit(1);
		}
		n_read += tmp;
	}
	close(fd);
	cJSON *json = cJSON_ParseWithLength(file, filesize);
	cJSON *mod_array;
	for(mod_array = json->child; strcmp(mod_array->string, "enabled_mods") != 0 && mod_array != NULL; mod_array = mod_array->next);
	if(mod_array == NULL)
	{
		fprintf(stderr, "ERROR: %s not a valid playset file\n", path);
	}
	size_t n_mods = cJSON_GetArraySize(mod_array);
	struct playset *playset = malloc(sizeof(*playset));
	playset->n_mods = n_mods;
	playset->mods = calloc(sizeof(*playset->mods), n_mods);
	playset->enabled = calloc(sizeof(*playset->enabled), n_mods/sizeof(*playset->enabled)/8 + 1);
	cJSON *curr_mod = mod_array->child;
	for(size_t i = 0; i < n_mods; ++i, curr_mod = curr_mod->next)
	{
		char modname[MAX_FILENAME + 13];
		memset(modname, 0, MAX_FILENAME + 13);
		strcpy(modname, curr_mod->valuestring);
		char* ending = strrchr(modname, '.');
		if(strcmp(ending, ".disabled") == 0)
		{
			*ending = '\0';
			playset->enabled[i/sizeof(*playset->enabled)] &= ~(1 << (i % sizeof(*playset->enabled)) );
			playset->mods[i] = NULL;
			char *no_path = strrchr(modname, '/') + 1;
			for(size_t j = 0; j < available_mods->n_mods; ++j)
			{
				if(strcmp(no_path, available_mods->mods[j].filename) == 0)
				{
					playset->mods[i] = available_mods->mods+j;
					break;
				}
			}
		}
		else
		{
			playset->enabled[i/sizeof(*playset->enabled)] |= (1 << (i % sizeof(*playset->enabled)) );
			playset->mods[i] = NULL;
			char *no_path = strrchr(modname, '/') + 1;
			for(size_t j = 0; j < available_mods->n_mods; ++j)
			{
				if(strcmp(no_path, available_mods->mods[j].filename) == 0)
				{
					playset->mods[i] = available_mods->mods+j;
					break;
				}
			}

		}
	}
	cJSON_Delete(json);
	free(file);
	return playset;	
}

void
write_playset
	(struct playset *playset,
	 char *path)
{
	cJSON *array = cJSON_CreateArray();
	char modname[MAX_FILENAME+9];
	for(size_t i = 0; i < playset->n_mods; ++i)
	{
		memset(modname, 0, MAX_FILENAME + 9);
		strcpy(modname, playset->mods[i]->filename);
		if( !((playset->enabled[i/sizeof(*playset->enabled)]) &( 1 << (i % sizeof(*playset->enabled))) ) )
		{
			strcat(modname, ".disabled");
		}
		cJSON_AddItemToArray(array, cJSON_CreateString(modname));
	}
	cJSON *object = cJSON_CreateObject();
	cJSON_AddItemToObjectCS(object, "enabled_mods", array);
	cJSON_AddItemToObjectCS(object, "disabled_dlcs", cJSON_CreateArray());
	char* json_string = cJSON_Print(object);
	cJSON_Delete(object);
	int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT);
	ssize_t filesize = strlen(json_string);
	ssize_t written = 0;
	while(written < filesize)
	{
		ssize_t tmp = write(fd, json_string + written, filesize - written);
		if(tmp != -1)
		{
			written += tmp;
		}
	}

	close(fd);
	free(json_string);
}
