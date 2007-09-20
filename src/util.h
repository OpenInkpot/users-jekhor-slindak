/*
 * vi: sw=4 ts=4 noexpandtab
 */

#ifndef __UTIL_H__
#define __UTIL_H__

char *parent_dir(char *path);

typedef void (*traverse_fn_t)(char *path, void *data);

int traverse(char *path, traverse_fn_t callback, void *data);

int spawn(char *cmd, char **argv);

int mkdir_p(char *dst, mode_t mode);

void root_squash();

#define xmalloc(__s) ({                    \
                void *__ret = malloc(__s); \
                if (!__ret)                \
                        return GE_ERROR;   \
                __ret;                     \
        })

#define xfree(__p) do {                      \
                void **__P = (void *)&(__p); \
                if (*__P)                    \
                        free(*__P);          \
                *__P = NULL;                 \
        } while (0)

#endif

