#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <terminal.h>
#include <rtc.h>
#include <lib.h>
#include <MMU.h>
#include <process.h>
#include <IPC.h>

#define SYSCALLS 30

extern char* moduleNames[];

typedef int (*sys)(uint64_t rsi, uint64_t rdx, uint64_t rcx);

static sys sysCalls[SYSCALLS];

int sysRead(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);
int sysWrite(uint64_t fileDescriptor, uint64_t buffer, uint64_t size);
int sysSetTimeZone(uint64_t timeZone, uint64_t rdx, uint64_t rcx);
int sysGetTime(uint64_t hour, uint64_t minute, uint64_t seconds);
int sysGetDate(uint64_t day, uint64_t month, uint64_t year);
int sysEcho(uint64_t echoOn, uint64_t rdx, uint64_t rcx);
int sysCallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx);
int sysOpenSem(uint64_t name, uint64_t value, uint64_t id);
int sysCloseSem(uint64_t id, uint64_t rdx, uint64_t rcx);
int sysUpSem(uint64_t semaphore, uint64_t rdx, uint64_t rcx);
int sysDownSem(uint64_t semaphore, uint64_t rdx, uint64_t rcx);
void sysCallsSetup();

#endif
