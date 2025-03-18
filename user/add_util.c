#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  const int MAX_LEN = 22;
  int i;
  char c, cc;
  char input[MAX_LEN];

  for (i = 0; i + 2 < MAX_LEN; ) {
    cc = read(0, &c, 1);
    if (cc < 1)
      break;
    if (c == '\n' || c == '\r')
      break;
    input[i++] = c;
  }
  input[i] = 0;

  printf("|%s|\n", input);

  char fst = input[0];

  if (!('0' <= fst && fst <= '9')) {
    write(2, "Error: Invalid input format\n", 28);
    exit(-1);
  }
  int a = atoi(input);
  char* nxt = strchr(input, ' ');
  if (*nxt == ' ') {
    nxt++;
    if (!('0' <= *nxt && *nxt <= '9')) {
      write(2, "Error: Invalid input format\n", 28);
      exit(-2);
    }
    int b = atoi(nxt);
    int sum = a + b;
    printf("%d\n", sum);
  }
  else {
    write(2, "Error: Invalid input format\n", 28);
    exit(-3);
  }
}
