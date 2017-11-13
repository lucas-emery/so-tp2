#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int message_init(char* name, int messageSize);
int message_open(char* name);
int message_close(int id);
int message_write(int id, char* content);
char* message_read(int id, char* buffer);

#endif
