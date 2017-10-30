#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

extern int int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);

char * array[] = {"Alooo"};

int main(int argc, char *argv[]) {
  char buffer[128];
  while(1) {
    printf("$> ");
    scanf("%s", buffer);
    if(execv(buffer,1,array) == -1)
      printf("%s: Invalid Module\n", buffer);
  }

  return 0;
}

int execv(char *filename, int argc, char *argv[]) {
	return int80(7, filename, argc, argv);
}
