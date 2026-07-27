#include "stringview.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Creates a new StringView.
StringView sv_create(const char *str) {
    return (StringView){
        .data = str,
        .length = strlen(str),
    };
}

// Converts a StringView to a standard C string.
void sv_to_string(const StringView *self, char *output) {
    memcpy(output, self->data, self->length);
    output[self->length] = '\0';
}

// Utility to print a StringView.
void sv_print(const StringView *self) { printf(SV_FMT "\n", SV_ARGP(self)); }

// Removes `n` characters from the left side of StringView.
void sv_chop_left(StringView *self, size_t n) {
    if (n > self->length) {
        n = self->length;
    }
    self->data += n;
    self->length -= n;
}

// Removes `n` characters from the right side of StringView.
void sv_chop_right(StringView *self, size_t n) {
    if (n > self->length) {
        n = self->length;
    }
    self->length -= n;
}

// Removes all leading whitespace from the StringView.
void sv_trim_left(StringView *self) {
    while (self->length > 0 && isspace(self->data[0])) {
        sv_chop_left(self, 1);
    }
}

// Removes all trailing whitespace from the StringView.
void sv_trim_right(StringView *self) {
    while (self->length > 0 && isspace(self->data[self->length - 1])) {
        sv_chop_right(self, 1);
    }
}

// Removes all leading and trailing whitespace from the StringView.
void sv_trim(StringView *self) {
    sv_trim_left(self);
    sv_trim_right(self);
}

// Splits the StringView at the first occurrence of the delimiter. Returns the
// section before the delimiter, and updates the original StringView to point to
// the remaining tail.
StringView sv_chop_by_delim(StringView *self, char delim) {
    size_t i = 0;
    while (i < self->length && self->data[i] != delim) {
        i += 1;
    }
    if (i >= self->length) {
        return *self;
    }
    StringView result = {
        .data = self->data,
        .length = i,
    };
    sv_chop_left(self, i + 1);
    return result;
}

// Splits the StringView at the first character that satisfies the given
// condition. Returns the prefix before that character, and updates the original
// StringView to point to the remaining tail.
StringView sv_chop_by_type(StringView *self, int (*istype)(int c)) {
    size_t i = 0;
    while (i < self->length && !istype(self->data[i])) {
        i += 1;
    }
    if (i >= self->length) {
        return *self;
    }
    StringView result = {
        .data = self->data,
        .length = i,
    };
    sv_chop_left(self, i + 1);
    return result;
}

bool sv_contains_sv(const StringView *self, const StringView *str);
bool sv_contains_str(const StringView *self, const char *str);

// Checks for equality of two StringViews.
bool sv_equals_sv(const StringView *self, const StringView *str) {
    if (self->length != str->length) {
        return false;
    }
    for (size_t i = 0; i < self->length; ++i) {
        if (self->data[i] != str->data[i]) {
            return false;
        }
    }
    return true;
}

// Checks for StringView and standard C string equality. Assumes that the C
// string is correctly null-terminated.
bool sv_equals_str(const StringView *self, const char *str) {
    if (self->length != strlen(str)) {
        return false;
    }
    for (size_t i = 0; i < self->length; ++i) {
        if (self->data[i] != str[i]) {
            return false;
        }
    }
    return true;
}
