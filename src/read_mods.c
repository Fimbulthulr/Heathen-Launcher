/*
 * =====================================================================================
 *
 *       Filename:  read_mods.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/20 21:26:53
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Freyja Walberg (), gx231@stud.uni-heidelberg.de
 *   Organization:  Universität Heidelberg
 *
 * =====================================================================================
 */

#include "read_mods.h"

#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MODS_BUFFER 100

static void 
read_mod
	(char *path,
	 struct mod_file *mod);

void
read_mods
	(char *path,
	 struct mods_structure *mods)
{
	mods->mods_buf = MODS_BUFFER;
	mods->mods = calloc(sizeof(*mods->mods), mods->mods_buf);
	mods->n_mods = 0;
	DIR *directory;
	struct dirent *entry;

	directory = opendir(path);
	if( directory == NULL)
	{
		perror("Reading directory failed");
		return;
	}
	while( (entry = readdir(directory)) )
	{
		char *file_ending = strrchr(entry->d_name, '.');
		if( strcmp(file_ending, ".mod") == 0)
		{
			if( mods->n_mods == mods->mods_buf)
			{
				mods->mods_buf *= 2;
				mods->mods = reallocarray(mods->mods, sizeof(*mods->mods), mods->mods_buf);
			}
			struct mod_file *curr = mods->mods + mods->n_mods;
			curr->filename_length = strlen(entry->d_name) + 1;
			curr->filename = calloc(sizeof(*curr->filename), curr->filename_length);
			strncpy(curr->filename, entry->d_name, curr->filename_length);
			read_mod(path, curr);
			mods->n_mods += 1;
		}
	}
	mods->mods = reallocarray(mods->mods, sizeof(*mods->mods), mods->n_mods);
}




static void
read_mod
	(char *path,
	 struct mod_file *mod)
{
	size_t path_len = strlen(path);
	char* path_total = calloc(sizeof(*path_total), path_len + mod->filename_length + 1);
	strcpy(path_total, path);
	if( path_total[path_len - 1] != '/')
	{
		path_total[path_len + 1] = '\0';
		path_total[path_len] = '/';
	}
	strcat(path_total, mod->filename);
	int fd = open(path_total, O_RDONLY);
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
			fprintf(stderr, "Read on file %s failed\n", mod->filename);
			exit(1);
		}
		n_read += tmp;
	}
	close(fd);
	char *start, *end;
	


	start = strstr(file, "name");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	mod->name_length = end - start;
	mod->name = calloc(sizeof(*mod->name), mod->name_length);
	memcpy(mod->name, start + 1, mod->name_length - 1);
	mod->name[mod->name_length-1] = '\0';
	
	start = strstr(file, "supported_version");
	start = strchr(start, '"');
	end = strchr(start + 1, '"');
	mod->version_length = end - start;
	mod->version = calloc(sizeof(*mod->version), mod->version_length);
	memcpy(mod->version, start + 1, mod->version_length - 1);
	mod->version[mod->version_length-1] = '\0';



	free(file);	
	free(path_total);
}
