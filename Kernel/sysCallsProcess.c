#include <process.h>
#include <sysCallsProcess.h>

int sysForkProcess(uint64_t pidReturn, uint64_t rdx, uint64_t rcx){
	//*pidReturn = addChild(fatherPid); que pid de papa le paso?!
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