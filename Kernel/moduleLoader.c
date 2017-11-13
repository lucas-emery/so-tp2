#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <MMU.h>
#include <terminal.h>

static void loadModule(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUint32(uint8_t ** address);

extern uint32_t moduleCount;

void ** loadModules(void * payloadStart){
	int i;
	uint64_t moduleAddress = ROM;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	moduleCount = readUint32(&currentModule);
	void ** moduleAddresses = malloc(moduleCount * sizeof(void *));

	for (i = 0; i < moduleCount; i++) {
		changePDE(moduleAddress / PAGESIZE, getFreePage(), PRESENT);
		flushPaging();
		loadModule(&currentModule, (void*)moduleAddress);
		moduleAddresses[i] = (void*)moduleAddress;
		moduleAddress += PAGESIZE;
	}

	return moduleAddresses;
}

static void loadModule(uint8_t ** module, void * targetModuleAddress){
	uint32_t moduleSize = readUint32(module);

	ncPrint("  Will copy module at 0x");
	ncPrintHex((uint64_t)*module);
	ncPrint(" to 0x");
	ncPrintHex((uint64_t)targetModuleAddress);
	ncPrint(" (");
	ncPrintDec(moduleSize);
	ncPrint(" bytes)");

	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;

	ncPrint(" [Done]");
	ncNewline();
}

static uint32_t readUint32(uint8_t ** address){
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
