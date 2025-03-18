#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

extern struct spinlock wait_lock;

uint64
sys_ps_listinfo(void) {
    uint64 uaddr;
    int lim;
    struct procinfo pinfo;
    struct proc *p;
    int count = 0;
    argaddr(0, &uaddr);
    argint(1, &lim);
    int proc_count = 0;

    if (uaddr == 0) {
      for (p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if (p->state != UNUSED)
            proc_count++;
        release(&p->lock);
      }
      return proc_count;
    }

    if (proc_count > lim)
        return -proc_count;

    acquire(&wait_lock);
    for (p = proc; p < &proc[NPROC]; ++p) {
        if (count >= lim) break;
        acquire(&p->lock);
        if (p->state != UNUSED) {
            pinfo.pid = p->pid;
            pinfo.ppid = p->parent ? p->parent->pid : 0;
            pinfo.state = p->state;

            safestrcpy(pinfo.name, p->name, sizeof(pinfo.name));
            if (copyout(myproc()->pagetable, uaddr + count * sizeof(struct procinfo), (char *)&pinfo, sizeof(struct procinfo)) < 0) {
                release(&p->lock);
                release(&wait_lock);
                return -2;
            }
            count++;
        }
        release(&p->lock);
    }
    release(&wait_lock);

    return count;
}

