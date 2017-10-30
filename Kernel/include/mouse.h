#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdint.h>

uint8_t mouseRead();
void mouseWrite(uint8_t a_write);
void mouseWait(uint8_t type);
void initializeMouse();
void mouseHandler();
uint8_t moveMouse(char deltaX, char deltaY);
void copySelection();

#endif