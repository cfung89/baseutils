#include "argparse.h"
#include "arena.h"
#include "error.h"
#include "path.h"
#include <stdbool.h>
#include <string.h>

void argp_parse_bool(const void *inp, void *out);
void argp_parse_int(const void *inp, void *out);
void argp_parse_float(const void *inp, void *out);
void argp_parse_str(const void *inp, void *out);

void argp_parse_path(const void *inp, void *out) {
    Path path = path_create(inp);
    out = &path;
}

Error argp_parse_cmd(ArgCmd *cmd, int argc, char *argv[], void *out) {
    // check subcommands
    for (size_t i = 0; i < cmd->subcommands->length; ++i) {
        char *inp = argv[0];
        Result res = array_get(cmd->subcommands, i);
        ArgCmd *subcmd;
        TRY_UNPACK_ERR(subcmd, res);
        if (strcmp(inp, subcmd->name) == 0) {
            CHECK_ERR(argp_parse_cmd(subcmd, argc - 1, argv + 1, out));
            break;
        }
    }

    // check positionals
    for (size_t i = 0; i < cmd->positionals->length; ++i) {
        char *inp = argv[0];
        Result res = array_get(cmd->positionals, i);
        ArgPos *positionals;
        TRY_UNPACK_ERR(positionals, res);
        if (strcmp(inp, positionals->name) == 0) {
            // TODO: parse positionals
            break;
        }
    }

    // check flags
    for (size_t i = 0; i < cmd->positionals->length; ++i) {
        char *inp = argv[0];
        Result res = array_get(cmd->positionals, i);
        ArgPos *flags;
        TRY_UNPACK_ERR(flags, res);
        if (strcmp(inp, flags->name) == 0) {
            // TODO: parse flags
            break;
        }
    }

    return NO_ERROR;
}

typedef struct {
    bool verbose;
    const char *config_path;
    const char *active_subcommand;
    union {
        struct {
            const char *target;
            float optimization_level;
        } build;
        struct {
            const char *filter;
            bool fail_fast;
        } test;
    };
} AppConfig;

void argp_create() {
    Arena *arena = arena_alloc(1024, 1024);
    Array positionals = arena_create_array(arena, ArgPos, 1, true);
    Array flags = arena_create_array(arena, ArgFlag, 1, true);
    array_set(&flags, 0, &ARGP_FLAG("v", "verbose", "Debug outputs", ARGT_BOOL(), AppConfig, verbose));
    ArgCmd cmd = ARGP_CMD("baseutils", "", NULL, &positionals, &flags);
}
