#include <sysCalls.h>

static sys sysCalls[SYSCALLS];

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size) {
	int index = 0;
	char c;
	if(getCurrentProcess() != getFocusedPID())
		return 1;
	if(fileDescriptor == 0) {
		if((c = readBuffer()) == NULL)
			block(0, STDIN);
		*((char*)buffer++)= c;
		index++;
		while(index++ < size)
			*((char*)buffer++)= readBuffer();
	}
	return 0;
}

int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size) {
	if(fileDescriptor == 1 || fileDescriptor == 2) {
		char next;
		while(size--) {
			next = *(char *)(buffer++);
			if(fileDescriptor == 1)
				printc(next);
			else
				printcWithStyle(next, 0x04);
		}
	}
	return 0;
}

int sysClear(uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	clearScreen();
	return 0;
}

int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx) {
	setTimeZone((int) timeZone);
	return 0;
}

int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds) { /*Puede optimizarse*/
	*(int*)hour = getTime(HOURS);
	*(int*)minute = getTime(MINUTES);
	*(int*)seconds = getTime(SECONDS);
	return 0;
}

int sysGetDate(uint64_t day, uint64_t month, uint64_t year) {
	*(int*)day = getTime(DAY);
	*(int*)month = getTime(MONTH);
	*(int*)year = getTime(YEAR);
	return 0;
}

int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx) {
	setEcho((uint8_t) echoOn);
	return 0;
}

int sysExec(uint64_t filename, uint64_t argc, uint64_t argv) {
	int i = 0;
	while(moduleNames[i] != 0){
		if(strcmp(filename, moduleNames[i]) == 0) {
			if(argc == 0) {
				return createProcess(i, argc, argv);
			}
			else if(strcmp(((char**)argv)[argc-1], "&") == 0) {
				return createProcess(i, argc-1, argv);
			}
			else {
				int pid = createProcess(i, argc, argv);
				setFocusedPID(pid);
				exitCurrentProcess(0);
				return pid;
			}
		}
		i++;
	}
	return -1;
}

int sysMalloc(uint64_t address, uint64_t size, uint64_t rcx) {
	userMode();
	*((uint64_t *)address) = malloc(size);
	kernelMode();
	return 0;
}

int sysFree(uint64_t address, uint64_t rdx, uint64_t rcx){
	//Change to userMode
	//TO DO
	//Return to kernelMode
	return 0;
}

int sysSetSem(uint64_t id, uint64_t value, uint64_t rcx){
	return executeSemaphore(SET, NULL, (int)id, (int)value);
}

int sysOpenSem(uint64_t name, uint64_t value, uint64_t id){
	return executeSemaphore(OPEN, (char*) name, (int) value, NULL);
}

int sysCloseSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	return executeSemaphore(CLOSE, NULL, (int) id, NULL);
}

int sysUpSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	return executeSemaphore(POST, NULL, (int) id, NULL);
}

int sysDownSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	return executeSemaphore(WAIT, NULL, (int) id, NULL);
}

int sysPthread(uint64_t startRoutine, uint64_t arg, uint64_t rcx){
	return createThread(startRoutine, arg); //Crea una shell sin args
}

int sysKillProcess(uint64_t pid, uint64_t rdx, uint64_t rcx){
	exitProcess(pid, 1);
	return 0;
}

int sysListProcesses(uint64_t buffer, uint64_t rdx, uint64_t rcx){
	processesInfo(buffer);
	return 0;
}

int sysYieldProcess(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	//block();
}

int sysInitMsg(uint64_t name, uint64_t messageSize, uint64_t rcx){
	return executeMessage(INIT, (char*)name, (int)messageSize);
}

int sysOpenMsg(uint64_t name, uint64_t rdx, uint64_t rcx){
	return executeMessage(OPEN, (char*)name, NULL);
}

int sysDeleteMsg(uint64_t id, uint64_t rdx, uint64_t rcx){
	return executeMessage(CLOSE, NULL, (int)id);
}

int sysWriteMsg(uint64_t id, uint64_t content, uint64_t rcx){
	return executeMessage(WRITE, NULL, (int)id);
}

int sysReadMsg(uint64_t id, uint64_t buffer, uint64_t rcx){
	return executeMessage(READ, (char*)buffer, (int)id);
}

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	if(rdi < 0 || rdi >= SYSCALLS)
		return -1; //Tirar error??
	return sysCalls[rdi](rsi, rdx, rcx);
}

int sysExit(uint64_t value, uint64_t rdx, uint64_t rcx){
	exitCurrentProcess(value);
	return 0;
}

int sysExitThread(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	killThread();
	return 0;
}

int sysGetPid(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	return getCurrentProcess();
}

int sysGetVar(uint64_t name, uint64_t buffer, uint64_t rcx){
	if(strcmp((char*) name,"$?") == 0)
		uintToBase(exitValue,buffer,10);
	else if(strcmp((char*) name, "$sems") == 0)
		semString((char*) buffer);
	else if(strcmp((char*) name, "$msgs") == 0)
		msgString((char*) buffer);
	return 0;
}

void sysCallsSetup(){
	sysCalls[0] = &sysRead;
	sysCalls[1] = &sysWrite;
	sysCalls[2] = &sysClear;
	sysCalls[3] = &sysSetTimeZone;
	sysCalls[4] = &sysGetTime;
	sysCalls[5] = &sysGetDate;
	sysCalls[6] = &sysEcho;
	sysCalls[7] = &sysExec;
	sysCalls[8] = &sysMalloc;
	sysCalls[9] = &sysFree;
	sysCalls[10] = &sysPthread;
	sysCalls[11] = &sysKillProcess;
	sysCalls[12] = &sysListProcesses;
	sysCalls[13] = &sysYieldProcess;
	sysCalls[14] = &sysInitMsg;
	sysCalls[15] = &sysSetSem;
	sysCalls[16] = &sysOpenSem;
	sysCalls[17] = &sysCloseSem;
	sysCalls[18] = &sysUpSem;
	sysCalls[19] = &sysDownSem;
	sysCalls[20] = &sysOpenMsg;
	sysCalls[21] = &sysDeleteMsg;
	sysCalls[22] = &sysWriteMsg;
	sysCalls[23] = &sysReadMsg;
	sysCalls[24] = &sysExit;
	sysCalls[25] = &sysExitThread;
	sysCalls[26] = &sysGetPid;
	sysCalls[27] = &sysGetVar;
}
