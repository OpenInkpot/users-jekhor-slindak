/*
 * vi: sw=4 ts=4 noexpandtab
 */
#ifndef __LUA_H__
#define __LUA_H__

char *L_call(char *fn, int argc, ...);

int L_call_aptconf(void);

int L_load_aptconf(void);

char *L_get_string(char *name, int table);

char *L_get_confstr(char *field, char *table);

int L_dofile(const char *path);

int L_init(void);

void L_done(void);

#endif

