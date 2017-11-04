#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define STEP 10
#define BUFFERSIZE 128

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
int execv(char * filename, int argc, char * argv[]);
void freeParams(char ** argv);
void parseParams(char * command, int * argc, char *** argv);

int main() {
  char buffer[BUFFERSIZE];
  char ** argv;
  int argc;
  while(1) {
    printf("$> ");
    scanf("%s", buffer);
    parseParams(buffer, &argc, &argv);
    if(execv(argv[0], argc, argv) == -1) {
      printf("%s: Invalid Module\n", argv[0]);
      freeParams(argv);
    }
  }

  return 0;
}

void parseParams(char * command, int * argc, char *** argv) {
  char buffer[BUFFERSIZE];
  int count = 0, size = 0, i = 0, j = 0;
  do {
    if(command[i] != ' ' && command[i] != 0) {
      buffer[j] = command[i];
      j++;
    } else {
      if(size - count == 0) {
        size += STEP;
        (*argv) = (char **)malloc(sizeof(void*)*size);
      }
      (*argv)[count] = malloc(sizeof(char)*(j+1));
      for (int k = 0; k < j; k++) {
        (*argv)[count][k] = buffer[k];
      }
      (*argv)[count][j] = 0; //Null terminated
      count++;
      j = 0;
    }
  } while (command[i++] != 0);

  (*argc) = count;
}

int execv(char * filename, int argc, char * argv[]) {
	return int80(7, (uint64_t)filename, (uint64_t)argc, (uint64_t)argv);
}

void freeParams(char ** argv) {

}
