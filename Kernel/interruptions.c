//interruptions.c
#include <interruptions.h>


extern uint64_t getStackPtr();

typedef struct {
	uint16_t offset_l; //bit 0..15
	uint16_t selector;
	uint8_t ist;
	uint8_t attrs;
	uint16_t offset_m; //16..31
	uint32_t offset_h; //32..63
	uint32_t zero_h;
} IDTEntry_t;

#pragma pack(pop)

typedef void (*handler_t)(void);

static IDTEntry_t* IDT = (IDTEntry_t*) 0x0;

void screenTickHandler() {
	static int count = 0;
	count++;
	if(count == 10) { //Cada 825ms
		blinkCursor();
		count = 0;
	}
}

void timerTickHandler(uint64_t rsp) {
	kernelMode();
	schedule();
	userMode();
}

void irqDispatcher(int irq) {
	switch(irq) {
		case 1:
			keyboardHandler();
			break;
		case 12:
			mouseHandler();
			break;
	}
}

void sendEOI(int irq) {
	if(irq >= 8)
		writePort(0xA0, 0x20);
	writePort(0x20, 0x20);
}

void iSetHandler(int index, uint8_t ist, uint64_t handler) {
	IDT[index].offset_l = (uint16_t) handler & 0xFFFF;
	IDT[index].offset_m = (uint16_t) (handler >> 16) & 0xFFFF;
	IDT[index].offset_h = (uint32_t) (handler >> 32) & 0xFFFFFFFF;
	IDT[index].selector = 0x08;
	IDT[index].attrs = 0xEE;
	IDT[index].ist = ist;
	IDT[index].zero_h = 0;
}

void debug(){
	uint64_t * reg = getStackPtr();
	while(reg <= KERNEL_STACKBASE) {
		newLine();
		printHex(*reg);
		reg++;
	}
	while(1);
}

void setupIDT() {
	//iSetHandler(0x0D, 0, (uint64_t) &debug);
	iSetHandler(0x0E, 0, (uint64_t) &PFHandler);
	iSetHandler(0x20, 2, (uint64_t) &TTHandler);
	iSetHandler(0x21, 2, (uint64_t) &irq1Handler);
	iSetHandler(0x2C, 2, (uint64_t) &irq12Handler);
	iSetHandler(0x80, 1, (uint64_t) &int80Handler);

	initializeMouse();
	sysCallsSetup();
	setupRTC();
	setupSemaphores();
	setupMessages();

	setPicMaster(0xF8);
	setPicSlave(0xEF);
}
