#ifndef BASEUTILS_STRINGVIEW_H
#define BASEUTILS_STRINGVIEW_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define SV_FMT "%.*s"
#define SV_ARG(s) (int)(s).length, (s).data
#define SV_ARGP(s) (int)(s)->length, (s)->data

typedef struct {
    const char *data;
    size_t length;
} StringView;

StringView sv_create(const char *str);
void sv_to_string(const StringView *self, char *output);
void sv_print(const StringView *self);
void sv_chop_left(StringView *self, size_t n);
void sv_chop_right(StringView *self, size_t n);
void sv_trim_left(StringView *self);
void sv_trim_right(StringView *self);
void sv_trim(StringView *self);
StringView sv_chop_by_delim(StringView *self, char delim);
StringView sv_chop_by_type(StringView *self, int (*istype)(int c));
void sv_split_by_delim(StringView *self, char delim);
void sv_split_by_type(StringView *self, int (*istype)(int c));
bool sv_contains_sv(const StringView *self, const StringView *str);
bool sv_contains_str(const StringView *self, const char *str);
bool sv_equals_sv(const StringView *self, const StringView *str);
bool sv_equals_str(const StringView *self, const char *str);

#endif
