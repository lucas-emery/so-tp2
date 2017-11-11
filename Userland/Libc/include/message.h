#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <stdio.h>

extern void int80(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx); 

message_open(char* name);
message_close(char* name);
message_write(char* name, char* content);
essage_read(char* name, char* buffer);

#endif