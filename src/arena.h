#ifndef BASEUTILS_ARENA_H
#define BASEUTILS_ARENA_H

#include <stdint.h>
#include "array.h"

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;
typedef int32_t b32;

#define ARENA_ALIGN (sizeof(void *))
#define IS_POW2(n) (n > 0 && ((n & (n - 1)) == 0))
#define ALIGN_UP_POW2(n, p) (((u64)(n) + ((u64)(p) - 1)) & (~((u64)(p) - 1)))

typedef struct ArenaRegion ArenaRegion;

struct ArenaRegion {
    u64 commit_pos;
    u64 pos;

    ArenaRegion *next;
};

typedef struct {
    ArenaRegion *region;
    u64 pos;
} ArenaMarker;

typedef struct {
    ArenaRegion *head;
    ArenaRegion *current;

    u64 reserve_size;
    u64 commit_size;
    u32 page_size;
} Arena;

Arena *arena_alloc(u64 reserve_size, u64 commit_size);
void arena_free(Arena *arena);
void *arena_push(Arena *arena, u64 size, b32 set_zero);
void arena_clear(Arena *arena);

ArenaMarker arena_snapshot(Arena *arena);
void arena_pop(Arena *arena, ArenaMarker marker);

#define arena_push_struct(arena, type, set_zero)                               \
    (type *)arena_push(arena, sizeof(type), set_zero)

#define arena_push_array(arena, type, length, set_zero)                         \
    (type *)arena_push(arena, sizeof(type) * (length), set_zero)

#define arena_create_array(arena, type, length, set_zero)                                 \
    array_create(arena_push_array(arena, type, length, set_zero), length, sizeof(type))

#endif
