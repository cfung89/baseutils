#ifndef BASEUTILS_ERROR_H
#define BASEUTILS_ERROR_H

typedef enum {
    ERR_NONE = 0,
    ERR_UNKNOWN,
    ERR_NOT_IMPLEMENTED,
    ERR_OUT_OF_BOUNDS,
    ERR_NULL,
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

#define ERROR(type, msg, exit_code)                                            \
    (Error) { type, msg, exit_code }

#define NO_ERROR ERROR(ERR_NONE, NULL, 0)
#define SUCCESS(_data)                                                         \
    (Result) { RESULT_SUCCESS, .data = _data }

#define FAILURE(type, msg, exit_code)                                          \
    (Result) { RESULT_FAILURE, .error = ERROR(type, msg, exit_code) }

#define TRY(result)                                                            \
    do {                                                                       \
        Result res = (result);                                                 \
        if (res.status == RESULT_FAILURE) {                                    \
            return res;                                                        \
        }                                                                      \
    } while (0)

#define TRY_UNPACK(_data, _result)                                             \
    do {                                                                       \
        Result res = (_result);                                                \
        if (res.status == RESULT_FAILURE) {                                    \
            return res;                                                        \
        }                                                                      \
        _data = res.data;                                                      \
    } while (0)

#define TRY_UNPACK_ERR(_data, _result)                                         \
    do {                                                                       \
        Result res = (_result);                                                \
        if (res.status == RESULT_FAILURE) {                                    \
            return res.error;                                                  \
        }                                                                      \
        _data = res.data;                                                      \
    } while (0)

#define CHECK_ERR(error)                                                       \
    do {                                                                       \
        Error err = (error);                                                   \
        if (err.type != ERR_NONE) {                                            \
            return err;                                                        \
        }                                                                      \
    } while (0)

#endif
