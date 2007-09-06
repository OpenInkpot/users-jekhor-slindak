/*
 * vi: sw=4 ts=4 noexpandtab
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "common.h"
#include "util.h"

/*
 * Given a path, returns a parent directory,
 * that is allocated (needs free'ing).
 */
char *parent_dir(char *path)
{
	char *s1, *s2 = NULL;
	char *res = NULL;

	s1 = strrchr(path, '/');
	if (s1) {
		*s1 = '\0';

		s2 = strrchr(path, '/');
		if (s2++)
			res = strdup(s2);

		*s1 = '/';
	}

	return res;
}

int traverse(char *path, traverse_fn_t callback, void *data)
{
	DIR *dir;
	struct dirent *de;
	struct stat st;
	char *newpath;
	int s;

	dir = opendir(path);
	GE_ERROR_IFNULL(dir);

	do {
		de = readdir(dir);
		if (!de)
			break;

		if ( 
			de->d_name[0] == '.' &&
			((de->d_name[1] == '.' && de->d_name[2] == '\0') ||
			(de->d_name[1] == '\0'))
		   )
			continue;

		asprintf(&newpath, "%s/%s", path, de->d_name);

		s = stat(newpath, &st);
		if (s) {
			free(newpath);
			continue;
		}

		if (S_ISDIR(st.st_mode))
			traverse(newpath, callback, data);
		else
			callback(newpath, data);

		free(newpath);
	} while (de);
	closedir(dir);

	return GE_OK;
}
