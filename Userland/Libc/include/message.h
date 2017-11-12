#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void message_open(char* name, int messageSize);
void message_close(char* name);
void message_write(char* name, char* content);
void message_read(char* name, char* buffer);

#endif
