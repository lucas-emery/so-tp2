#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>
#include <lib.h>
#include <terminal.h>
#include <keyboard.h>
#include <mouse.h>
#include <video.h>
#include <sysCalls.h>
#include <rtc.h>
#include <MMU.h>
#include <semaphore.h>
#include <message.h>

#pragma pack(push)
#pragma pack(1)

void iSetHandler(int index, uint8_t ist, uint64_t handler);
void sendEOI(int irq);
void setupIDT();
void irqDispatcher(int irq);

extern void PFHandler();
extern void TTHandler();
extern void irq0Handler();
extern void irq1Handler();
extern void irq12Handler();
extern void int80Handler();
extern void sti();
extern void cli();
extern void setPicMaster(uint8_t mask);
extern void setPicSlave(uint8_t mask);

#endif
