#ifndef BASEUTILS_PATH_H
#define BASEUTILS_PATH_H

typedef struct {
} Path;

Path path_create(const char *str);
Path path_get_absolute(const Path *path);
Path path_get_relative(const Path *path, const Path *other);
Path path_join(const Path *path, int n, ...); // variadic function

#endif
