#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

void print_processes() {
    int buf_size = ps_listinfo(0, 0) * 2;

    struct procinfo *procs = malloc(buf_size * sizeof(struct procinfo));
    if (!procs) {
        printf("Memory allocation error\n");
        exit(1);
    }

    int actual = ps_listinfo(procs, buf_size);
    if (actual < 0) {
        printf("Procinfo write error\n");
        exit(2);
    }

    while (actual == buf_size + 1) {
        buf_size <<= 1;
        free(procs);
        procs = malloc(buf_size * sizeof(struct procinfo));

        if (!procs) {
            printf("Memory allocation error\n");
            exit(1);
        }

        int actual = ps_listinfo(procs, buf_size);
        if (actual < 0) {
            printf("Procinfo write error\n");
            exit(2);
        }
    }

    static char *states[] = { "UNUSED", "USED", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE" };

    printf("pid\tppid\tName\tState\n");
    for (int i = 0; i < actual; i++) {
        printf("%d\t%d\t%s\t%s\n", procs[i].pid, procs[i].ppid, procs[i].name, states[procs[i].state]);
    }

    free(procs);
}

int main() {
    print_processes();
    exit(0);
}
