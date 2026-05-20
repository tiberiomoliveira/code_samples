// C++ port of the K&R storage allocator
// Original C source: https://stackoverflow.com/a/36512105 (dpritch, CC BY-SA 3.0)

#include <unistd.h>   // sbrk
#include <cstddef>    // std::size_t, std::nullptr_t
#include <new>        // std::bad_alloc (optional throw path)

namespace kandr {

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------

static constexpr unsigned NALLOC = 1024; // Minimum block units requested from OS

// ---------------------------------------------------------------------------
// Header
//
// Every allocated (and free) region is preceded by one Header unit.  The union
// ensures the header is at least as large and as strictly aligned as the
// platform's most restrictive scalar type (long), so that the payload memory
// that follows the header satisfies any ordinary alignment requirement.
// ---------------------------------------------------------------------------

union Header {
    struct {
        Header  *next;   // Next block on the free list
        unsigned size;   // Size of this block in Header units (including header)
    } s;

    long x;              // Alignment member — never accessed directly
};

// ---------------------------------------------------------------------------
// Free-list state (translation-unit scope, mirrors the original file-statics)
// ---------------------------------------------------------------------------

static Header  base;            // Sentinel node that anchors the circular list
static Header *freep = nullptr; // Roving pointer into the free list

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------

static Header *morecore(unsigned nunits);
void           kandr_free(void *ptr);

// ---------------------------------------------------------------------------
// kandr_malloc
//
// Allocates at least nbytes of memory and returns a pointer to it, or nullptr
// if the OS cannot provide more memory.
// ---------------------------------------------------------------------------
void *kandr_malloc(unsigned nbytes)
{
    // Compute required units: one for the header, the rest for the payload.
    // The expression ((nbytes + sizeof(Header) - 1) / sizeof(Header)) + 1
    // rounds up to the next whole Header unit and adds one for the header
    // itself.  This works for all nbytes > 0.
    unsigned nunits =
        ((nbytes + sizeof(Header) - 1u) / sizeof(Header)) + 1u;

    // Initialise the free list the first time we are called.
    if (freep == nullptr) {
        base.s.next = &base; // Circular: points to itself
        base.s.size = 0;
        freep = &base;
    }

    Header *prevp = freep;
    Header *currp = prevp->s.next;

    // Walk the circular free list searching for a block large enough.
    for (;; prevp = currp, currp = currp->s.next) {

        if (currp->s.size >= nunits) {

            if (currp->s.size == nunits) {
                // Exact fit: splice the block out of the list.
                // (Safe because base has size 0 and nunits >= 1, so we can
                // never arrive here with currp == &base.)
                prevp->s.next = currp->s.next;
            } else {
                // Larger than needed: carve off the tail for the caller and
                // leave the remainder on the free list.
                currp->s.size -= nunits;        // Shrink the free block
                currp          += currp->s.size; // Advance to the new block
                currp->s.size   = nunits;        // Record its size
            }

            freep = prevp; // Next search starts from here (next-fit policy)

            // Return a pointer just past the header so the caller never sees it.
            return static_cast<void *>(currp + 1);
        }

        // We have lapped the list without finding a fit — ask the OS for more.
        if (currp == freep) {
            // morecore may call kandr_free, which can change freep, so we must
            // reassign currp from the return value rather than relying on the
            // equality we just tested.
            currp = morecore(nunits);
            if (currp == nullptr) {
                return nullptr; // OS is out of memory
            }
        }
    }
}

// ---------------------------------------------------------------------------
// morecore
//
// Requests at least nunits Header-sized units from the OS via sbrk, wraps the
// raw memory in a Header, inserts it into the free list via kandr_free, and
// returns the updated freep so that the caller can immediately find the new
// block.
// ---------------------------------------------------------------------------
static Header *morecore(unsigned nunits)
{
    // Amortise expensive sbrk calls by always requesting at least NALLOC units.
    if (nunits < NALLOC) {
        nunits = NALLOC;
    }

    // sbrk(n) extends the data segment by n bytes and returns the old break
    // (i.e. the first byte of the newly allocated region), or (void *)-1 on
    // failure.
    void *freemem = sbrk(static_cast<intptr_t>(nunits * sizeof(Header)));
    if (freemem == reinterpret_cast<void *>(-1)) {
        return nullptr;
    }

    // Overlay a Header at the start of the raw memory and record the block size.
    Header *insertp        = static_cast<Header *>(freemem);
    insertp->s.size        = nunits;

    // Hand the block to kandr_free (passing the payload address, not the header)
    // so it is inserted into the free list and potentially coalesced with
    // adjacent blocks.
    kandr_free(static_cast<void *>(insertp + 1));

    // freep now points to the block immediately before the newly inserted one,
    // so the very next iteration in kandr_malloc will land on the new block.
    return freep;
}

// ---------------------------------------------------------------------------
// kandr_free
//
// Returns the block pointed to by ptr to the free list, coalescing it with any
// physically adjacent free blocks to prevent fragmentation.
// ---------------------------------------------------------------------------
void kandr_free(void *ptr)
{
    // Step back one Header unit to reach the block's own header.
    Header *insertp = static_cast<Header *>(ptr) - 1;

    // Find the correct position in the address-ordered free list.
    //
    // We want the node currp such that  currp < insertp < currp->s.next
    // (in address space).  The loop exits via the break when the free list
    // wraps around (insertp is either the new leftmost or new rightmost block).
    Header *currp = freep;
    for (;
         !(insertp > currp && insertp < currp->s.next);
         currp = currp->s.next)
    {
        // currp >= currp->s.next means the list has wrapped (currp is the
        // rightmost block).  If insertp falls outside the [next, currp]
        // interior it must be the new extreme block on one end.
        if (currp >= currp->s.next &&
            (insertp > currp || insertp < currp->s.next)) {
            break;
        }
    }

    // --- Link to the right neighbour ---
    if (insertp + insertp->s.size == currp->s.next) {
        // insertp is left-adjacent to the next free block: coalesce right.
        insertp->s.size += currp->s.next->s.size;
        insertp->s.next  = currp->s.next->s.next;
    } else {
        // No right adjacency: just point forward.
        insertp->s.next = currp->s.next;
    }

    // --- Link from the left neighbour ---
    if (currp + currp->s.size == insertp) {
        // currp is left-adjacent to insertp: coalesce left (absorb insertp).
        currp->s.size += insertp->s.size;
        currp->s.next  = insertp->s.next;
    } else {
        // No left adjacency: point currp forward to insertp.
        currp->s.next = insertp;
    }

    // Park freep at currp so the next malloc search starts near fresh memory.
    freep = currp;
}

} // namespace kandr
