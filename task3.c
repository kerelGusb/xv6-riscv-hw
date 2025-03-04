#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
  int pfd[2];

  if (pipe(pfd) < 0) {
    fprintf(stderr, "pipe error\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(stderr, "fork error\n");
    exit(1);
  }

  if (pid == 0) {  
    close(pfd[1]); 

    char buffer[256];
    size_t bytesRead;

    while ((bytesRead = read(pfd[0], buffer, sizeof(buffer))) > 0) {
      write(1, buffer, bytesRead);
    }

    if (bytesRead < 0) {
      fprintf(stderr, "pipe read error\n");
    }

    close(pfd[0]);
    exit(0);
  }
  else {
    close(pfd[0]);

    for (int i = 1; i < argc; i++) {
      write(pfd[1], argv[i], strlen(argv[i]));
      write(pfd[1], "\n", 1);
    }

    close(pfd[1]);

    int status;
    waitpid(pid, &status, 0); 
  }

  return 0;
}
