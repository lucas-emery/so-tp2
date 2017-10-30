void changePDEPresent(int entry, int present);
void setKernelPresent(int present);
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]);
void copyAndExectueDefaultModule();
void changePDE(int entry, uint64_t* physAddr, int present);
void changePDEPresent(int entry, int present);
void * initializeKernelBinary();
void * malloc(uint64_t size);
