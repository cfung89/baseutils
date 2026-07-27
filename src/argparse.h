#ifndef BASEUTILS_ARGPARSE_H
#define BASEUTILS_ARGPARSE_H

typedef enum { UNKNOWN, BOOL, INT, FLOAT, CHAR, STRING } ArgType;

typedef struct {
    void *(*action)();
} Argument;

#endif
