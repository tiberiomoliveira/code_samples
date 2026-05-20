/**
 * Custom malloc() / free() — interview-ready implementation
 *
 * Strategy: Explicit free list with first-fit search.
 *
 * Memory layout
 * ─────────────
 * A single large arena is carved out with mmap() (or a static array in
 * environments where mmap is unavailable).  Every allocation is preceded
 * by a BlockHeader that tracks its size and whether it is free.  Free
 * blocks form a singly-linked list so that free() can coalesce adjacent
 * free neighbours (forward merge) without scanning the whole arena.
 *
 *  ┌──────────────┬────────────── · · · ──┐
 *  │  BlockHeader │     user payload       │
 *  └──────────────┴────────────── · · · ──┘
 *  ↑                              ↑
 *  internal pointer          pointer returned to caller
 *
 * Design decisions worth discussing in an interview
 * ─────────────────────────────────────────────────
 *  1. First-fit  – O(n) per alloc; simple to explain.
 *     Alternatives: best-fit (less fragmentation, same asymptotic),
 *     segregated free lists / slab allocator (O(1) amortised).
 *  2. Block splitting – avoids wasting space on over-sized blocks.
 *  3. Coalescing on free() – merges with the *next* free block only
 *     (forward merge).  Full bi-directional coalescing requires a
 *     doubly-linked list or boundary tags (Knuth / Wilson).
 *  4. Alignment – every returned pointer is ALIGNMENT-byte aligned
 *     (16 bytes on 64-bit; matches glibc malloc).
 *  5. Thread safety – NOT implemented here; add a std::mutex around
 *     my_malloc/my_free for a production allocator.
 */

#include <cstddef>    // size_t, nullptr
#include <cstdint>    // uintptr_t
#include <cstring>    // memset
#include <cassert>
#include <iostream>
#include <new>        // std::bad_alloc (demo only)

// ─── tunables ────────────────────────────────────────────────────────────────

static constexpr std::size_t HEAP_SIZE  = 64 * 1024;   // 64 KiB arena
static constexpr std::size_t ALIGNMENT  = alignof(std::max_align_t); // 16 on x86-64
static constexpr std::size_t MIN_BLOCK  = 32;           // don't split below this

// ─── helpers ─────────────────────────────────────────────────────────────────

