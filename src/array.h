#ifndef BASEUTILS_ARRAY_H
#define BASEUTILS_ARRAY_H

#include "error.h"
#include <stddef.h>

typedef struct {
    void *data;
    size_t length;
    size_t element_size;
} Array;

#define array_create(data, length, element_size)                               \
    (Array) { data, length, element_size }

Result array_get(const Array *self, size_t index);
Error array_set(Array *self, size_t index, void *data);

#endif
