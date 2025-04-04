#include "kernel/types.h" 
#include "user/user.h"

#define MSIZE 30000

int 
main() {
    printf("Table at start:\n");
    vmprint(0, 0, 0);

    uint64 a = 3;
    char stackbuf[1000];

    char *heapbuf = malloc(MSIZE);
    printf("\n\nTable after malloc\n");
    vmprint(0, 0, 0);

    remflags(0, 0, 3);
    printf("\n\nTable after removing flags:\n");
    vmprint(0, 0, 0);

    for (int i = 0; i < MSIZE; ++i)
        a ^= heapbuf[i];
    printf("\n\nTable after reading data:\n");
    vmprint(heapbuf, MSIZE, 0);
    
    for (int i = 0; i < MSIZE; ++i)
        heapbuf[i] = i;
    printf("\n\nTable after changing data:\n");
    vmprint(heapbuf, MSIZE, 0);

    free(heapbuf);
    printf("\n\nTable after malloc free:\n");
    vmprint(0, 0, 0);

    remflags(0, 0, 1);
    printf("\n\nTable after removing D flags:\n");
    vmprint(0, 0, 0);

    stackbuf[1] = 0;
    a = stackbuf[1];

    exit(0);
}
