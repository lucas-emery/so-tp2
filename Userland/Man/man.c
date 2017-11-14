#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const char * modules[][2] = {
  "accessKernel","Attempts to read data from the kernel.\nParams: None",
  "clear","Clears the screen.\nParams: None",
  "date","Prints today's date.\nParams: None",
  "echo","Prints a line of text.\nParams: [Text | $Variable]",
  "hello","Prints arguments.\nParams: Any",
  "help","Legacy help module similar to man.\nParams: None",
  "kill","Kills the specified process.\nParams: [PID]",
  "ls","Prints available modules.\nParams: None",
  "man","Provides information about a module.\nParams: [Module]",
  "philosophers","Prints solution to philosophers problem.\nParams: None",
  "ps","Prints information about the system processes.\nParams: None",
  "roflmao","Easter Egg.\nParams: None",
  "ts","Prints information about the system threads.\nParams: None",
  "sem","Performs a sem_post on the 'topkek' semaphore.\nParams: None",
  "time","Prints the actual time.\nParams: None",
  "prodcons","Prints solution to producer-consumer problem.\nParams: create producer letter, create consumer letter, remove producer letter, remove consumer letter",
  0};

int main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("Use 'man (module)' to get more information about a module.\n");
  }
  else {
    for (int i = 0; modules[i][0] != 0; i++) {
      if(strcmp(modules[i][0], argv[1]) == 0) {
        printf("%s\n",modules[i][1]);
        return 0;
      }
    }
    printf("No information available for %s.\n", argv[1]);
  }
  return 0;
}
