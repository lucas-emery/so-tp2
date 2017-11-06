#include <sysCalls.h>
#include <terminal.h>
#include <rtc.h>
#include <lib.h>
#include <MMU.h>
#include <sysCallsProcess.h>
#include <IPC.h>

extern char* moduleNames[];

#define SYSCALLS 30

typedef int (*sys)(uint64_t rsi, uint64_t rdx, uint64_t rcx);

static sys sysCalls[SYSCALLS];

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
			//argv = backupArguments(argc, argv);
			copyAndExecuteModule(i, argc, argv);
			return 0;
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

void sysOpenSem(uint64_t name, uint64_t value, uint64_t id){
	*id = sem_open(name, value);
}

void sysCloseSem(uint64_t id, uint64_t rdx, uint64_t rcx){
	sem_close(id);
}

void sysUpSem(uint64_t semaphore, uint64_t rdx, uint64_t rcx){
	sem_post(id);
}

void sysDownSem(uint64_t semaphore, uint64_t rdx, uint64_t rcx){
	sem_wait(id);
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
	sysCalls[10] = &sysForkProcess;
	sysCalls[11] = &sysKillProcess;
	sysCalls[12] = &sysListProcesses;
	sysCalls[13] = &sysBlockProcess;
	sysCalls[14] = &sysUnblockProcess;
	sysCalls[15] = &sysYieldProcess;
	sysCalls[16] = &sysOpenSem;
	sysCalls[17] = &sysCloseSem;
	sysCalls[18] = &sysUpSem;
	sysCalls[19] = &sysDownSem;
}
