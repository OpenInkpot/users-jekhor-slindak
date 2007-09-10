/*
 * vi: sw=4 ts=4 noexpandtab
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "common.h"
#include "debfile.h"

int deb_ver_gt(char *v1, char *v2)
{
	char *argv[] = { "dpkg", "--compare-versions", v1, "gt", v2, NULL };
	int ret;

	ret = spawn(DPKG_BIN_PATH, argv);
	if (ret)
		return GE_ERROR;

	return GE_OK;
}

int debfile_read(char *path, struct debfile *df)
{
	FILE *p;
	char cmd[PATH_MAX];
	char tok[256];

	snprintf(cmd, PATH_MAX, "/usr/bin/dpkg --info %s", path);
	p = popen(cmd, "r");
	GE_ERROR_IFNULL(p);

	memset(df, 0, sizeof(struct debfile));

	while (!feof(p)) {
		fscanf(p, "%s", tok);
		
		if (!strcmp(tok, "Package:")) {
			fscanf(p, "%s", tok);

			strncpy(df->debname, tok, DF_NAMELEN);
		} else if (!strcmp(tok, "Version:")) {
			fscanf(p, "%s", tok);

			strncpy(df->version, tok, DF_VERLEN);
		} else if (!strcmp(tok, "Architecture:")) {
			fscanf(p, "%s", tok);

			strncpy(df->arch, tok, DF_ARCHLEN);
		} else if (!strcmp(tok, "Section:")) {
			fscanf(p, "%s", tok);

			strncpy(df->component, tok, DF_COMPLEN);
		} else if (!strcmp(tok, "Source:")) {
			fscanf(p, "%s", tok);

			strncpy(df->source, tok, DF_SRCLEN);
		}
	}

	pclose(p);

	if (df->source[0] == '\0')
		strncpy(df->source, df->debname, DF_SRCLEN);

	return GE_OK;
}

int dscfile_read(char *path, struct dscfile *df)
{
	FILE *f;
	char tok[256];

	f = fopen(path, "r");
	GE_ERROR_IFNULL(f);

	memset(df, 0, sizeof(struct dscfile));

	while (!feof(f)) {
		fscanf(f, "%s", tok);
		
		if (!strcmp(tok, "Package:")) {
			fscanf(f, "%s", tok);

			strncpy(df->pkgname, tok, DF_SRCLEN);
		} else if (!strcmp(tok, "Version:")) {
			fscanf(f, "%s", tok);

			strncpy(df->version, tok, DF_VERLEN);
		} else if (!strcmp(tok, "Architecture:")) {
			fscanf(f, "%s", tok);

			if (!strcmp(tok, "all") || !strcmp(tok, "any"))
				df->arch[0] = '\0';
			else
				strncpy(df->arch, tok, DF_ARCHLEN);
		} else if (!strcmp(tok, "Section:")) {
			fscanf(f, "%s", tok);

			strncpy(df->component, tok, DF_COMPLEN);
		} else if (!strcmp(tok, "Source:")) {
			fscanf(f, "%s", tok);

			strncpy(df->pkgname, tok, DF_SRCLEN);
		}
	}

	fclose(f);

	return GE_OK;
}

