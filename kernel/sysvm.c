#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "stat.h"
#include "spinlock.h"
#include "proc.h"
#include "fs.h"

#define LVLSZ(level) (1 << PXSHIFT(level))

#define PGROUNDUPLVL(sz, level)  (((sz)+(LVLSZ(level))-1) & ~((LVLSZ(level))-1))
#define PGROUNDDOWNLVL(a, level) (((a)) & ~((LVLSZ(level))-1))

void
print_pagetable_info(pte_t *pte, uint64 pa, int level, int i) {
    for (int j = 0; j < level * 8; ++j)
    printf(".");

    char* zeros = "";
    if (i < 99) zeros = "0";
    if (i < 9) zeros = "00";

    printf("0x%s%d -> %p ", zeros, i + 1, (pagetable_t)pa);
    printf("%s%s%s%s%s%s%s\n",
        (*pte & PTE_R) ? "R" : "_",
        (*pte & PTE_W) ? "W" : "_",
        (*pte & PTE_X) ? "X" : "_",
        (*pte & PTE_U) ? "U" : "_",
        (*pte & PTE_G) ? "G" : "_",
        (*pte & PTE_A) ? "A" : "_",
        (*pte & PTE_D) ? "D" : "_");
}

void 
remove_flags(pte_t *pte, int mask) {
    if (mask & 2) *pte &= ~PTE_A;
    if (mask & 1) *pte &= ~PTE_D;
}

// pgtb_buf_iterator -- applies function on each page of vm table which contains given buffer
// mask -- binary mask with AD bits
// pendva -- virtual address of end of page on higher level that contains this pagetable
// mode --
//  0 - print pagetable info;
//  1 - remove flags;

uint64 
pgtb_buf_iterator(pagetable_t pagetable, uint64 buf, uint64 len, int mask, int level, uint64 pendva, int mode) { 
    uint64 lvlsize = LVLSZ(2 - level);
    uint64 sva = PGROUNDDOWNLVL(buf, 2 - level);
    uint64 fva = PGROUNDUPLVL(buf + len, 2 - level);
    uint64 i = sva >> PXSHIFT(2 - level);
    while (sva < fva && sva < pendva) {
        if (i >= 512) break;
        
        pte_t *pte = &pagetable[i];

        if (!(*pte & PTE_V)) {
            sva += lvlsize;
            i += 1;
            continue;
        }

        if (level == 2 && (((mask & 2) && !(*pte & PTE_A)) || ((mask & 1) && !(*pte & PTE_D)))) {
            sva += lvlsize;
            i += 1;
            continue;
        }

        uint64 pa = PTE2PA(*pte);

        if (mode == 0)
            print_pagetable_info(pte, pa, level, i);
        else if (mode == 1 && level == 2)
            remove_flags(pte, mask);
        
        if ((*pte & PTE_V) && !(*pte & (PTE_R | PTE_W | PTE_X)))
            pgtb_buf_iterator((pagetable_t)pa, buf, len, mask, level + 1, sva + lvlsize, mode);
        
        sva += lvlsize;
        i += 1;
    }
    return 0;
}

uint64
sys_vmprint(void) { 
    struct proc *p = myproc();
    uint64 buf, len;
    int mask;
    argaddr(0, &buf);
    argaddr(1, &len);
    argint(2, &mask);
    if (buf < 0 || len < 0 || mask < 0)
        return -1;

    if (mask < 0 || mask > 3)
        return -1;

    if (buf == 0) buf = 0x0;
    if (len == 0) len = MAXVA - 1;
    
    printf("PAGETABLE %p\n", p->pagetable);
    pgtb_buf_iterator(p->pagetable, buf, len, mask, 0, MAXVA, 0);

    return 0;
}

uint64
sys_remflags(void) {
    struct proc *p = myproc();
    uint64 buf, len;
    int mask;
    argaddr(0, &buf);
    argaddr(1, &len);
    argint(2, &mask);
    if (buf < 0 || len < 0 || mask < 0)
        return -1;

    if (mask < 0 || mask > 3)
        return -1;

    if (buf == 0) buf = 0x0;
    if (len == 0) len = MAXVA;
    
    pgtb_buf_iterator(p->pagetable, buf, len, mask, 0, MAXVA, 1);

    return 0;
    
}