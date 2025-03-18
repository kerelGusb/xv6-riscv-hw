#ifndef PROCINFO_H
#define PROCINFO_H

#include "types.h"

struct procinfo {
    int pid;
    char name[16];
    int state;
    int ppid;
};

#endif