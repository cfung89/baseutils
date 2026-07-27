#include "array.h"
#include "error.h"
#include <string.h>

Result array_get(const Array *self, size_t index) {
    if (self == NULL || self->data == NULL) {
        return FAILURE(ERR_NULL, "Error: Array or data is null.", 1);
    }

    if (index >= self->length) {
        return FAILURE(ERR_OUT_OF_BOUNDS,
                       "Error: Cannot index array out of bounds.", 1);
    }

    void *target = (char *)self->data + (index * self->element_size);
    return SUCCESS(target);
}

Error array_set(Array *self, size_t index, void *data) {
    if (self == NULL || self->data == NULL) {
        return ERROR(ERR_NULL, "Error: Array or data is null.", 1);
    }

    if (index >= self->length) {
        return ERROR(ERR_OUT_OF_BOUNDS,
                     "Error: Cannot index array out of bounds.", 1);
    }
    void *target = (char *)self->data + (index * self->element_size);
    memcpy(target, data, self->element_size);
    return NO_ERROR;
}
