#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64 sys_add(void)
{
  int a = 0, b = 0;
  
  argint(0, &a);
  if (a < 0) return -1;

  argint(1, &b);
  if (b < 0) return -2;

  return a + b;
}
