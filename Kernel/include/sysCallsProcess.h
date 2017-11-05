#ifndef SYSCALLSPROCESS_H
#define SYSCALLSPROCESS_H

#include <stdint.h>

int sysForkProcess(uint64_t pidReturn, uint64_t rdx, uint64_t rcx);
int sysKillProcess(uint64_t pid, uint64_t rdx, uint64_t rcx);
int sysListProcesses(uint64_t rsi, uint64_t rdx, uint64_t rcx);
int sysBlockProcess(uint64_t pid, uint64_t rdx, uint64_t rcx);
int sysUnblockProcess(uint64_t pid, uint64_t rdx, uint64_t rcx);
int sysYieldProcess(uint64_t pid, uint64_t rdx, uint64_t rcx);

#endif