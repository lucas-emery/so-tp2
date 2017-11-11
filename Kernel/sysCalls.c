#include <sysCalls.h>

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size) {
	int index = 0;
	char c;
	if(fileDescriptor == 0) {
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
			return createProcess(i, argc, argv);
		}
		i++;
	}
	return -1;
}

int sysMalloc(uint64_t address, uint64_t size, uint64_t rcx) {
	*((uint64_t *)address) = malloc(size);
	return 0;
}

int sysFree(uint64_t address, uint64_t rdx, uint64_t rcx){
	//TO DO
}

int sysOpenSem(uint64_t name, uint64_t value, uint64_t id){
	*((uint8_t *)id) = execute(OPEN, (char*) name, (int) value);
	return 0;
}

int sysCloseSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	execute(CLOSE, NULL, (int) id);
	return 0;
}

int sysUpSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	execute(POST, NULL, (int) id);
	return 0;
}

int sysDownSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	execute(WAIT, NULL, (int) id);
	return 0;
}

int sysPthread(uint64_t startRoutine, uint64_t arg, uint64_t rcx){

	return createThread(startRoutine, arg); //Crea una shell sin args
}

int sysKillProcess(uint64_t pid, uint64_t rdx, uint64_t rcx){
	removePCB(pid);
}

int sysListProcesses(uint64_t buffer, uint64_t rdx, uint64_t rcx){
	processesInfo(buffer);
}

int sysBlockProcess(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	//block();
}

int sysUnblockProcess(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	//unblock();
}

int sysYieldProcess(uint64_t rsi, uint64_t rdx, uint64_t rcx){
	//block();
}

int sysCreateMsg(uint64_t name, uint64_t messageSize, uint64_t rcx){
	createMessage(name, messageSize);
	return SUCCESS;
}

int sysDeleteMsg(uint64_t name, uint64_t rdx, uint64_t rcx){
	return closeMessage(name);
}

int sysWriteMsg(uint64_t name, uint64_t content, uint64_t rcx){
	writeMessage(name, content);
	return SUCCESS;
}

int sysReadMsg(uint64_t name, uint64_t buffer, uint64_t rcx){
	readMessage(name, buffer);
	return SUCCESS;
}

int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	if(rdi < 0 || rdi >= SYSCALLS)
		return -1; //Tirar error??
	return sysCalls[rdi](rsi, rdx, rcx);
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
	sysCalls[13] = &sysBlockProcess;
	sysCalls[14] = &sysUnblockProcess;
	sysCalls[15] = &sysYieldProcess;
	sysCalls[16] = &sysOpenSem;
	sysCalls[17] = &sysCloseSem;
	sysCalls[18] = &sysUpSem;
	sysCalls[19] = &sysDownSem;
	sysCalls[20] = &sysCreateMsg;
	sysCalls[21] = &sysDeleteMsg;
	sysCalls[22] = &sysWriteMsg;
	sysCalls[23] = &sysReadMsg;
}
