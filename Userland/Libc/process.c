#include <process.h>

int exec(char * command) {
	char ** argv;
  int argc;
	parseParams(command, &argc, &argv);
	if(argc == 0) {
		printf("Please specify a module.\nUse 'ls' to get the list of available modules.\n");
		return -1;
	}
	int ret = execv(argv[0], argc, argv);
	freeParams(argv);
	return ret;
}

int execv(char * filename, int argc, char * argv[]){
	int ret = int80(7, (uint64_t)filename, (uint64_t)argc, (uint64_t)argv);
	if(ret == -1)
		printf("%s: Invalid Module\n", argv[0]);
	return ret;
}

int pthread_create(void *(*startRoutine)(void*), void * arg){
	return int80(10, startRoutine, arg, 0);
}

int getPid(){
	return int80(26,0,0,0);
}

int getVar(char* name){
	return int80(27,name,0,0);
}

void kill(int pid){
	int80(11,pid,0,0);
}

char* ps(char* buffer){
	int80(12, buffer,0,0);
	return buffer;
}

void block(int pid){
	int80(13,pid,0,0);
}

void unblock(int pid){
	int80(14,pid,0,0);
}

void yield(){
	int80(15,0,0,0);
}

void exit(int value){
	uint64_t ret = 0;
	ret += value;
	int80(24, ret,0,0);
}

void pthread_exit(){
	int80(25,0,0,0);
}

void parseParams(char * command, int * argc, char *** argv) {
  char buffer[BUFFERSIZE];
  int count = 0, size = 0, i = 0, j = 0;
  do {
    if(command[i] != ' ' && command[i] != 0) {
      buffer[j] = command[i];
      j++;
    } else if(j != 0) {
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

void freeParams(char ** argv) {

}
