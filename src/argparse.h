#ifndef BASEUTILS_ARGPARSE_H
#define BASEUTILS_ARGPARSE_H

#include "array.h"
#include "path.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int num_args;
    void (*convert)(const void *inp, void *out);
} ArgType;

typedef struct {
    const char *name;
    const char *help;

    Array *subcommands; // Array of ArgParser elements (subcommands)
    Array *positionals; // Array of PositionalArg elements
    Array *flags;       // Array of Flag elements
} ArgCmd;

typedef struct {
    const char *name;
    const char *help;
    ArgType type;
    size_t offset;
} ArgPos; // Positional argument

typedef struct {
    const char *short_name;
    const char *long_name;
    const char *help;
    ArgType type;
    size_t offset;
} ArgFlag;

#define ARGP_CMD(name, help, subcommands, positionals, flags)                  \
    (ArgCmd) { name, help, subcommands, positionals, flags }

#define ARGP_POS(_name, _help, _type, struct_type, member)                     \
    (ArgPos) { name, help, type, offsetof(struct_type, member) }

#define ARGP_FLAG(short_name, long_name, help, type, struct_type, member)      \
    (ArgFlag) {                                                                \
        short_name, long_name, help, type, offsetof(struct_type, member)       \
    }

void argp_parse_bool(const void *inp, void *out);
void argp_parse_int(const void *inp, void *out);
void argp_parse_float(const void *inp, void *out);
void argp_parse_str(const void *inp, void *out);
void argp_parse_path(const void *inp, void *out);

#define ARGT_BOOL()                                                            \
    (ArgType) { .num_args = 0, .convert = &argp_parse_bool }
#define ARGT_INT(_num_args)                                                    \
    (ArgType) { .num_args = _num_args, .convert = &argp_parse_int }
#define ARGT_FLOAT(_num_args)                                                  \
    (ArgType) { .num_args = _num_args, .convert = &argp_parse_float }
#define ARGT_STR(_num_args)                                                    \
    (ArgType) { .num_args = _num_args, .convert = &argp_parse_str }
#define ARGT_PATH(_num_args)                                                   \
    (ArgType) { .num_args = _num_args, .convert = &argp_parse_path }

Error argp_parse_cmd(ArgCmd *cmd, int argc, char *argv[], void *out);

#endif
