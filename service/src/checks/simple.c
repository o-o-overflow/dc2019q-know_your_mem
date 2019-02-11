static int mio_errno = 0;
#define SYS_ERRNO mio_errno

#include "../linux-syscall-support/linux_syscall_support.h"


void _start()
{
    char s[] = "Ciao\n";
    if (sys_write(1, s, sizeof(s)) == sizeof(s))
        sys_exit_group(0);
    else sys_exit_group(1);
}
