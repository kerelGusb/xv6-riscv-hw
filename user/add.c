#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXBUF 30

int
main(void)
{
  int ret;

  ret = add(2, 3);
  printf(ret == 5 ? "Test 1: success (%d)\n" : "Test 1: add error (%d)\n", ret);

  ret = add(200, 30000000);
  printf(ret == 30000200 ? "Test 2: success (%d)\n" : "Test 2: add error (%d)\n", ret);

  ret = add(-3, 492);
  printf(ret == -1 ? "Test 2: success (%d)\n" : "Test 2: add error (%d)\n", ret);

  ret = add(0, -3);
  printf(ret == -2 ? "Test 2: success (%d)\n" : "Test 2: add error (%d)\n", ret);
}
