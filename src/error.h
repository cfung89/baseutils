#ifndef BASEUTILS_ERROR_H
#define BASEUTILS_ERROR_H

typedef enum {
    ERR_NONE = 0,
} ErrorType;

typedef struct {
    int type; // enum
    const char *msg;
    int exit_code;
} Error;

typedef struct {
    enum { RESULT_SUCCESS, RESULT_FAILURE } status;
    union {
        void *data;
        Error error;
    };
} Result;

#define SUCCESS(data)                                                          \
    (Result) { .status = RESULT_SUCCESS, .data = data }

#define FAILURE(error)                                                         \
    (Result) { .status = RESULT_FAILURE, .error = error }

#define TRY(result)                                                            \
    do {                                                                       \
        Result res = (result);                                                 \
        if (res.status == RESULT_FAILURE) {                                    \
            return res;                                                        \
        }                                                                      \
    } while (0)

#define TRY_UNPACK(data, result)                                               \
    do {                                                                       \
        Result res = (result);                                                 \
        if (res.status == RESULT_FAILURE) {                                    \
            return res;                                                        \
        }                                                                      \
        data = res.data;                                                       \
    } while (0)

#endif
