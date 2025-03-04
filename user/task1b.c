#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid = fork();

  if (pid < 0)
    fprintf(2, "fork error\n");

  else if (pid > 0) {   // parent process
    fprintf(1, "PID: %d\tCPID: %d\n", getpid(), pid);
    int cpid, status;
    kill(pid);
    cpid = wait(&status);
    fprintf(1, "Return ID: %d\tReturn code: %d\n", cpid, status);
    exit(0);
  }

  else if (pid == 0) {  // child process
    sleep(6000);
    exit(1);
  }
}
