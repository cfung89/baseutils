#include "arena.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void *mem_reserve(u64 size);
b32 mem_commit(void *ptr, u64 size);
b32 mem_free(void *ptr, u64 size);

Arena *arena_alloc(u64 reserve_size, u64 commit_size) {
    assert(commit_size <= reserve_size);
    assert(IS_POW2(reserve_size) && IS_POW2(commit_size));
    u64 header_size = sizeof(Arena) + sizeof(ArenaRegion);
    u32 page_size = (u32)sysconf(_SC_PAGESIZE);
    u64 c_size = ALIGN_UP_POW2(commit_size, page_size);
    u64 r_size = ALIGN_UP_POW2(reserve_size, page_size);
    commit_size = ALIGN_UP_POW2(header_size, page_size);
    reserve_size = ALIGN_UP_POW2(reserve_size + header_size, page_size);

    void *ptr = mem_reserve(reserve_size);
    assert(ptr != NULL);
    assert(mem_commit(ptr, commit_size));

    Arena *arena = (Arena *)ptr;
    ArenaRegion *region = (ArenaRegion *)((u8 *)ptr + sizeof(Arena));

    region->commit_pos = commit_size - sizeof(Arena);
    region->pos = sizeof(ArenaRegion);
    region->next = NULL;

    arena->head = region;
    arena->current = region;
    arena->reserve_size = r_size;
    arena->commit_size = c_size;
    arena->page_size = page_size;

    return arena;
}

void arena_free(Arena *arena) {
    assert(arena != NULL);
    ArenaRegion *head = arena->head;
    ArenaRegion *current = arena->head;

    u64 page_size = arena->page_size;
    u64 reserve_size = arena->reserve_size;
    u64 head_size = ALIGN_UP_POW2(
            reserve_size + sizeof(Arena) + sizeof(ArenaRegion), page_size);
    u64 r_size = ALIGN_UP_POW2(reserve_size + sizeof(ArenaRegion), page_size);

    while (current != NULL) {
        ArenaRegion *next = current->next;
        if (current == head) {
            assert(mem_free(arena, head_size));
        } else {
            assert(mem_free(current, r_size));
        }
        current = next;
    }
}

void *arena_push(Arena *arena, u64 size, b32 set_zero) {
    assert(arena != NULL);
    assert(size <= arena->commit_size);
    assert(arena->head != NULL && arena->current != NULL);

    ArenaRegion *region = arena->current;
    size = ALIGN_UP_POW2(size, ARENA_ALIGN);
    u64 pos_aligned = ALIGN_UP_POW2(region->pos, ARENA_ALIGN);

    if (pos_aligned + size > arena->reserve_size) {
        u64 header_size = sizeof(ArenaRegion);
        u64 commit_size = ALIGN_UP_POW2(header_size, arena->page_size);
        u64 reserve_size =
            ALIGN_UP_POW2(arena->reserve_size + header_size, arena->page_size);

        void *ptr = mem_reserve(reserve_size);
        assert(ptr != NULL);
        assert(mem_commit(ptr, commit_size));

        ArenaRegion *new_region = (ArenaRegion *)ptr;
        new_region->commit_pos = commit_size;
        new_region->pos = sizeof(ArenaRegion);
        new_region->next = NULL;

        region->next = new_region;
        arena->current = new_region;

        region = new_region;
        pos_aligned = ALIGN_UP_POW2(region->pos, ARENA_ALIGN);
    }

    if (pos_aligned + size > region->commit_pos) {
        u64 old_commit_pos = region->commit_pos;
        u64 new_commit_pos =
            ALIGN_UP_POW2(pos_aligned + size, arena->commit_size);
        if (new_commit_pos > arena->reserve_size) {
            new_commit_pos = arena->reserve_size;
        }
        u64 len = new_commit_pos - old_commit_pos;
        if (len > 0) {
            assert(mem_commit((u8 *)region + old_commit_pos, len));
            region->commit_pos = new_commit_pos;
        }
    }

    u8 *ptr = (u8 *)region + pos_aligned;
    region->pos = pos_aligned + size;

    if (set_zero) {
        memset(ptr, 0, size);
    }
    return ptr;
}

void arena_clear(Arena *arena) {
    for (ArenaRegion *r = arena->head; r != NULL; r = r->next) {
        r->pos = sizeof(ArenaRegion);
    }
    arena->current = arena->head;
}

ArenaMarker arena_snapshot(Arena *arena) {
    assert(arena != NULL);
    ArenaMarker marker;
    if (arena->current == NULL) {
        assert(arena->head == NULL);
        marker.pos = 0;
    } else {
        marker.pos = arena->current->pos;
    }
    marker.region = arena->current;
    return marker;
}

void arena_pop(Arena *arena, ArenaMarker marker) {
    assert(arena != NULL);
    if (marker.region == NULL) {
        arena_clear(arena);
        return;
    }
    arena->current = marker.region;
    arena->current->pos = ALIGN_UP_POW2(marker.pos, ARENA_ALIGN);
    for (ArenaRegion *r = marker.region->next; r != NULL; r = r->next) {
        r->pos = sizeof(ArenaRegion);
    }
}

void *mem_reserve(u64 size) {
    void *ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }
    return ptr;
}

b32 mem_commit(void *ptr, u64 size) {
    assert(ptr != NULL);
    i32 out = mprotect(ptr, size, PROT_READ | PROT_WRITE);
    return out == 0;
}

b32 mem_free(void *ptr, u64 size) {
    assert(ptr != NULL);
    i32 out = munmap(ptr, size);
    return out == 0;
}