/// Round `n` up to the nearest multiple of ALIGNMENT.
static inline std::size_t align_up(std::size_t n) {
    // Non negative number: ((numToRound + multiple - 1) / multiple) * multiple
    // Round to the nearest multiple that happens to be a power of 2
    return (n + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

// ─── block header ────────────────────────────────────────────────────────────

struct BlockHeader {
    std::size_t   size;      // size of the *payload* (bytes), always aligned
    BlockHeader*  next_free; // intrusive singly-linked free list

    // Pointer arithmetic helpers
    void*        payload()       { return this + 1; }
    BlockHeader* next_block()    {
        return reinterpret_cast<BlockHeader*>(
            reinterpret_cast<std::byte*>(payload()) + size);
    }
};

// Header size is itself always a multiple of ALIGNMENT, so the payload
// that follows is properly aligned.
static_assert(sizeof(BlockHeader) % ALIGNMENT == 0 ||
              alignof(BlockHeader) <= ALIGNMENT,
              "BlockHeader size breaks payload alignment");

// ─── heap arena ──────────────────────────────────────────────────────────────

/// Backing store: a static byte array (swap for mmap in a real allocator).
alignas(std::max_align_t) static std::byte heap[HEAP_SIZE];

static BlockHeader* free_list_head = nullptr;  // head of the free list
static bool         initialized    = false;

// ─── initialisation ──────────────────────────────────────────────────────────

static void heap_init() {
    // Create one giant free block that covers the whole arena.
    free_list_head = reinterpret_cast<BlockHeader*>(heap);
    free_list_head->size      = HEAP_SIZE - sizeof(BlockHeader);
    free_list_head->next_free = nullptr;
    initialized = true;
}

// ─── my_malloc ───────────────────────────────────────────────────────────────

/**
 * Allocate at least `size` bytes.
 *
 * Returns a pointer aligned to ALIGNMENT bytes, or nullptr on failure.
 * Complexity: O(n) first-fit walk of the free list.
 */
void* my_malloc(std::size_t size) {
    if (size == 0) return nullptr;
    if (!initialized) heap_init();

    const std::size_t needed = align_up(size);  // align the request

    // ── first-fit search ────────────────────────────────────────────────────
    BlockHeader* prev = nullptr;
    BlockHeader* cur  = free_list_head;

    while (cur) {
        if (cur->size >= needed) {
            // ── found a big-enough block ────────────────────────────────────

            // Split if the remainder is large enough to be its own block.
            const std::size_t remainder = cur->size - needed;
            if (remainder >= sizeof(BlockHeader) + MIN_BLOCK) {
                // Carve a new free block from the tail of `cur`.
                auto* split = reinterpret_cast<BlockHeader*>(
                    reinterpret_cast<std::byte*>(cur->payload()) + needed);
                split->size      = remainder - sizeof(BlockHeader);
                split->next_free = cur->next_free;

                cur->size      = needed;
                cur->next_free = split;   // temporarily; removed below
            }

            // Remove `cur` from the free list.
            if (prev)
                prev->next_free = cur->next_free;
            else
                free_list_head = cur->next_free;

            cur->next_free = nullptr;
            return cur->payload();
        }

        prev = cur;
        cur  = cur->next_free;
    }

    // No suitable block found.
    return nullptr;
}

// ─── my_free ─────────────────────────────────────────────────────────────────

/**
 * Release memory previously returned by my_malloc().
 *
 * Attempts forward-coalescing with the next block if it is free.
 * Complexity: O(n) to find insertion point and check the neighbour.
 */
void my_free(void* ptr) {
    if (!ptr) return;   // free(nullptr) is a no-op per the C standard.

    // Recover the header that sits just before the payload.
    auto* block = reinterpret_cast<BlockHeader*>(ptr) - 1;

    // ── insert into free list (kept in address order for coalescing) ─────────
    BlockHeader* prev = nullptr;
    BlockHeader* cur  = free_list_head;

    while (cur && cur < block) {
        prev = cur;
        cur  = cur->next_free;
    }

    // Link block between prev and cur.
    block->next_free = cur;
    if (prev)
        prev->next_free = block;
    else
        free_list_head = block;

    // ── forward coalesce: block → next ───────────────────────────────────────
    BlockHeader* next_blk = block->next_block();
    // Check next_blk is inside the arena before dereferencing.
    if (reinterpret_cast<std::byte*>(next_blk) < heap + HEAP_SIZE
        && next_blk == block->next_free)
    {
        block->size     += sizeof(BlockHeader) + next_blk->size;
        block->next_free = next_blk->next_free;
    }

    // ── backward coalesce: prev → block ──────────────────────────────────────
    if (prev && prev->next_block() == block) {
        prev->size     += sizeof(BlockHeader) + block->size;
        prev->next_free = block->next_free;
    }
}

// ─── optional: my_calloc / my_realloc ────────────────────────────────────────

void* my_calloc(std::size_t count, std::size_t size) {
    const std::size_t total = count * size;
    void* ptr = my_malloc(total);
    if (ptr) std::memset(ptr, 0, total);
    return ptr;
}

void* my_realloc(void* ptr, std::size_t new_size) {
    if (!ptr)        return my_malloc(new_size);
    if (!new_size) { my_free(ptr); return nullptr; }

    auto* block = reinterpret_cast<BlockHeader*>(ptr) - 1;
    if (block->size >= align_up(new_size)) return ptr;  // already large enough

    void* new_ptr = my_malloc(new_size);
    if (!new_ptr) return nullptr;
    std::memcpy(new_ptr, ptr, block->size);   // copy the OLD (smaller) payload
    my_free(ptr);
    return new_ptr;
}

// ─── diagnostics ─────────────────────────────────────────────────────────────

void heap_dump() {
    std::cout << "\n=== heap dump ===\n";
    const std::byte* end      = heap + HEAP_SIZE;
    auto*            cur      = reinterpret_cast<BlockHeader*>(heap);
    int              block_id = 0;

    while (reinterpret_cast<const std::byte*>(cur) < end) {
        std::cout << "  [" << block_id++ << "] "
                  << "  size=" << cur->size
                  << "  @" << static_cast<void*>(cur) << "\n";

        // Advance to next contiguous block.
        auto* next = cur->next_block();
        if (reinterpret_cast<const std::byte*>(next) >= end) break;
        cur = next;
    }
    std::cout << "=================\n\n";
}

// ─── demo ────────────────────────────────────────────────────────────────────

int main() {
    // --- basic allocation ---
    int*       a = static_cast<int*>   (my_malloc(sizeof(int) * 4));
    double*    b = static_cast<double*>(my_malloc(sizeof(double)));
    std::byte* c = static_cast<std::byte*>  (my_calloc(8, sizeof(std::byte)));

    assert(a && b && c);
    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4;
    *b   = 3.14;
    c[0] = 'H'; c[1] = 'i'; c[2] = '\0';

    std::cout << "a[0..3] = " << a[0] << " " << a[1] << " "
              << a[2] << " " << a[3] << "\n";
    std::cout << "*b = " << *b << "\n";
    std::cout << "c  = " << c  << "\n";

    heap_dump();   // three used blocks

    // --- free + coalesce ---
    my_free(a);
    my_free(b);    // not adjacent to a, so no merge yet
    my_free(c);
    heap_dump();   // should coalesce back to one large free block

    // --- realloc ---
    int* d = static_cast<int*>(my_malloc(sizeof(int) * 2));
    d[0] = 10; d[1] = 20;
    d = static_cast<int*>(my_realloc(d, sizeof(int) * 6));
    assert(d);
    d[2] = 30; d[3] = 40; d[4] = 50; d[5] = 60;
    std::cout << "realloc'd array: ";
    for (int i = 0; i < 6; ++i) std::cout << d[i] << " ";
    std::cout << "\n";
    my_free(d);

    // --- alignment check ---
    void* p = my_malloc(1);
    assert(reinterpret_cast<uintptr_t>(p) % ALIGNMENT == 0);
    std::cout << "Alignment check passed (" << ALIGNMENT << "-byte aligned)\n";
    my_free(p);

    heap_dump();   // back to one big free block
    std::cout << "All tests passed.\n";
}
