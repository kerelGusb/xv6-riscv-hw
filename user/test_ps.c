#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

void test_ps() {
    int count = ps_listinfo(0, 0);
    printf("Proc count: %d\n", count);

    struct procinfo procs[2];
    int result = ps_listinfo(procs, 2);

    if (result < 0)
        printf("Error: buffer is too small (code %d)\n", result);
    else
        printf("Got %d processes\n", result);

    int invalid = ps_listinfo((struct procinfo *)0xFFFFFFFF, 5);
    if (invalid < 0)
        printf("Error: invalid address\n");
}

int main() {
    test_ps();
    exit(0);
}
