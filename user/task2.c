#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  int pfd[2];

  if (pipe(pfd) < 0) {
    fprintf("pipe error\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf("fork error\n");
    exit(1);
  }

  if (pid == 0) { 
    close(pfd[1]);

    close(0);
    dup(pfd[0]);
    close(pfd[0]);

    char* wc_argv[] = {"/wc", 0};
    exec("/wc", wc_argv);

    fprintf("exec wc error\n");  // в случае неудачи exec
    exit(1);
  }
  else {
    close(pfd[0]);

    for (int i = 1; i < argc; ++i) {
      write(pfd[1], argv[i], strlen(argv[i]));
      write(pfd[1], "\n", 1);
    }
    close(pfd[1]);

    wait(0);
  }

  exit(0);
}
