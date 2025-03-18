#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

void print_processes() {
    int count = ps_listinfo(0, 0);
    if (count < 0) {
        printf("Getting proc count error\n");
        exit(1);
    }

    struct procinfo *procs = malloc(count * sizeof(struct procinfo));
    if (!procs) {
        printf("Memory allocation error\n");
        exit(1);
    }

    int actual = ps_listinfo(procs, count);
    if (actual < 0) {
        printf("Getting list of proc error\n");
        free(procs);
        exit(1);
    }

    printf("pid\tppid\tState\tName\n");
    for (int i = 0; i < actual; i++) {
        printf("%d\t%d\t%d\t%s\n", procs[i].pid, procs[i].ppid, procs[i].state, procs[i].name);
    }

    free(procs);
}

int main() {
    print_processes();
    exit(0);
}
